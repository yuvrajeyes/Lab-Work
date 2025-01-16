import numpy as np
import torch 
import torch.nn as nn
from itertools import chain

from utils import create_block_from_cfgfile


def create_modules(blocks):
    """
    Constructs module list of layer blocks from module configuration in blocks
    """
    hyperparameters = blocks[0]  #Captures the information about the input and pre-processing
    hyperparameters.update({
        'batch': int(hyperparameters['batch']),
        'subdivisions': int(hyperparameters['subdivisions']),
        'width': int(hyperparameters['width']),
        'height': int(hyperparameters['height']),
        'channels': int(hyperparameters['channels']),
        'optimizer': hyperparameters.get('optimizer'),
        'momentum': float(hyperparameters['momentum']),
        'decay': float(hyperparameters['decay']),
        'learning_rate': float(hyperparameters['learning_rate']),
        'burn_in': int(hyperparameters['burn_in']),
        'max_batches': int(hyperparameters['max_batches']),
        'policy': hyperparameters['policy'],
        'lr_steps': list(zip(map(int,   hyperparameters["steps"].split(",")),
                             map(float, hyperparameters["scales"].split(","))))
    })
    assert hyperparameters["height"] == hyperparameters["width"], \
        "Height and width should be equal! Non square images are padded with zeros."
    
    module_list = nn.ModuleList()
    prev_filters = hyperparameters["channels"]
    output_filters = []

    for index, block in enumerate(blocks[1:]):
        module = nn.Sequential()

        #check the type of block
        #create a new module for the block
        #append to module_list
        if (block["type"] == "convolutional"):
            #Get the info about the layer
            activation = block["activation"]
            try:
                batch_normalize = int(block["batch_normalize"])
                bias = False
            except:
                batch_normalize = 0
                bias = True

            filters= int(block["filters"])
            padding = int(block["pad"])
            kernel_size = int(block["size"])
            stride = int(block["stride"])

            if padding: 
                pad = (kernel_size - 1) // 2  # same padding
            else:
                pad = 0  # valid padding

            #Add the convolutional layer
            conv = nn.Conv2d(in_channels=prev_filters, out_channels=filters, kernel_size=kernel_size, stride=stride, padding=pad, bias=bias)
            module.add_module("conv_{0}".format(index), conv)

            #Add the Batch Norm Layer
            if batch_normalize:
                bn = nn.BatchNorm2d(filters)
                module.add_module("batch_norm_{0}".format(index), bn)

            #Check the activation. 
            #It is either Linear or a Leaky ReLU for YOLO
            if activation == "leaky":
                activn = nn.LeakyReLU(0.1, inplace = True)
                module.add_module("leaky_{0}".format(index), activn)

            # prev_filters = filters
        
         #If it's an maxpooling layer
        elif block["type"] == "maxpool":
            kernel_size = int(block["size"])
            stride = int(block["stride"])
            if kernel_size == 2 and stride == 1:
                module.add_module(f"_debug_padding_{index}", nn.ZeroPad2d((0, 1, 0, 1)))  # nn.ZeroPad2D((left, right, top, bottom))
            maxpool = nn.MaxPool2d(kernel_size=kernel_size, stride=stride,
                                   padding=int((kernel_size - 1) // 2))
            module.add_module(f"maxpool_{index}", maxpool)
            
        #If it's an upsampling layer (for expanding the height and width of input)
        elif (block["type"] == "upsample"):
            stride = int(block["stride"])

            # For NearestSampling
            upsample = Upsample(scale_factor=int(block["stride"]), mode="nearest")
            # For Bilinear2dUpsampling
            # upsample = Upsample(scale_factor=int(block["stride"]), mode="'bilinear", align_corners=False)
            module.add_module("upsample_{}".format(index), upsample)

        #shortcut corresponds to skip connection
        elif block["type"] == "shortcut":
            filters = output_filters[int(block["from"])]
            shortcut = EmptyLayer()
            module.add_module("shortcut_{}".format(index), shortcut)
            
        #If it is a route layer
        elif (block["type"] == "route"):
            block["layers"] = block["layers"].split(',')
            #Start  of a route
            start = int(block["layers"][0])
            #end, if there exists one.
            try:
                end = int(block["layers"][1])
            except:
                end = 0
            #Positive anotation
            if start > 0: 
                start = start - index
            if end > 0:
                end = end - index
            
            if end == 0:
                # if -1 given and present layer is 20. 
                # we have to sum filters in 19th layer
                filters= output_filters[index + start]
            else:
                # If we are concatenating maps
                # suppose we have -1,8 and present layer is 20. 
                # we have to sum filters in 19th and 8th layer
                filters = output_filters[index + start] + output_filters[index + end]
            
            route = EmptyLayer()  # that means this layer don't have any forward operation
            module.add_module("route_{0}".format(index), route)
            
            # prev_filters = filters
        
        #Yolo is the detection layer
        elif block["type"] == "yolo":
            mask = block["mask"].split(",")
            mask = [int(x) for x in mask]

            anchors = block["anchors"].split(",")
            anchors = [int(a) for a in anchors]
            anchors = [(anchors[i], anchors[i+1]) for i in range(0, len(anchors),2)]
            anchors = [anchors[i] for i in mask]
            
            num_classes = int(block['classes'])
            yolo_layer = YOLOLayer(anchors, num_classes)
            module.add_module("Detection_{}".format(index), yolo_layer)

        module_list.append(module)
        prev_filters = filters
        output_filters.append(prev_filters)

    return (hyperparameters, module_list)


class Upsample(nn.Module):
    """ Upsample Layer """

    def __init__(self, scale_factor, mode="nearest"):
        super(Upsample, self).__init__()
        self.scale_factor = scale_factor
        self.mode = mode

    def forward(self, x):
        x = nn.functional.interpolate(x, scale_factor=self.scale_factor, mode=self.mode)
        return x
    

class EmptyLayer(nn.Module):      # For Route and Shortcut Layer
    def __init__(self):
        super(EmptyLayer, self).__init__()


class YOLOLayer(nn.Module):
    """Detection layer"""

    def __init__(self, anchors, num_classes):
        super(YOLOLayer, self).__init__()
        self.num_anchors = len(anchors)  # 3
        self.num_classes = num_classes
        self.num_outputs = num_classes + 5  # number of outputs per anchor ( 5 + 80 )
        self.grid = torch.zeros(1)
        
        anchors = torch.tensor(list(chain(*anchors))).float().view(-1, 2)
        self.register_buffer('anchors', anchors)  # to save it on state_dict coz it does not need to optimized
        self.register_buffer(
            'anchor_grid', anchors.clone().view(1, -1, 1, 1, 2))
        
    def forward(self, x, inp_dim):
        """
        This function will take input_dimension_of_image,anchors and number of classes as input 
        """
        # x --> 4D feature map
        batch_size = x.shape[0]
        nx, ny = x.shape[2:4]
        stride =  inp_dim // x.size(2)   # factor by which current feature map reduced from input
        
        x_y_offset = self.make_grid(nx, ny).to(x.device)
        
        # Lets say shape of the Tensor is [1, 255, 13, 13]
        prediction = x.view(batch_size, self.num_anchors, self.num_outputs, ny, nx).permute(0, 1, 3, 4, 2).contiguous()
        
        #Sigmoid the  centre_X, centre_Y. and object confidencce
        prediction[..., 0:2] = (prediction[..., 0:2].sigmoid() + x_y_offset) * stride  # predictions
        prediction[..., 2:4] = torch.exp(prediction[..., 2:4]) * self.anchor_grid # wh
        prediction[..., 4:] = prediction[..., 4:].sigmoid()
        prediction = prediction.view(batch_size, -1, self.num_outputs)
        
        return prediction
    
    @staticmethod
    def make_grid(nx=20, ny=20):
        yv, xv = torch.meshgrid([torch.arange(ny), torch.arange(nx)])
        return torch.stack((xv, yv), 2).view((1, 1, ny, nx, 2)).float()
        
        
class ObjectDetection(nn.Module):
    def __init__(self, cfgfile, CUDA):
        super(ObjectDetection, self).__init__()
        self.blocks = create_block_from_cfgfile(cfgfile)
        self.hyperparameters, self.module_list = create_modules(self.blocks)
        self.CUDA = CUDA

    def get_blocks(self):
        return self.blocks
    
    def get_module_list(self):
        return self.module_list
     
    def forward(self, x):
        modules = self.blocks[1:]
        outputs = {}   # We cache the outputs for the route layer
        first = True   # This is explained a bit later
        
        CUDA = self.CUDA
        if CUDA:
            x = x.cuda()

        for i, module in enumerate(modules):        
            module_type = (module["type"])

            if module_type == "convolutional" or module_type == "upsample" or module_type == "maxpool":
                x = self.module_list[i](x)

            elif  module_type == "shortcut":
                # adding outputs for residual network
                x = outputs[i-1] + outputs[i+int(module["from"])]

            elif module_type == "route":
                layers = module["layers"]
                layers = [int(a) for a in layers]

                if (layers[0]) > 0:
                    layers[0] = layers[0] - i

                if len(layers) == 1:
                    x = outputs[i + (layers[0])]

                else:
                    if (layers[1]) > 0:
                        layers[1] = layers[1] - i
                        
                    map1 = outputs[i + layers[0]]
                    map2 = outputs[i + layers[1]]
                    
                    x = torch.cat((map1, map2), 1)
            
            elif module_type == 'yolo':        

                anchors = self.module_list[i][0].anchors
                #Get the input dimensions
                inp_dim = int(self.hyperparameters["height"])

                #Get the number of classes
                num_classes = int(module["classes"])

                #Transform 
                # x = x.data
                x = self.module_list[i][0](x,inp_dim)  # output from yolo layer
                if first:              #if no collector has been intialised. 
                    detections = x
                    first = False

                else:       
                    detections = torch.cat((detections, x), 1)

            outputs[i] = x
                   
        try:
            return detections
        except:
            return 0
    
    
    # The weights belong to only two types of layers, either a batch norm layer or a convolutional layer.
    # The weights are stored in exactly the same order as in configuration file.
    # If the convolution layer contains batch normalization, there we will be no bias value for convolution.Only weights value will be there for such convolution layers.
    # If the convolution layer does not contain batch normalization there will be both bias and weight value.
    def load_weights(self, weightfile):
        #Open the weights file
        fp = open(weightfile, "rb")

        #The first 5 values are header information 
        # 1. Major version number
        # 2. Minor Version Number
        # 3. Subversion number 
        # 4,5. Images seen by the network (during training)
        header = np.fromfile(fp, dtype = np.int32, count = 5)
        self.header = torch.from_numpy(header)
        self.seen = self.header[3]
        weights = np.fromfile(fp, dtype = np.float32)
        ptr = 0
        for i in range(len(self.module_list)):
            module_type = self.blocks[i + 1]["type"]

            #If module_type is convolutional load weights
            #Otherwise ignore.
            if module_type == "convolutional":
                model = self.module_list[i]
                try:
                    batch_normalize = int(self.blocks[i+1]["batch_normalize"])
                except:
                    batch_normalize = 0

                conv = model[0]
                if (batch_normalize):
                    bn = model[1]

                    #Get the number of weights of Batch Norm Layer
                    num_bn_biases = bn.bias.numel()

                    #Load the weights
                    bn_biases = torch.from_numpy(weights[ptr:ptr + num_bn_biases])
                    ptr += num_bn_biases
    
                    bn_weights = torch.from_numpy(weights[ptr: ptr + num_bn_biases])
                    ptr  += num_bn_biases
    
                    bn_running_mean = torch.from_numpy(weights[ptr: ptr + num_bn_biases])
                    ptr  += num_bn_biases
    
                    bn_running_var = torch.from_numpy(weights[ptr: ptr + num_bn_biases])
                    ptr  += num_bn_biases
    
                    #Cast the loaded weights into dims of model weights. 
                    bn_biases = bn_biases.view_as(bn.bias.data)
                    bn_weights = bn_weights.view_as(bn.weight.data)
                    bn_running_mean = bn_running_mean.view_as(bn.running_mean)
                    bn_running_var = bn_running_var.view_as(bn.running_var)

                    #Copy the data to model
                    bn.bias.data.copy_(bn_biases)
                    bn.weight.data.copy_(bn_weights)
                    bn.running_mean.copy_(bn_running_mean)
                    bn.running_var.copy_(bn_running_var)
                else:
                    #Number of biases
                    num_biases = conv.bias.numel()
    
                    #Load the weights
                    conv_biases = torch.from_numpy(weights[ptr: ptr + num_biases])
                    ptr = ptr + num_biases
    
                    #reshape the loaded weights according to the dims of the model weights
                    conv_biases = conv_biases.view_as(conv.bias.data)
    
                    #Finally copy the data
                    conv.bias.data.copy_(conv_biases)
                
                #Let us load the weights for the Convolutional layers
                num_weights = conv.weight.numel()
            
                #Do the same as above for weights
                conv_weights = torch.from_numpy(weights[ptr:ptr+num_weights])
                ptr = ptr + num_weights
        
                conv_weights = conv_weights.view_as(conv.weight.data)
                conv.weight.data.copy_(conv_weights)

    def save_weights(self, path, cutoff=-1):
        """
            @:param path    - path of the new weights file
            @:param cutoff  - save layers between 0 and cutoff (cutoff = -1 -> all are saved)
        """
        fp = open(path, "wb")
        self.header_info[3] = self.seen
        self.header_info.tofile(fp)

        # Iterate through layers
        for i, (module_def, module) in enumerate(zip(self.module_defs[:cutoff], self.module_list[:cutoff])):
            if module_def["type"] == "convolutional":
                conv_layer = module[0]
                # If batch norm, load bn first
                if module_def["batch_normalize"]:
                    bn_layer = module[1]
                    bn_layer.bias.data.cpu().numpy().tofile(fp)
                    bn_layer.weight.data.cpu().numpy().tofile(fp)
                    bn_layer.running_mean.data.cpu().numpy().tofile(fp)
                    bn_layer.running_var.data.cpu().numpy().tofile(fp)
                # Load conv bias
                else:
                    conv_layer.bias.data.cpu().numpy().tofile(fp)
                # Load conv weights
                conv_layer.weight.data.cpu().numpy().tofile(fp)

        fp.close()

        
def initialize_weights_normal(m):
    classname = m.__class__.__name__
    if classname.find("Conv") != -1:
        nn.init.normal_(m.weight.data, 0.0, 0.02)
    elif classname.find("BatchNorm2d") != -1:
        nn.init.normal_(m.weight.data, 1.0, 0.02)
        nn.init.constant_(m.bias.data, 0.0)

        
def load_model(model_path, weights_path=None, CUDA=False):

    device = torch.device("cuda" if CUDA else "cpu")  # Select device for inference
    model = ObjectDetection(model_path, CUDA)

    model.apply(initialize_weights_normal)

    # If pretrained weights are specified, start from checkpoint or weight file
    if weights_path:
        if weights_path.endswith(".pth"):
            # Load checkpoint weights
            model.load_state_dict(torch.load(weights_path, map_location=device))
        else:
            # Load darknet weights
            model.load_weights(weights_path)
    return model