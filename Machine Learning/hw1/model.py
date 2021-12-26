import torch.nn as nn
from dataset import MnistDataset
from torch import Generator
from torch.utils.data import Dataset,DataLoader,random_split
import torchvision.transforms as T
import torch
import torch.nn.functional as F

class one_layer_net(nn.Module):
    def __init__(self):
        super(one_layer_net,self).__init__()
        # input is 1dim, output is 100 class
        self.fc1 = nn.Linear(1*32*64,100)
        
        
    def forward(self,x):
        # flatten the tensor
        x = x.view(x.size(0),-1)
        x = self.fc1(x)
        x = torch.log_softmax(x,dim=1)
        return x
       
class two_layer_net(nn.Module):
    def __init__(self,act,layer_size):
        super(two_layer_net,self).__init__()
        # input is 1dim, output is 100 class
        
        self.activation = act
        self.hidden_size = layer_size

        self.fc1 = nn.Linear(1*32*64,self.hidden_size)
        self.fc2 = nn.Linear(self.hidden_size,100)
        
    def forward(self,x):
        # flatten the tensor
        x = x.view(x.size(0),-1)
        x = self.fc1(x)
       
        if(self.activation =="relu"):
            
            x = F.relu(x)
        elif(self.activation=="sigmoid"):
            
            x = F.sigmoid(x)
        elif(self.activation=="tanh"):
            
            x = F.tanh(x)

        x = self.fc2(x)
        x = torch.log_softmax(x,dim=1)
        return x
       

class three_layer_net(nn.Module):
    def __init__(self,act,layer_size):
        super(three_layer_net,self).__init__()
        # input is 1dim, output is 100 class
        self.hidden_size = layer_size

        self.fc1 = nn.Linear(1*32*64,self.hidden_size)
        self.fc2 = nn.Linear(self.hidden_size,self.hidden_size)
        self.fc3 = nn.Linear(self.hidden_size,100)
        self.activation = act


    def forward(self,x):
        # flatten the tensor
        x = x.view(x.size(0),-1)
        x = self.fc1(x)
        #print(x)
        
        if(self.activation =="relu"):
            
            x = F.relu(x)
        elif(self.activation=="sigmoid"):
            
            x = F.sigmoid(x)
        elif(self.activation=="tanh"):
            
            x = F.tanh(x)

        #print(x)
        x = self.fc2(x)
        
        if(self.activation =="relu"):
            
            x = F.relu(x)
        elif(self.activation=="sigmoid"):
            
            x = F.sigmoid(x)
        elif(self.activation=="tanh"):
            
            x = F.tanh(x)

        x = self.fc3(x)
        x = torch.log_softmax(x,dim=1)
        return x
       
    
        


if __name__ == '__main__':

    

    # Dataset initialization
    transforms = T.Compose([
        T.ToTensor(),
        T.Normalize((0.5,),((0.5,))),
    ])

    dataset = MnistDataset('./data','train',transforms)

    #test_dataset = MnistDataset('./data','test',transforms)
    #dataloader = DataLoader(dataset,batch_size=64,shuffle=True,num_workers=4)
    

    # Dividing training set into train and validation
    # 0.2 validation 0.8 train
    total_cnt = len(dataset)
    #print("total count: ",total_cnt)

    valid_cnt = int(0.2*total_cnt)
    train_cnt = total_cnt - valid_cnt

    # validation set creation with fixed seed 
    train_dataset, valid_dataset = random_split(dataset, (train_cnt, valid_cnt),generator = Generator().manual_seed(42))

    # DataLoaders for train and validation
    train_dataset_loader = DataLoader(train_dataset, batch_size=64,shuffle=True,num_workers=4)  
    valid_dataset_loader = DataLoader(valid_dataset, batch_size=64,shuffle=True,num_workers=4)
    #test_dataset_loader  = DataLoader(test_dataset, batch_size=64,shuffle=True,num_workers=4)
    dataloaders = {'train': train_dataset_loader, 'val': valid_dataset_loader}
        
    #print(len(dataloaders["train"].dataset))
    #print(len(dataloaders["val"].dataset))

    #for image_path,labels in dataloaders["val"].dataset:
    #    print(image_path,labels) 

    ## simple training example

    model = three_layer_net()
    for images,labels in dataloaders['train']:
        pred = model(images)
        #print(pred)
        #exit()



    # for images,labels in dataloaders["train"]:
    #     print(images.size())
    #     print(labels)




