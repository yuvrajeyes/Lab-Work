import os
import cv2
import time
import torch
import argparse
import numpy as np
from PIL import Image
import matplotlib.pyplot as plt
from torch.utils.data import DataLoader, Dataset

from models import load_model
from utils import prepare_image, draw_bbox_and_save_image, Find_URL, read_url_image, show_image, show_image_by_image_path
from utils import load_classes, scaled_bbox, nms, print_info, write_summary


class Folder(Dataset):
    def __init__(self, image_path):
        self.url = Find_URL(image_path)
        if len(self.url) == 0:
            try:
                self.files = [os.path.join(os.path.realpath('.'), image_path, img) for img in os.listdir(image_path)]
            except NotADirectoryError:
                self.files = []
                self.files.append(os.path.join(os.path.realpath('.'), image_path))
            except FileNotFoundError:
                print ("No file or directory with the name {}".format(image_path))
                exit()
        
    def __getitem__(self, index):
        if len(self.url) == 0:
            img_path = self.files[index % len(self.files)]
            img = np.array(
                Image.open(img_path).convert('RGB'),
                dtype=np.uint8)
        else:
            img_path = self.url[0]
            img = read_url_image(self.url[0])

        return img_path, img

    def __len__(self):
        if len(self.url) == 0:
            return len(self.files)
        else:
            return len(self.url)

def load_data(img_path, batch_size, img_size, n_cpu=2):

    dataset = Folder(
        img_path)
    
    image_path = []
    for i in range(len(dataset)):
        image_path.append(dataset[i][0])
    
    dataloader = DataLoader(
        dataset,
        batch_size=batch_size,
        shuffle=False,
        num_workers=n_cpu,
        pin_memory=True)
    return dataloader, image_path

def detect(model_path, weights_path, image_path, classes, output_path,
                     batch_size=1, img_size=416, n_cpu=8, conf_thres=0.5, nms_thres=0.5, show=False, CUDA=True):
    """
    Detects objects on all images in specified directory or link
    
    """
    #Set up the neural network
    print("---------------------------------------------------------------------------------------------------------------------------")
    print("Loading network.....")
    model = load_model(model_path, weights_path, CUDA)
    print("Network successfully loaded")
    print("---------------------------------------------------------------------------------------------------------------------------")

    dataloader, img_path = load_data(image_path, batch_size, img_size, n_cpu)
    
    detected_image_path, outputs, original_images, times = detect_images(
        model,
        dataloader,
        img_path,
        output_path,
        img_size,
        batch_size,
        conf_thres,
        nms_thres,
        classes,
        CUDA)

    # torch.save(outputs, 'pred.pt')

    if show:  
        print('Before Detection:')
        for org_img in original_images:
            show_image(org_img)
    
        print('After Detection:')
        for det_img in detected_image_path:
            show_image_by_image_path(det_img)
        
    return times, detected_image_path


