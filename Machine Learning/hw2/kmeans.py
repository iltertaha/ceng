import numpy as np
import matplotlib.pyplot as plt
import random
import copy
from kmeans_helpers import euclidean,plot_centroids
import sys

# loaders
clustering1 = np.load('./hw2_data/kmeans/clustering1.npy')
clustering2 = np.load('./hw2_data/kmeans/clustering2.npy')
clustering3 = np.load('./hw2_data/kmeans/clustering3.npy')
clustering4 = np.load('./hw2_data/kmeans/clustering4.npy')

def start_centers(traindata,num_clusters,num_training):
    starting_centroids = []
    
    # todo
    #random.seed(42)
    
    for i in range(num_clusters):
        random_center = (random.randint(0, num_training - 1))
        starting_centroids.append(np.ravel(traindata[random_center, :]))
        
    return np.array(starting_centroids)


def cluster_it(centroids,data_x):
    num_clusters = len(centroids)
    
    prev_centers = np.zeros(centroids.shape)
    
    never_change = 0.000
    counter = 0
    objective_vals = []
    
    
    dontchange=[]
    while( euclidean(prev_centers,centroids) > never_change ):
        clusters = np.zeros(len(data_x))    
        
        
        # assign data points to clusters
        
        for i in range(len(data_x)):
            dists = []
            for k in range(len(centroids)):
                measured = euclidean(data_x[i, :], centroids[k])
                dists.append(measured)
            
            # assign data to minimum cluster
            cluster = np.argmin(dists)
            
            # but if the cluster is empty dont assign new data
            if(cluster not in dontchange):
                clusters[i] = cluster
            
        
        prev_centers = copy.deepcopy(centroids)
        
        # calculate new centers
        
        for i in range(num_clusters):
            
            new_cluster_points= []
            for j in range(len(data_x)):
                if((i not in dontchange) and (clusters[j]==i)):
                    new_cluster_points.append(data_x[j,:])
                    
            
            
            # check if there are points in cluster
            if(len(new_cluster_points)):
                # calculate new mean
                centroids[i] = np.mean(new_cluster_points,axis=0)
            else:
                # if no point assigned to cluster
                # mark cluster to not change in following iterations
                if(i not in dontchange):
                    dontchange.append(i)
        
        
        objective_vals.append([counter,calculate_objective_function(centroids,dontchange,clusters,num_clusters,data_x)])
        counter+=1
    #plot_centroids(centroids,clusters,data_x)
    return objective_vals[-1][-1],centroids,clusters




def calculate_objective_function(centroids,dont_count,clusters,num_clusters,data_x):
    
    #distances to clusters
    far_from_cntr = []
    
    # for each cluster
    for i in range(num_clusters):
        # if the cluster is valid
        if(i not in dont_count):
            # calculate the valid points to valid cluster
            measured = []
            for j in range(len(data_x)):
                if(clusters[j] == i):
                    measured.append(data_x[j, :])
            measured = np.array(measured)
            
            # if there are points(extra control)
            if(len(measured)):
                # 
                for k in range(len(measured)):
                    far_from_cntr.append((euclidean(measured[k, :], centroids[i]))**2)
    
    # refer to lecture material objective function (added 1/2)
    return (1/2)*sum(far_from_cntr)
            

def plot_elbow_method(elbow_list,counter):
    x_axis_part = []
    y_axis_part = []
    for e in elbow_list:
        x_axis_part.append(e[0])
        y_axis_part.append(e[1])
        
    fig,ax = plt.subplots(1)

    # create some x data and some integers for the y axis
    x = x_axis_part
    y = y_axis_part
    
    # plot the data
    ax.plot(x,y)

    
    plt.xlabel("Value of K")
    plt.ylabel("Averaged final objective function")
    
    
    plt.savefig("./kmeans_k_vs_obj_"+"cluster"+str(counter)+".png")
    #plt.show()
    


    
### main section

if(str(sys.argv[1]) == "elbow"):    
    counter = 1
    for X in [clustering1,clustering2,clustering3,clustering4]:#,clustering2,clustering3,clustering4]:
          #X = clustering1
          print("elbow method for cluster "+ str(counter))
          num_of_training_examples = X.shape[0]
          features_per_example = X.shape[1]
       
          elbow_list = []
          for k in range(1,11):
              averages = []
              for i in range(10):    
                  starting_points = start_centers(X,k,num_of_training_examples)
                  obj1,_,_ = cluster_it(starting_points,X)             
                  #print(obj1)
                  averages.append(obj1)
                
                  print(k,"loop restart",i)
           
              avg_val =sum(averages)/len(averages)
              elbow_list.append((k,avg_val))
      
          plot_elbow_method(elbow_list,counter)
          counter += 1
    
    

######################## PLOTS FOR SELECTED K VALUES ####################3

if(str(sys.argv[1]) == "result"):
    ##### CLUSTER 1 #####
    counter=1
    k = 2
    num_of_training_examples = clustering1.shape[0]
    starter = start_centers(clustering1,k,num_of_training_examples)
    obj1,centeroids,clusters = cluster_it(starter,clustering1)
    plot_centroids(centeroids,clusters,clustering1,counter)
    
    
    
    ##### CLUSTER 2 #####
    counter=2
    k = 3
    num_of_training_examples = clustering2.shape[0]
    starter = start_centers(clustering2,k,num_of_training_examples)
    obj1,centeroids,clusters = cluster_it(starter,clustering2)
    plot_centroids(centeroids,clusters,clustering2,counter)
    
    
    ##### CLUSTER 3 #####
    counter=3
    k = 4
    num_of_training_examples = clustering3.shape[0]
    starter = start_centers(clustering3,k,num_of_training_examples)
    obj1,centeroids,clusters = cluster_it(starter,clustering3)
    plot_centroids(centeroids,clusters,clustering3,counter)
    
    
    ##### CLUSTER 4 #####
    counter=4
    k = 5
    num_of_training_examples = clustering4.shape[0]
    starter = start_centers(clustering4,k,num_of_training_examples)
    obj1,centeroids,clusters = cluster_it(starter,clustering4)
    plot_centroids(centeroids,clusters,clustering4,counter)


    