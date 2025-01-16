import torch
import numpy as np
from collections import Counter


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

            # if len(detections) > len(truths):
            #     FP[detection_idx] = 1

        if len(detections) == 0 and len(truths) == 0:
            continue

        # print('TP:', TP)
        # print('FP:', FP)
        TP_cumsum = torch.cumsum(TP, dim=0)
        FP_cumsum = torch.cumsum(FP, dim=0)
        recalls = TP_cumsum / (len(truths) + 1e-6)
        precisions = torch.divide(TP_cumsum, (TP_cumsum + FP_cumsum + 1e-6))
        
        precisions = torch.cat([torch.tensor([1]), precisions])
        recalls = torch.cat([torch.tensor([0]), recalls])
        # print('precisions:', precisions)
        # print('recalls:', recalls)
        # calculates the area between precisions and recalls curve
        average_precisions.append(torch.trapz(precisions, recalls))
    print(average_precisions)
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
    
    return torch.abs(iou)

def mAP(pred_boxes, true_boxes, start=0.50, end=1.0, step=0.05, img_size=608, num_classes=80):
    predictions = []
    for j in range(len(pred_boxes)):
        for i in pred_boxes[j][...,:]:
            predictions.append(i)

    mean_AP = []
    for iou in np.arange(start, end, step):
    # for iou in [0.75]:
        ap = AP(predictions, true_boxes, img_size=img_size, iou_thres=float(iou), num_classes=80)
        mean_AP.append(ap)
    
    print(mean_AP)
    return sum(mean_AP) / len(mean_AP)

pred = torch.load('pred.pt')
truth2 = torch.load('truth2.pt')


a = mAP(pred, truth2)
print('mAP-------:', a)

# print(pred)
