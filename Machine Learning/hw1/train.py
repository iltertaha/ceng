import torch
import torchvision.transforms as T
from dataset import MnistDataset,TestDataset
from model import one_layer_net,two_layer_net,three_layer_net
from torch.utils.data import Dataset,DataLoader,random_split
from torch import Generator
import torch.nn.functional as F
import matplotlib.pyplot as plt
import numpy as np

def train(model,optimizer,dataloaders,criterion,epochs,device,acti,hid,lrn,layercnt):
   
    steps = 0
    running_loss = 0
    print_every = 125
    train_losses, valid_losses = [], []
    valid_acc = []

    finished_epoch = -1
    early_stopped = False

    for epoch in range(epochs):
        current_valid_loss = -1
        for inputs, labels in dataloaders["train"]:
            steps += 1
            inputs, labels = inputs.to(device), labels.to(device)
            optimizer.zero_grad()
            logps = model.forward(inputs)
            loss = criterion(logps, labels)
            loss.backward()
            optimizer.step()
            running_loss += loss.item()
            
            if steps % print_every == 0:
                valid_loss = 0
                #accuracy = 0
                model.eval()
                with torch.no_grad():
                    for inputs, labels in dataloaders["val"]:
                        inputs, labels = inputs.to(device),labels.to(device)
                        logps = model.forward(inputs)
                        batch_loss = criterion(logps, labels)
                        valid_loss += batch_loss.item()

                        
                        #ps = torch.exp(logps)
                        #top_p, top_class = ps.topk(1, dim=1)
                        #equals = top_class == labels.view(*top_class.shape)
                        #accuracy += torch.mean(equals.type(torch.FloatTensor)).item()
                train_losses.append(running_loss/print_every)
                valid_losses.append(valid_loss/len(dataloaders["val"]))   
                valid_acc.append(valid_accu(model,64,dataloaders,device))                
                print("Epoch: ", epoch+1,str("/"),epochs )
                print("Train loss: ", running_loss/print_every)
                print("Valid loss: ", valid_loss/len(dataloaders["val"]))
                print("Valid accuracy: ", valid_accu(model,64,dataloaders,device))#accuracy/len(dataloaders["val"]))
                running_loss = 0
                model.train()

        ## check if epoch passed 20
        if(epoch > 20):
            last_twenty_loss = valid_losses[-21:]
            stop = True
            for i in range(len(last_twenty_loss)-1):
                if(last_twenty_loss[i]> last_twenty_loss[-1]):
                    stop = False

            if(stop == True):
                print("early stopped at: epoch",epoch)
                finished_epoch = epoch
                early_stopped = True
                break

        
    if(early_stopped == False):
        finished_epoch = 100 
    
    plot_train_valid(train_losses,valid_losses,valid_acc,finished_epoch,acti,hid,lrn,layercnt)
    
    #test(model,optimizer,dataloaders,device)

    print("--------final accuracy:  ",valid_accu(model,64,dataloaders,device))
    #activation,hidden,learning
    torch.save(model.state_dict(),str('./savedmodels/e_'+str(finished_epoch)+'_acc_'+str(acti)+"_hid_"+str(hid)+"_lr_"+str(lrn)+"_"+str(layercnt)+"layer"))
    
    # valid loss , train loss, epoch,accuraccy, act,hidden,lr
    return [valid_losses[-1],train_losses[-1],finished_epoch,valid_acc[-1],acti,hid,lrn,layercnt]

    

def validation(model,optimizer,dataloaders,device):
    
    #model.load_state_dict(torch.load('model_state_dict'))
    model.eval()
    
    
    valid_loss = 0
    accuracy = 0

    
    
    with torch.no_grad():
        for inputs,labels in dataloaders["val"]:
            
            inputs,labels = inputs.to(device),labels.to(device)
            logps = model.forward(inputs)
            batch_loss = F.nll_loss(logps,labels)
            valid_loss += batch_loss.item()

            ps = torch.exp(logps)
            top_p,top_class = ps.topk(1,dim=1)
            equals = (top_class == labels.view(*top_class.shape))
            accuracy+= torch.mean(equals.type(torch.FloatTensor)).item()

        
    #print("this ",valid_loss/len(dataloaders["val"]))
    return valid_loss/len(dataloaders["val"])
    

