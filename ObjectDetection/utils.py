import platform
import torch
import numpy as np
import os 
import matplotlib.pyplot as plt
import matplotlib.image as mpimg
from PIL import Image
import cv2
import re
import urllib.request
import io


def create_block_from_cfgfile(configfile):
    
    file = open(configfile, 'r').read()
    
    # store the lines in a list
    # get rid of comments and whitespaces
    lines = [x.strip() for x in [y for y in file.split('\n') if len(y) > 0] if x[0] != '#']
    block, blocks = {}, []

    for line in lines:
        if line[0] == "[":               # start of a new block
            if len(block) != 0:          # If block is not empty, it implies the storing values of previous block.
                blocks.append(block)     # add it the blocks list
                block = {}               # re-initialize the block
            block["type"] = line[1:-1].rstrip()     
        else:
            key,value = line.split("=") 
            block[key.rstrip()] = value.lstrip()
    blocks.append(block)

    return blocks


# Since there can be multiple true detections of the same class, we use
# a function called unique to get classes present in any given image.
def unique(tensor):
    tensor_np = tensor.cpu().numpy()
    unique_np = np.unique(tensor_np)
    unique_tensor = torch.from_numpy(unique_np)
    
    tensor_res = tensor.new(unique_tensor.shape)
    tensor_res.copy_(unique_tensor)
    return tensor_res

def mAP(pred_boxes, true_boxes, start=0.50, end=1.0, step=0.05, img_size=608, num_classes=80):
    predictions = []
    for j in range(len(pred_boxes)):
        for i in pred_boxes[j][...,:]:
            predictions.append(i)

    mean_AP = []
    for iou in np.arange(start, end, step):
        ap = AP(predictions, true_boxes, img_size=img_size, iou_thres=float(iou), num_classes=80)
        mean_AP.append(ap)

    return sum(mean_AP) / len(mean_AP)
    
def AP(predictions, true_boxes, img_size=608, iou_thres=0.5, num_classes=80):

    average_precisions = []
    for c in range(num_classes):
        detections = []
        truths = []

        for detection in predictions:
            if detection[-1] == c:
                detections.append(detection)
        
        for true_box in true_boxes:
                if true_box[1] == c :
                    truths.append(true_box)
        
        bboxes = Counter([truth[0] for truth in truths])
            
        for key, val in bboxes.items():
            bboxes[key] = torch.zeros(val)

        detections.sort(key=lambda x: x[-1], reverse=True)

        TP = torch.zeros((len(detections)))
        FP = torch.zeros((len(detections)))

        for detection_idx, detection in enumerate(detections):
            correct_img = [bbox for bbox in truths if bbox[0] == detection[0]]

            best_iou = 0

            for idx, cor_img in enumerate(correct_img):
                cor_img = cor_img[2:].detach().clone()
                detection_ = detection[...,1:5].detach().clone()

                iou = box_iou(detection_, cor_img, img_size)
    
                if iou > best_iou:
                    best_iou = iou
                    best_cor_img_idx = idx

            if best_iou > iou_thres:
                if bboxes[detection[best_cor_img_idx]] == 0:
                    TP[detection_idx] = 1
                    bboxes[detection[best_cor_img_idx]] = 1
                else:
                    FP[detection_idx] = 1
            else:
                FP[detection_idx] = 1

            if len(detections) > len(truths):
                FP[detection_idx] = 1

        if len(detections) == 0 and len(truths) == 0:
            continue

        TP_cumsum = torch.cumsum(TP, dim=0)
        FP_cumsum = torch.cumsum(FP, dim=0)
        recalls = TP_cumsum / (len(truths) + 1e-6)
        precisions = torch.divide(TP_cumsum, (TP_cumsum + FP_cumsum + 1e-6))
        
        precisions = torch.cat([torch.tensor([1]), precisions])
        recalls = torch.cat([torch.tensor([0]), recalls])
        
        # calculates the area between precisions and recalls curve
        average_precisions.append(torch.trapz(precisions, recalls))

    return sum(average_precisions) / len(average_precisions)
    
