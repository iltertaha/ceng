import numpy as np
import matplotlib.pyplot as plt

def euclidean_dist(row1,row2):
    # convert to np array to avoid possible error
    # no problem if already np array
    item1 = np.array(row1)
    item2 = np.array(row2)

     
    # calc euclidean distance of two feature vector
    dist = np.linalg.norm(item1-item2)
    
    
    return dist



def split_data(fold_num,raw_data):
    
    raw_copy=list(raw_data)
    
    raw_copy_len = len(raw_copy)
    
    folds = []
    
    fold_size = int(len(raw_data)/fold_num)
    
    for i in range(fold_num):
        chunk = []
        
        chunk_size = len(chunk)
        
        while(chunk_size < fold_size):
            
            selected = raw_copy.pop(0)
            
            chunk.append(selected)
            
            # update variables
            raw_copy_len = len(raw_copy)
            chunk_size = len(chunk)
    
        folds.append(chunk)
        
    return folds


def plot_k_and_acc(accuracies):
    x_axis_part = []
    y_axis_part = []
    for a in accuracies:
        x_axis_part.append(a[0])
        y_axis_part.append(a[1])
        
    fig,ax = plt.subplots(1)

    # create some x data and some integers for the y axis
    x = list(range(1,200,2))
    y = y_axis_part
    
    # plot the data
    ax.plot(x,y)

    
    plt.xlabel("K values for KNN")
    plt.ylabel("Average Accuracies")
    
    
    plt.savefig("./K_vs_average_accuracy_plot.png")
    #plt.show()