def test(model,optimizer,dataloaders,device):
    model.eval()
    
    with open("inference_results.txt", "w") as file:
        
        with torch.no_grad():
            for data in dataloaders["test"]:
                #images= data[0]
                #image_labels = data[1]
                #print(data)
                outputs = model(data[0])
                _,predicted = torch.max(outputs.data,1)
                #print(predicted)
                for i in range(len(data[1])):
                    #print("file: ",data[1][i]," label: ",predicted[i].item())
                    im_name = data[1][i].split('/')
                    #print("im_name ",im_name)
                    im_name = im_name[-1]
                    file.write(str(im_name+" "+str(predicted[i].item())+"\n"))





def plot_train_valid(train_losses,valid_losses,valid_acc,epoch,acti,hid,lrn,layercnt):
    plt.clf()
    plt.plot(train_losses, label='Training loss')
    plt.plot(valid_losses, label='Validation loss')
    #plt.plot(valid_acc, label='Validation acc')
    plt.xlabel("Epochs")
    plt.ylabel("Loss")
    plt.legend(frameon=False)
    
    plt.savefig(str('./plots/e_'+str(epoch)+'_acc_'+str(acti)+"_hid_"+str(hid)+"_lr_"+str(lrn)+"_"+str(layercnt)+"layer"+".png"))
    #plt.show()


def valid_accu(model,batch_size,dataloaders,device):
    valid_loader = dataloaders["val"]

    correct_count = 0.0

    
    for i, data in enumerate(valid_loader, 0):
        
        inputs, labels = data
        

        #for label in labels:
        #    print(">"+str(label.item()))
        inputs = inputs.to(device)
        labels = labels.to(device)

        outs = model(inputs)
        _, pred = torch.max(outs, -1)
        correct_count += (pred == labels).double().sum().item()
        #print("correct count : ")
        #print(correct_count)
    

    return correct_count/len(valid_loader.dataset)