def box_iou(box1, box2, img_size=608):
    #Get the coordinates of bounding boxes
    b1_x1, b1_y1, b1_x2, b1_y2 = box1[0], box1[1], box1[2], box1[3]
    b2_x1, b2_y1, b2_x2, b2_y2 = box2[0] * img_size, box2[1] * img_size, box2[2] * img_size, box2[3] * img_size

    rect_x1 =  torch.max(b1_x1, b2_x1)
    rect_y1 =  torch.max(b1_y1, b2_y1)
    rect_x2 =  torch.min(b1_x2, b2_x2)
    rect_y2 =  torch.min(b1_y2, b2_y2)
    
    #Intersection area
    inter_area = (rect_x2 - rect_x1) * (rect_y2 - rect_y1)
 
    #Union Area
    b1_area = (b1_x2 - b1_x1) * (b1_y2 - b1_y1)
    b2_area = (b2_x2 - b2_x1) * (b2_y2 - b2_y1)
    
    iou = inter_area / (b1_area + b2_area - inter_area)
    
    # print('--iou--:', iou)
    return torch.abs(iou)

def bbox_iou(box1, box2):

    #Get the coordinates of bounding boxes
    b1_x1, b1_y1, b1_x2, b1_y2 = box1[:,0], box1[:,1], box1[:,2], box1[:,3]
    b2_x1, b2_y1, b2_x2, b2_y2 = box2[:,0], box2[:,1], box2[:,2], box2[:,3]    
    
    #get the corrdinates of the intersection rectangle
    inter_rect_x1 =  torch.max(b1_x1, b2_x1)
    inter_rect_y1 =  torch.max(b1_y1, b2_y1)
    inter_rect_x2 =  torch.min(b1_x2, b2_x2)
    inter_rect_y2 =  torch.min(b1_y2, b2_y2)
    
    #Intersection area
    inter_area = torch.clamp(inter_rect_x2 - inter_rect_x1 + 1, min=0) * torch.clamp(inter_rect_y2 - inter_rect_y1 + 1, min=0)
 
    #Union Area
    b1_area = (b1_x2 - b1_x1 + 1)*(b1_y2 - b1_y1 + 1)
    b2_area = (b2_x2 - b2_x1 + 1)*(b2_y2 - b2_y1 + 1)
    
    iou = inter_area / (b1_area + b2_area - inter_area)
    
    return iou

def nms(prediction, conf_thres=0.5, nms_thres = 0.4, num_classes=None):
    conf_mask = (prediction[:,:,4] > conf_thres).float().unsqueeze(2)
    prediction = prediction*conf_mask
    
    box_corner = prediction.new(prediction.shape)
    box_corner[:,:,0] = (prediction[:,:,0] - prediction[:,:,2]/2)
    box_corner[:,:,1] = (prediction[:,:,1] - prediction[:,:,3]/2)
    box_corner[:,:,2] = (prediction[:,:,0] + prediction[:,:,2]/2) 
    box_corner[:,:,3] = (prediction[:,:,1] + prediction[:,:,3]/2)
    prediction[:,:,:4] = box_corner[:,:,:4]
    
    batch_size = prediction.size(0)
    write = False

    for ind in range(batch_size):
        image_pred = prediction[ind]          #image Tensor
        
        #Get the class having maximum score, and the index of that class
        #Get rid of num_classes softmax scores 
        #Add the class index and the class score of class having maximum score
        max_conf, max_conf_score = torch.max(image_pred[:,5:5+ num_classes], 1)
        max_conf = max_conf.float().unsqueeze(1)
        max_conf_score = max_conf_score.float().unsqueeze(1)
        seq = (image_pred[:,:5], max_conf, max_conf_score)
        image_pred = torch.cat(seq, 1)
        
        #Get rid of the zero entries
        non_zero_ind =  (torch.nonzero(image_pred[:,4]))
        
        image_pred_ = image_pred[non_zero_ind.squeeze(),:].view(-1,7)
        
        #Get the various classes detected in the image
        img_classes = unique(image_pred_[:,-1])  # -1 index holds the class index
                
        #WE will do NMS classwise
        for cls in img_classes:
            #get the detections with one particular class
            cls_mask = image_pred_*(image_pred_[:,-1] == cls).float().unsqueeze(1)
            class_mask_ind = torch.nonzero(cls_mask[:,-2]).squeeze()
            image_pred_class = image_pred_[class_mask_ind].view(-1,7)
        
            #sort the detections such that the entry with the maximum objectness
            #confidence is at the top
            conf_sort_index = torch.sort(image_pred_class[:,4], descending = True )[1]
            image_pred_class = image_pred_class[conf_sort_index]
            idx = image_pred_class.size(0)  #Number of detections
            
            #For each detection
            for i in range(idx):
                #Get the IOUs of all the boxes
                try:
                    ious = bbox_iou(image_pred_class[i].unsqueeze(0), image_pred_class[i+1:])
                except ValueError:
                    break
        
                except IndexError:
                    break
                
                #Zero out all the detections that have IoU > treshhold
                iou_mask = (ious < nms_thres).float().unsqueeze(1)
                image_pred_class[i+1:] *= iou_mask       
                #Remove the non-zero entries
                non_zero_ind = torch.nonzero(image_pred_class[:,4]).squeeze()
                image_pred_class = image_pred_class[non_zero_ind].view(-1,7)                   
            
            #Concatenate the batch_id of the image to the detection
            #this helps us identify which image does the detection correspond to    
            
            batch_ind = image_pred_class.new(image_pred_class.size(0), 1).fill_(ind)
            #Repeat the batch_id for as many detections of the class cls in the image
            seq = batch_ind, image_pred_class

            if not write:
                output = torch.cat(seq,1)
                write = True
            else:
                out = torch.cat(seq,1)
                output = torch.cat((output,out))

    return output

