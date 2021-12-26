
from torch.utils.data import Dataset,DataLoader,random_split
import os
from PIL import Image
import torchvision.transforms as T

from torchvision import transforms, utils, datasets, models


class MnistDataset(Dataset):
    def __init__(self,dataset_path,split,transforms):
        images_path = os.path.join(dataset_path,split)
        self.data = []
        
        
        with open(os.path.join(images_path,'labels.txt'),'r') as f:
            for line in f:
                image_name, label = line.split()
                image_path = os.path.join(images_path,image_name)
                label = int(label)
                self.data.append((image_path,label))
    
        
        self.transforms = transforms

    def __len__(self):
        return len(self.data)
    
    def __getitem__(self,index):
        image_path = self.data[index][0]
        label = self.data[index][1]
        image = Image.open(image_path)
        image = self.transforms(image)
        
        return image,label

    


class TestDataset(Dataset):
    def __init__(self,dataset_path,split,transforms):
        images_path = os.path.join(dataset_path,split)
        self.data = []
        
        
        with open(os.path.join(images_path,'labels.txt'),'r') as f:
            for line in f:
                line = [x.strip() for x in line] 
                line = ''.join(line)
                image_path = os.path.join(images_path,str(line))
                #print("image path : ",image_path)
                self.data.append((image_path))
    
        
        self.transforms = transforms

    def __len__(self):
        return len(self.data)
    
    def __getitem__(self,index):
        image_path = self.data[index]
        image = Image.open(image_path)
        image = self.transforms(image)
        #print(image_path)

        return image,image_path


            