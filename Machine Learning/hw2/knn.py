import numpy as np
import matplotlib.pyplot as plt
from knn_helpers import euclidean_dist,split_data,plot_k_and_acc


# KNN with k-fold cross validation 

# try several K's 
# set k = 10 for k-fold

# for every k
    # calculate distances
    # get nearest neighbour func
    # predict
    # apply k-fold and predict

# choose best k 
# eval on test data


### loaders ###

train_data_loader = np.load("./hw2_data/knn/train_data.npy")
test_data_loader = np.load("./hw2_data/knn/test_data.npy")
train_labels_loader = np.load("./hw2_data/knn/train_labels.npy")
test_labels_loader = np.load("./hw2_data/knn/test_labels.npy")

###############


##### global vars #########

DEBUG = 0
##############




def get_nearest_neighbors(test_item,k,train_data,train_labels):
    
    # holds distances to other rows
    dists = []
    
    # closest k neighbor will be stored
    closest_neighbors = []
    
    # calculate distance for each
    for i, d in enumerate(train_data):
        dists.append([d, euclidean_dist(test_item, d), train_labels[i]])
        # inplace sort according to distance values
        dists.sort(key=lambda x: x[1])
        
    # get only needed amount of highest neighbor
    for ind in range(k):
        closest_neighbors.append(dists[ind])
        
    #print("neighbor found:",len(closest_neighbors))
    return closest_neighbors



def make_pred(k,test_data,train_data,train_labels):
    preds = []
        
    
    for i, test_item in enumerate(test_data):
        res = []
        neighs = get_nearest_neighbors(test_item, k,train_data,train_labels)
        
        for n in neighs:
            res.append(n[-1])
        
        majority_vote = np.bincount(res).argmax()
        
        preds.append(majority_vote)  

    return preds





def prepare_data_label_split(raw_data):
    
    data = []
    labels = []
    
    for d in raw_data:
        data.append(d[:-1])
        labels.append(d[-1])
    
    return data,labels



def calc_acc(predicted,val_labels):
    
    total_len = len(predicted)
    correct = 0
    
    for i in range(len(predicted)):
        
        if(predicted[i] == val_labels[i]):
            correct += 1
            
    res = correct/float(total_len)
    
    return res



def apply_cross_val(k,raw_data,foldnum,mode,eval_test_data=None):
    
    
    if(mode=="train"):
        
    
        chunks = split_data(foldnum,raw_data)
        accs= []
        for i,chunk in enumerate(chunks):
            will_trained = list(chunks)
            del will_trained[i]
            
            will_trained = [x for chn in will_trained for x in chn]
            
            
            will_tested = []
            
            for c in chunk:
                will_tested.append(list(c))
                
            train_data,train_labels = prepare_data_label_split(will_trained)
            val_data,val_labels = prepare_data_label_split(will_tested)
            
            
            pred = make_pred(k,val_data,train_data,train_labels)
            
            acc = calc_acc(pred,val_labels)
            accs.append(acc)
            
        
        mean_acc = sum(accs)/len(accs)
        
        print("mean_acc",mean_acc)
        return mean_acc
            
    elif(mode=="eval"):
        chunks = split_data(foldnum,raw_data)
        will_trained = list(chunks)
        
        will_trained = [x for chn in will_trained for x in chn]
            
        chunks2= split_data(foldnum,eval_test_data)
        will_tested = list(chunks2)
        
        will_tested = [x for chn in will_tested for x in chn]
            
        train_data,train_labels = prepare_data_label_split(will_trained)
        testt_data,testt_labels = prepare_data_label_split(will_tested)
        
        
        pred = make_pred(k,testt_data,train_data,train_labels)
        
        acc = calc_acc(pred,testt_labels)
        return acc       
    
    

    
    
        
# since we will split train set into folds, it is easy if we merge data with labels



if(DEBUG):
    print("train shape",train_data_loader.shape)
    print("train_labels",train_labels_loader.shape)
        
    print("test shape",test_data_loader.shape)
    print("test_labels",test_labels_loader.shape)



train_labels = train_labels_loader.reshape(len(train_labels_loader),1)
train_merged = np.hstack((train_data_loader,train_labels))


accuracies= []
highest_acc_index = -1
highest_acc_value = -1

print("started applying cross validation to find K for KNN")
for i in range(1,200,2):
    print("K = ",i)
    val = apply_cross_val(i,train_merged,10,"train")
    if(val > highest_acc_value):
        highest_acc_index = i
        highest_acc_value= val
    accuracies.append((i,val))
    
highest_acc_index = 11
    
print("highest k-fold cross validation accuracy: ",highest_acc_value)
print("selected highest k value: ",highest_acc_index)


    
test_labels_loader = test_labels_loader.reshape(len(test_labels_loader),1)

test_merged = np.hstack((test_data_loader,test_labels_loader))

test_acc = apply_cross_val(highest_acc_index,train_merged,10,"eval",test_merged)

# test_data_loader,test_labels_loader = prepare_data_label_split(test_merged)

# test_predictions = make_pred(highest_acc_index,test_data_loader,train_data_loader,train_labels)

# test_acc = calc_acc(test_predictions,test_labels_loader)

print("Test accuracy with", highest_acc_index,"nearest neighbor: ",test_acc)

plot_k_and_acc(accuracies)
    



#print(train_data)

#print(train_labels)


