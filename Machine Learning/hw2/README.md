## CENG 499 THE 2

This work belongs to Ilter Taha Aktolga
Student id : 2236891  

(Prefer editor with markdown highlighting to read this file)

## USED LIBRARIES
- numpy
- matplotlib.pylot
- random
- copy
- sys 


## How to use?
 - There are three main files namely knn,kmeans and hac, and their helpers in the folder.  
 
 1. Extract  hw2.zip
 2. Make sure that hw2_data is under the hw2 directory  
 (You need hw2_data folder. This folder should have the same hierarchy as given in the homework)
 	- To run **knn**:
       - `python3 knn.py`
	 - To run **kmeans**:
	    -  If you** want to see only the plots of elbow method**: run this command `python3 kmeans.py elbow`
		- In kmeans,  I have used the objective function in the lecture notes:
            >The k-means objective function is the sum of the squared distances of
           each point to each assigned mean.  
		- Notice that there is 1/2 coefficient.
		
		- If you **want to see only the final visualization** of clusters:run this command `python3 kmeans.py result`
	 - To run **hac**:
	 	- `python3 hac.py`
		- It will print necessary information while merging clusters
		- It will test all 4 dataset with all combinations of linkage types.
		- Tested results will be plotted and saved under the directory.

**Note: Elbow method plotting takes too much time since every k is averaged after 10 restart. ** 

Plots are saved as a .png  under current directory with the necessary information to distinguish them.
