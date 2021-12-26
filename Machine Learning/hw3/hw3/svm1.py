import sklearn
import numpy as np
from sklearn import svm
from draw import draw_svm



train_data = np.load("hw3_data/linsep/train_data.npy") 
train_labels = np.load("hw3_data/linsep/train_labels.npy") 


# init classifier
# cls = svm.SVC(C=1,kernel="linear")
# cls.fit(train_data,train_labels)

# draw_svm(cls,train_data,train_labels,-10,10,-10,10)


c_values = [0.01, 0.1, 1, 10, 100]

for c_val in c_values:
    cls = svm.SVC(C=c_val,kernel = "linear")
    cls.fit(train_data,train_labels)
    draw_svm(cls,train_data,train_labels,-5,5,-5,5,"svm1_cvalue"+str(c_val)+".png")
    