def main():
    use_cuda = False
    device = torch.device('cuda' if use_cuda else 'cpu')
    epochs = 100
    torch.manual_seed(1234)
    

    # Dataset initialization
    transforms = T.Compose([
        T.ToTensor(),
        T.Normalize((0.5,),((0.5,))),
    ])

    dataset = MnistDataset('./data','train',transforms)

    total_cnt = len(dataset)
    valid_cnt = int(0.2*total_cnt)
    train_cnt = total_cnt - valid_cnt

    train_dataset, valid_dataset = random_split(dataset, (train_cnt, valid_cnt),generator = Generator().manual_seed(1234))
    
    test_dataset =  TestDataset("./data","test",transforms)

    train_dataset_loader = DataLoader(train_dataset, batch_size=64,shuffle=False,num_workers=4)  
    valid_dataset_loader = DataLoader(valid_dataset, batch_size=64,shuffle=False,num_workers=4)
    
    # cnt = 0
    # for inputs,labels in valid_dataset_loader:
    #     for k in inputs:
    #         print(k)
    #         cnt+=1
    # print("cnt ",cnt)
    

    # cnt = 0
    # for image_path,labels in valid_dataset_loader.dataset:
    #     print(image_path) 
    #     cnt+=1
    # print("cnt2 ",cnt)
    # exit()

    test_dataset_loader = DataLoader(test_dataset, batch_size=64,shuffle=False,num_workers=4)

    
    dataloaders = {'train': train_dataset_loader, 'val': valid_dataset_loader,'test':test_dataset_loader}



    

    # optimizers

    # set learning rate etc 
    
    # GRID SEARCH LOOP

    hiddens = [256,512,1024]
    learnings = [0.01,0.003,0.001,0.0003,0.0001,0.00003]
    learnings_for_one_layer = [0.05,0.01,0.005,0.003,0.001,0.0005,0.0003,0.0001,0.00003,0.00005]
    activates = ["sigmoid","tanh","relu"]
 
  # best_model variable stores (train loss, epoch,accuraccy, act,hidden,lr,layercnt)
    best_model = [None,None,None,None,None,None,None]
    best_valid_loss = float("inf")

    ## ONE LAYER NETWORK ###
    print("############ ONE LAYER SEARCH STARTS ############")
   

    
    for learning in learnings_for_one_layer:
        print("\n activation: NO   hidden_layer: ",0," learning rate: ",learning)
        print("--------------------------------")
        model = one_layer_net()
        model = model.to(device)
        optimizer = torch.optim.Adam(model.parameters(),lr=learning)
        crt = torch.nn.NLLLoss()
        # valid loss , train loss, epoch,accuraccy, act,hidden,lr,layercnt
        current = train(model,optimizer,dataloaders,crt,epochs,device,"none",0,learning,1)
        if(current[0] < best_valid_loss):
            best_valid_loss = current[0]

            best_model[0] = current[1]
            best_model[1] = current[2]
            best_model[2] = current[3]
            best_model[3] = current[4]
            best_model[4] = current[5]
            best_model[5] = current[6]
            best_model[6] = current[7]



    print("############ ONE LAYER SEARCH ENDS ############")
    print("############ TWO LAYER SEARCH STARTS ############")

    ### TWO LAYER NETWORK ###
    for activation in activates:
        for hidden in hiddens:
            for learning in learnings:
                print("\n activation:",activation,"hidden_layer: ",hidden," learning rate: ",learning)
                print("--------------------------------")
                model = two_layer_net(activation,hidden)
                model = model.to(device)
                optimizer = torch.optim.Adam(model.parameters(),lr=learning)
                crt = torch.nn.NLLLoss()
                current = train(model,optimizer,dataloaders,crt,epochs,device,activation,hidden,learning,2)
                if(current[0] < best_valid_loss):
                    best_valid_loss = current[0]

                    best_model[0] = current[1]
                    best_model[1] = current[2]
                    best_model[2] = current[3]
                    best_model[3] = current[4]
                    best_model[4] = current[5]
                    best_model[5] = current[6]
                    best_model[6] = current[7]
    
    print("############ TWO LAYER SEARCH ENDS ############")
    print("############ THREE LAYER SEARCH STARTS ############")


    ### THREE LAYER NETWORK ###
    for activation in activates:
        for hidden in hiddens:
            for learning in learnings:
                print("\n activation:",activation,"hidden_layer: ",hidden," learning rate: ",learning)
                print("--------------------------------")
                model = three_layer_net(activation,hidden)
                model = model.to(device)
                optimizer = torch.optim.Adam(model.parameters(),lr=learning)
                crt = torch.nn.NLLLoss()
                current = train(model,optimizer,dataloaders,crt,epochs,device,activation,hidden,learning,3)
                if(current[0] < best_valid_loss):
                    best_valid_loss = current[0]

                    best_model[0] = current[1]
                    best_model[1] = current[2]
                    best_model[2] = current[3]
                    best_model[3] = current[4]
                    best_model[4] = current[5]
                    best_model[5] = current[6]
                    best_model[6] = current[7]
    
    
    print("############ THREE LAYER SEARCH ENDS ############")
    
    print("BEST RESULT with VALID LOSS : ",best_valid_loss)
    print("Train Loss:",str(best_model[0]))
    print("Trained Epoch:",str(best_model[1]))
    print("Validation Accuracy:",str(best_model[2]))
    print("Activation Function:",str(best_model[3]))
    print("Hidden Layer Size:",str(best_model[4]))
    print("Learning Rate:",str(best_model[5]))
    print("Layer count:",str(best_model[6]))



    # EVALUATION ON TEST DATA
    if(best_model[6]==1):
        model = one_layer_net()
    elif(best_model[6] ==2):
        model = two_layer_net(best_model[3],best_model[4])
    elif(best_model[6]==3):
        model = three_layer_net(best_model[3],best_model[4])

    model.load_state_dict(torch.load(str('./savedmodels/e_'+str(best_model[1])+'_acc_'+str(best_model[3])+"_hid_"+str(best_model[4])+"_lr_"+str(best_model[5])+"_"+str(best_model[6])+"layer")))
    

    
    model.eval()
    print("Running inference on Test Data")
    test(model,optimizer,dataloaders,device)
    print("Saved inference_result.txt")
    

    #train_model(model,dataloaders,crt,optimizer,25,device)


if __name__ == '__main__':
    main()