def scaled_bbox(output, inp_dim, img_dim):
    im_dim_list = torch.index_select(img_dim, 0, output[:,0].long())

    scaling_factor = torch.min(inp_dim/im_dim_list,1)[0].view(-1,1)
    output[:,[1,3]] -= (inp_dim - scaling_factor*im_dim_list[:,0].view(-1,1))/2
    output[:,[2,4]] -= (inp_dim - scaling_factor*im_dim_list[:,1].view(-1,1))/2
    output[:,1:5] /= scaling_factor

    # adjusting bounding box size between 0 and configuration image size
    output[:,1:5] = torch.clamp(output[:,1:5], 0.0, float(inp_dim))
    return output

def load_classes(namesfile):
    fp = open(namesfile, "r")
    names = fp.read().split("\n")[:-1]
    return names


def get_image(img, inp_resolution):
    if type(img) == str:
        img = cv2.imread(img)
    img = cv2.resize(img, (inp_resolution, inp_resolution))         # Resize to the input dimension
    img_ = img[:,:,::-1].transpose((2,0,1))                         # BGR -> RGB
    img_ = img_[np.newaxis,:,:,:]/255.0                             # Add a channel at 0 (for batch) | Normalise
    img_ = torch.FloatTensor(torch.from_numpy(img_).float())        # Convert to FloatTensor                                
    return img

def prepare_image(img, inp_dim, inp_resolution):
    """
    Prepare image for inputting to the neural network. 
    
    """
    org_img = get_image(img, inp_resolution)
    new_w = int(org_img.shape[1] * min(inp_dim/org_img.shape[1], inp_dim/org_img.shape[0]))     # w, h = inp_dim, inp_dim
    new_h = int(org_img.shape[0] * min(inp_dim/org_img.shape[1], inp_dim/org_img.shape[0]))
    img = cv2.resize(org_img, (new_w,new_h), interpolation = cv2.INTER_CUBIC)
    
    img = img[:,:,::-1].transpose((2,0,1)).copy()
    img = torch.from_numpy(img).float().div(255.0).unsqueeze(0)
    return img, org_img, (org_img.shape[1], org_img.shape[0])

def draw_bbox_and_save_image(detection, image, image_path, img_size, output_path, classes, color, conf_thres=0.50):
    c1 = tuple(map(int, detection[1:3]))
    c2 = tuple(map(int, detection[3:5]))
    ind = int(detection[0])
    cls = int(detection[-1])
    confidence_score = detection[-2]
    label = f"{classes[cls]} {confidence_score*100:0.2f}"

    if confidence_score > conf_thres:
        cv2.rectangle(image[ind], c1, c2,color, 1)
        t_size = cv2.getTextSize(label, cv2.FONT_HERSHEY_PLAIN, 1 , 1)[0]
        c2 = c1[0] + t_size[0] + 3, c1[1] + t_size[1] + 4
        cv2.putText(image[ind], label, (c1[0], c1[1] + t_size[1]), cv2.FONT_HERSHEY_DUPLEX, 0.5, [0,255,0], 1)

    img, img_path = save_image(image[ind], image_path[ind], img_size, output_path)
    return img_path

