#train_data = np . load ( ’ hw3_data/ catdogimba / t rain_da ta . npy ’ )
#train_labels = np . load ( ’ hw3_data/ catdogimba / t r a i n _ l a b e l s . npy ’ )
#test_data = np . load ( ’ hw3_data/ catdogimba / te s t_da ta . npy ’ )
#test_labels = np . load ( ’ hw3_data/ catdogimba / t e s t _ l a b e l s . npy ’ )

import sklearn
import numpy as np
from sklearn import svm
from sklearn.preprocessing import MinMaxScaler
from draw import draw_svm
from sklearn.model_selection import GridSearchCV
from sklearn.metrics import classification_report,plot_confusion_matrix
import matplotlib.pyplot as plt
import copy
from sklearn.metrics import accuracy_score, f1_score, precision_score, recall_score

print("loaded data")
train_data = np.load("hw3_data/catdogimba/train_data.npy")
train_labels = np.load("hw3_data/catdogimba/train_labels.npy")
test_data = np.load("hw3_data/catdogimba/test_data.npy")
test_labels = np.load("hw3_data/catdogimba/test_labels.npy")

scaler = MinMaxScaler(feature_range = (-1,1))
normalized_train = scaler.fit(train_data)
normalized_train_data = scaler.transform(train_data)

scaler2 = MinMaxScaler(feature_range = (-1,1))
normalized_test = scaler2.fit(test_data)
normalized_test_data = scaler2.transform(test_data)

# train rbf with C= 1

cls = svm.SVC(C=1,kernel = "rbf")
cls.fit(train_data,train_labels)
predicted = cls.predict(test_data)

# get the accuracy
# report accuracy on test set
print("accuracy on the test data:",accuracy_score(test_labels, predicted))
#print("f1_score:",f1_score(test_labels, predicted))
print("precision_score:",precision_score(test_labels, predicted))
print("recall_score:",recall_score(test_labels, predicted)) 

# calculate confusion matrix with test set
plot_confusion_matrix(cls, test_data, test_labels)
#plt.show()
plt.draw()
plt.savefig('svm4_normal_confusion.png')


ones = 0
zeros = 0

for d in train_labels:
    if(d == 1):
        ones+=1
    if(d == 0):
        zeros+=1
        
print("ones:",ones)
print("zeros:",zeros)


# oversample
print("TRYING OVERSAMPLING")
oversampled_train_data = copy.deepcopy(train_data)
oversampled_train_labels = copy.deepcopy(train_labels)



cont= 0
outsider = 0
for _ in range(4):
    
    for i,d in enumerate(train_labels):
        if(d == 0):
            if(cont >   ones-zeros):
                outsider = 1
                break
                
            cont+=1
            oversampled_train_labels = np.append(oversampled_train_labels,[d])
            oversampled_train_data = np.concatenate((oversampled_train_data, [train_data[i]]), axis=0)
            #oversampled_train_data = np.append(oversampled_train_data,[train_data[i]])
        if(outsider):
            break
    
ones = 0
zeros = 0
    
for d in oversampled_train_labels:
    #print("d",d)
    if(d == 1):
        ones+=1
    if(d == 0):
        zeros+=1

print("ones after oversampling:",ones)
print("zeros after oversampling:",zeros-1)

scaler = MinMaxScaler(feature_range = (-1,1))
normalized_train = scaler.fit(oversampled_train_data)
normalized_train_data = scaler.transform(oversampled_train_data)

oversampled_train_data = normalized_train_data

cls = svm.SVC(C=1,kernel = "rbf")
cls.fit(oversampled_train_data,oversampled_train_labels)
predicted = cls.predict(normalized_test_data)

# get the accuracy
# report accuracy on test set
print("oversampled accuracy on the test data:",accuracy_score(test_labels, predicted))
#print("oversampled f1_score:",f1_score(test_labels, predicted))
print("oversampled precision_score:",precision_score(test_labels, predicted))
print("oversampled recall_score:",recall_score(test_labels, predicted)) 

# calculate confusion matrix with test set
plot_confusion_matrix(cls, normalized_test_data, test_labels)
#plt.show()
plt.draw()
plt.savefig('svm4_oversample_confusion.png')



# undersample
print("TRYING UNDERSAMPLING")




undersampled_train_data = []
undersampled_train_labels = []

counter= 0
for i,d in enumerate(train_labels):
    if(d == 1):
        if(counter < 215):
            # delete
            undersampled_train_labels.append([d])
            undersampled_train_data.append(train_data[i])
           
            
        counter+=1
    if(d == 0):
        undersampled_train_labels.append([d])
        undersampled_train_data.append(train_data[i])
    
ones = 0
zeros = 0
    
for d in undersampled_train_labels:
    if(d[0] == 1):
        ones+=1
    if(d[0] == 0):
        zeros+=1

print("ones after oversampling:",ones)
print("zeros after oversampling:",zeros)


undersampled_train_data = np.array(undersampled_train_data)
undersampled_train_data = undersampled_train_data.T
undersampled_train_data = undersampled_train_data.T

undersampled_train_labels= np.array([np.array(xi) for xi in undersampled_train_labels])
undersampled_train_labels = undersampled_train_labels.ravel()


scaler = MinMaxScaler(feature_range = (-1,1))
normalized_train = scaler.fit(undersampled_train_data)
normalized_train_data = scaler.transform(undersampled_train_data)

undersampled_train_data = normalized_train_data

cls = svm.SVC(C=1,kernel = "rbf")
cls.fit(undersampled_train_data,undersampled_train_labels)
predicted = cls.predict(normalized_test_data)

# get the accuracy
# report accuracy on test set
print("undersampled accuracy on the test data:",accuracy_score(test_labels, predicted))
#print("undersampled f1_score:",f1_score(test_labels, predicted))
print("undersampled precision_score:",precision_score(test_labels, predicted))
print("undersampled recall_score:",recall_score(test_labels, predicted)) 

# calculate confusion matrix with test set
plot_confusion_matrix(cls, normalized_test_data, test_labels)
#plt.show()
plt.draw()
plt.savefig('svm4_undersample_confusion.png')


## balanced
print("TRYING BALANCED parameter of SVC")

scaler = MinMaxScaler(feature_range = (-1,1))
normalized_train = scaler.fit(train_data)
normalized_train_data = scaler.transform(train_data)

balanced_train_data = normalized_train_data


cls = svm.SVC(C=1,kernel = "rbf",class_weight="balanced")
cls.fit(balanced_train_data,train_labels)
predicted = cls.predict(normalized_test_data)


print("balanced accuracy on the test data:",accuracy_score(test_labels, predicted))
#print("balanced f1_score:",f1_score(test_labels, predicted))
print("balanced precision_score:",precision_score(test_labels, predicted))
print("balanced recall_score:",recall_score(test_labels, predicted)) 



# calculate confusion matrix with test set
plot_confusion_matrix(cls, normalized_test_data, test_labels)
#plt.show()
plt.draw()
plt.savefig('svm4_balanced_confusion.png')










# svm.SVC class_weight = "balanced"