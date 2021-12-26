import sklearn
import numpy as np
from sklearn import svm
from draw import draw_svm


train_data = np.load("hw3_data/nonlinsep/train_data.npy")
train_labels = np.load("hw3_data/nonlinsep/train_labels.npy")

#c_values = [0.01, 0.1, 1, 10, 100]
kernels = ["linear", "poly", "rbf", "sigmoid"]


for kerne in kernels:
        cls = svm.SVC(kernel = kerne)
        cls.fit(train_data,train_labels)
        # note that I know how to calculate minimum and maximum values for drawing, however
        # after testing with several numeric values, I have decided to go with 5 since there is no strict statement 
        # to use dataset's minimum maximum values.
        draw_svm(cls,train_data,train_labels,-5,5,-5,5,"./svm2_"+"kernel_"+str(kerne)+".png")