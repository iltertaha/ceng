import numpy as np
import sys
import matplotlib.pyplot as plt

data1 = np.load("./hw2_data/hac/data1.npy")
data2 = np.load("./hw2_data/hac/data2.npy")
data3 = np.load("./hw2_data/hac/data3.npy")
data4 = np.load("./hw2_data/hac/data4.npy")

def plot_final(cluster_list,counter,linktype):
    colors = ["#4C72B0", "#DD8452", "#55A868", "#C44E52", "#8172B3","#937860", "#DA8BC3", "#8C8C8C", "#CCB974", "#64B5CD"]
    
    
    
    fig,ax = plt.subplots()
    for i,cluster in enumerate(cluster_list):
        
        x_val = []
        y_val = []
        
        for d in cluster:
            x_val.append(d[0])
            y_val.append(d[1])
        
        ax.scatter(x_val, y_val, s=7, c=colors[i], label='Cluster {}'.format(i + 1))
    
    #plt.title('k-Means Clustering\n( Iteration count = {} Objective Function value = {:.2f} )'.format((self.iterating_count + 1), np.array(self.objective_func_values)[self.iterating_count, 1]))

    plt.legend()
    plt.savefig("./hac_data_"+str(counter)+"_"+str(linktype)+"_linkage"+".png")
    #plt.show()
    
    
    
def calc_dist(cluster1,cluster2,linkage_type):
    
    
    # shortest distance
    if(linkage_type == "single"):
        min_dist = sys.maxsize
        for c1 in cluster1:
            for c2 in cluster2:
                dist = np.linalg.norm(c1-c2)
                if(dist < min_dist):
                    min_dist = dist
        return min_dist
                    
            
    # largest distance
    elif(linkage_type == "complete"):
        max_dist= -1
        for c1 in cluster1:
            for c2 in cluster2:
                dist = np.linalg.norm(c1-c2)
                if(dist > max_dist):
                    max_dist = dist
        
        return max_dist
        
    # average 
    elif(linkage_type == "average"):
        avg_dist = 0
        counter = 0
        
        for c1 in cluster1:
            for c2 in cluster2:
                dist = np.linalg.norm(c1-c2)
                avg_dist += dist
                counter+=1
        
        avg_dist = avg_dist/counter
        return avg_dist
        
    
    # distances betweeen centroids
    elif(linkage_type == "centroid"):
        centroid1x = 0
        centroid1y = 0
        
        for c1 in cluster1:
            centroid1x += c1[0]
            centroid1y += c1[1]
            
        centroid1x = centroid1x/len(cluster1)
        centroid1y = centroid1y/len(cluster1)
            
        centroid2x = 0
        centroid2y = 0
        
        for c2 in cluster2:
            centroid2x += c2[0]
            centroid2y += c2[1]
            
        centroid2x = centroid2x/len(cluster2)        
        centroid2y = centroid2y/len(cluster2)    
        
        ctr1 = np.array([centroid1x,centroid1y])
        ctr2 = np.array([centroid2x,centroid2y])
        
        dist = np.linalg.norm([ctr1-ctr2])
        return dist
        

def merge_two_list(list1,list2):
    temp = []
    for a in list1:
        temp.append(a)
    for b in list2:
        temp.append(b)
    
    return temp


def hierar_aglora(input_data,stop_when,link_type):

    cluster_list = []
    
    
    
    # make each data single cluster
    for d in input_data:
        cluster_list.append([d])
        
        
    # loop until finished
    while( len(cluster_list) > stop_when):
        
        # length of cluster_list
        cls_len = len(cluster_list)
        
        
        # find the best pair to merge 
        # [cluster1_index,cluster2_index]
        best_pair_indexes=[None,None]    
        min_distance = sys.maxsize
        
        # upper triangular loop
        for i in range(cls_len):
            for j in range(i,cls_len):
                #print(cluster_list[i][j])
                if(i != j):
                    diff = calc_dist(cluster_list[i],cluster_list[j],link_type)
                    if(diff < min_distance):
                        min_distance = diff
                        best_pair_indexes[0] = i
                        #print(best_pair_indexes[0])
                        best_pair_indexes[1] = j
                        #print(best_pair_indexes[1])
                        
                
        
        
        # merge them and add to cluster_list
        merged = merge_two_list(cluster_list[best_pair_indexes[0]],cluster_list[best_pair_indexes[1]])
    
        print("best i: ",best_pair_indexes[0])
        print("best j: ",best_pair_indexes[1])
        # remove each selected item at index at best_pair_indexes
        print("selected to merge : ",cluster_list[best_pair_indexes[0]] )
        print("selected to merge : ",cluster_list[best_pair_indexes[1]] )
        #print(len(cluster_list))
        cluster_list = [v for i, v in enumerate(cluster_list) if i not in best_pair_indexes]
        print(len(cluster_list))
        # merge the merged
        
        cluster_list.append(merged)
        print("total_length", len(cluster_list))
    return cluster_list
   
# when the loop finishes we will have only n cluster


########### main_section #####################

stop_whens = [2,2,2,4]
given_datas = [data1,data2,data3,data4]
linktypes = ["single","complete","average","centroid"]

counter = 1
for n,inp_data in enumerate(given_datas):
    for linktype in linktypes:
        cluster_list = []
        cluster_list = hierar_aglora(inp_data,stop_whens[n],linktype)
        plot_final(cluster_list,counter,linktype)
    counter+=1

#plot_final(cluster_list) 