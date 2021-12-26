import numpy as np
import matplotlib.pyplot as plt

def euclidean(v1,v2):
    return np.sqrt(np.sum((np.array(v1)- np.array(v2))**2))



def plot_centroids(centroids,clusters,data_x,counter):
    colors = ["#34568B","#FF6F61", "#88B04B", "#009B77", "#EFC050", "#9B2335"]
    
    cl = clusters
    ce = centroids
    fig,ax = plt.subplots()
    len_clus = len(clusters)
    for i in range(len(centroids)):
        points = np.array([data_x[j, :] for j in range(len(data_x)) if clusters[j] == i])
        #print(points.shape)
        a = points[:, 0]
        a  = a.reshape(len(a),1)
        #print(a.shape)
        #print(a.ndim)
        b = points[:,1]
        b  = b.reshape(len(b),1)
        #print(b.shape)
        #print(b.ndim)
        ax.scatter(a, b, s=7, c=colors[i], label='Cluster {}'.format(i + 1))
    
    ax.scatter(centroids[:, 0], centroids[:, 1], marker='D', s=120, c='#000000', label='Centroids')

    plt.legend()
    plt.savefig("./kmeans_cluster"+str(counter)+".png")
    #plt.show()