def save_image(image, image_path, img_size, output_path):
    dest_path = str(os.getcwd() + "\\" + output_path + "\\")
    if platform.system() == 'Linux':
        dest_path = str(os.getcwd() + "/" + output_path + "/")
    
    path = image_path.split('\\')[-1].split('.')

    url = Find_URL(image_path)
    if len(url) > 0:
        file_name = url[0].split('//')[-1].split('.')[-2].split('/')[-1] + '.png'
    elif len(path[0].split('/')) == 1:
        file_name = str(path[0]) + '.png'
    else:
        file_name = str(path[0].split('/')[-1]) + '.png'
        
    img = cv2.resize(image, (img_size, img_size), interpolation= cv2.INTER_LINEAR)
    file_path = dest_path + file_name
    cv2.imwrite(file_path, img)
    return img, file_path

def show_image_by_image_path(image_path):
    url = Find_URL(image_path)
    if len(url) > 0:
        img = read_url_image(url[0])
    else:
        img = mpimg.imread(image_path)
    plt.imshow(img)
    plt.show()

def show_image(img, mode='RGB'):
    img = change_color_mode(img, to=mode)
    plt.imshow(img)
    plt.show()

def change_color_mode(img , to='RGB'):
    if to == 'RGB':
        try:
            img = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)
        except:
            pass
    elif to == 'BGR':
        try:
            img = cv2.cvtColor(img, cv2.COLOR_RGB2BGR)
        except:
            pass
    elif to == 'GRAY' or mode == 'GREY':
        try:
            try:
                img = cv2.cvtColor(img, cv2.COLOR_RGB2GRAY)
            except:
                img = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
        except:
            pass

    return img


def Find_URL(string):
    # with valid conditions for urls in string
    regex = r"(?i)\b((?:https?://|www\d{0,3}[.]|[a-z0-9.\-]+[.][a-z]{2,4}/)(?:[^\s()<>]+|\(([^\s()<>]+|(\([^\s()<>]+\)))*\))+(?:\(([^\s()<>]+|(\([^\s()<>]+\)))*\)|[^\s`!()\[\]{};:'\".,<>?«»“”‘’]))"
    url = re.findall(regex,string)      
    return [x[0] for x in url]


def read_url_image(url, mode='RGB'):
    fd = urllib.request.urlopen(url)
    image_file = io.BytesIO(fd.read())
    img = np.array(Image.open(image_file), dtype=np.uint8)
    if mode == 'BGR':
        img = cv2.cvtColor(img, cv2.COLOR_RGB2BGR)
    elif mode == 'GRAY' or mode == 'GREY':
        img = cv2.cvtColor(img, cv2.COLOR_RGB2GRAY)
    
    return img


def print_info():
    """
    Prints infos about the environment and the system.
    """

    print("Environment and System Information:")

    print(f"System: {platform.system()} {platform.release()}")
    print(f"Processor: {platform.processor()}")
    print(f"Python Compiler: {platform.python_compiler()}")
    print(f"Python Version: {platform.python_version()}")
    print(f"Platform Architecture: {platform.architecture()}")
    print()
    
def write_summary(times, image_path):
    print("---------------------------------------------------------------------------------------------------------------------------")
    print()
    print(f"{' ':27s} {'SUMMARY'}")
    print(' _ _ _ _ _ _ _ _ _ _ _ _  _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ ')
    print(f"{'|':16s} {'Task':20s}: {'Time Taken (in seconds)'} {'|'}")
    print('|--------------------------------------------------------------|')
    print(f"{'|'} {'Average time_per_img (' + str(len(image_path)) + ' images)':35s}: {(times['end_time'] - times['start_time'])/len(image_path):2.3f}   ( {(times['end_time'] - times['start_time']):2.3f} {')':7s} {'|'}")
    print('|_ _ _ _ _ _ _ _ _ _ _ _  _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _|')
    print("---------------------------------------------------------------------------------------------------------------------------")
    