def detect_images(model, dataloader, image_path, output_path, img_size, batch_size, conf_thres, nms_thres, classes, CUDA):
    
    inp_dim = int(img_size)
    assert inp_dim % 32 == 0 
    assert inp_dim > 32

    #If there's a GPU availible, put the model on GPU
    if CUDA:
        model.cuda()

    #Set the model in evaluation model
    model.eval()

    # If the directory defined by the output_path flag (to save the detections) doesn't exist, create it.
    if not os.path.exists(output_path):
        os.makedirs(output_path)

    image_path = sorted(image_path)
    
    url = Find_URL(image_path[0])
    if len(url) == 0:
        images_array = [cv2.imread(x) for x in image_path]
    else:
        images_array = [read_url_image(url[0], mode='BGR')]

    # PyTorch Variables for images
    image_batches = list(map(prepare_image, images_array, [inp_dim for x in range(len(image_path))], [img_size for x in range(len(image_path))]))

    processed_images, original_images, original_img_dims = [], [], []
    for batch in image_batches:
        processed_images.append(batch[0])
        original_images.append(batch[1])
        original_img_dims.append(batch[2])

    im_dim_list =  torch.FloatTensor(original_img_dims).repeat(1,2)
    if CUDA:
        im_dim_list = im_dim_list.cuda()
    
    left = 0
    if (len(im_dim_list) % batch_size):
        left = 1
        
    if batch_size != 1:
        num_batches = len(image_path) // batch_size + left          
        processed_images = [torch.cat((processed_images[i*batch_size : min((i +  1)*batch_size,
                            len(processed_images))]))  for i in range(num_batches)]
    
    detected_image_path = []
    outputs = []
    start_time = time.time()
    flag = 0
    for i, processed_image in enumerate(processed_images):
        
        #If there's a GPU availible, put the model on GPU
        if CUDA:
            processed_image = processed_image.cuda()

        # Load the images
        with torch.no_grad():
            prediction = model(processed_image)
        
        nc = prediction.shape[2]
        output = nms(prediction, conf_thres=conf_thres, nms_thres = nms_thres, num_classes=nc) 
    
        if type(output) == int:
            continue
    
        try:
            output
            print('Detection Succesfull')
        except NameError:
            print ("No detections were made")
            exit()
            
        
        output[..., 0] = output[..., 0]+ i + flag
        if batch_size > 1:
            flag += 1
        
        output = scaled_bbox(output, inp_dim, im_dim_list)
    
        a = list(map(lambda x: draw_bbox_and_save_image(x, original_images, image_path, inp_dim, output_path, classes, 
                                                        (0, 0, 255)), output))
        for i, im_pth in enumerate(a):
            if im_pth not in detected_image_path:
                detected_image_path.append(im_pth)

        outputs.append(output)
    end = time.time()
    
    times = { 'start_time': start_time, 'end_time': end }

    torch.cuda.empty_cache()
    return detected_image_path, outputs, images_array, times

def main():
    
    print_info()
    parser = argparse.ArgumentParser(description='YOLO v3 Detection Module')
    parser.add_argument('-f')
    parser.add_argument("-i", "--images", type=str, default="images/", help = "Image / Directory containing images to perform detection upon")
    parser.add_argument("-o", "--output_path", type = str, default="detected_image", help="Image / Directory to store detections to")
    parser.add_argument("-b", "--batch_size", type=int, default=2, help ="Batch size",)
    parser.add_argument("-m", "--model", type=str, default="cfg/yolov3.cfg", dest='cfgfile', help="Config file")
    parser.add_argument("-w", "--weights", type=str, default="weights/yolov3.weights", dest='weightsfile', help="weightsfile")
    parser.add_argument("-c", "--classes", type=str, default='data/classes.names', help='*.names path')
    parser.add_argument("-d", "--device", default='', help='device id (i.e. (0 or 1) or (cpu or gpu)')
    parser.add_argument("--conf_thres", type=float, default=0.4, help="Object Confidence to filter predictions")
    parser.add_argument("--nms_thres", type=float, default=0.5, help="IOU(NMS) Threshhold")
    parser.add_argument('--resolution', type=int, default=416, help='resizing size of image')
    parser.add_argument("--img_size", type=int, default=608, help="Input resolution of the network. Increase to increase accuracy. Decrease to increase speed")
    parser.add_argument("--n_cpu", type=int, default=os.cpu_count(), help="Number of cpu threads to use during batch generation")
    parser.add_argument("-show", "--show", type=bool, default=True, help="Show the image")
    
    args = parser.parse_args()
    print(f"Model: {args}")

    # Extract class names from file
    classes = load_classes(args.classes)  # List of class names

    times, image_path = detect(
        args.cfgfile,
        args.weightsfile,
        image_path = args.images,
        classes = classes,
        output_path = args.output_path,
        batch_size=int(args.batch_size),
        img_size=args.img_size,
        n_cpu=int(args.n_cpu),
        conf_thres=float(args.conf_thres),
        nms_thres=float(args.nms_thres),
        show=args.show,
        CUDA = False if args.device in ['', 0, '0', 'cpu'] else True)
    
    # SUMMARY BOARD
    write_summary(times, image_path)
    
if __name__ == '__main__':
    main()