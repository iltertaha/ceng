import sklearn
import numpy as np
from sklearn import svm
from sklearn.preprocessing import MinMaxScaler
from draw import draw_svm
from sklearn.model_selection import GridSearchCV
from sklearn.metrics import classification_report



# Set the parameters by cross-validation
my_kernel = ["linear"]
my_gamma = [0.00001,0.0001,0.001,0.01,0.1,1]
my_c = [0.01,0.1,1,10,100]


print("loaded data")
train_data = np.load("hw3_data/catdog/train_data.npy")
train_labels = np.load("hw3_data/catdog/train_labels.npy")
test_data = np.load("hw3_data/catdog/test_data.npy")
test_labels = np.load("hw3_data/catdog/test_labels.npy")

print("traindata_shape ",train_data.shape)
print("trainlabels_shape ",train_labels.shape)
print("testdata_shape ",test_data.shape)
print("testlabels_shape ",test_labels.shape)

#train_labels = train_labels.reshape(1, -1)
#test_labels = test_labels.reshape(1, -1)
#np.expand_dims(train_labels, axis=1)     
#np.expand_dims(test_labels, axis=1)  
samples = train_data.shape[0]
train_data = train_data.reshape((samples, -1))  
test_data = test_data.reshape((samples,-1))


print("after reshape")
print("traindata_shape ",train_data.shape)
print("trainlabels_shape ",train_labels.shape)
print("testdata_shape ",test_data.shape)
print("testlabels_shape ",test_labels.shape)


# normalize train and test data between -1 and 1
print("normalized data")

scaler = MinMaxScaler(feature_range = (-1,1))
normalized_train = scaler.fit(train_data)
normalized_train_data = scaler.transform(train_data)

scaler2 = MinMaxScaler(feature_range = (-1,1))
normalized_test = scaler2.fit(test_data)
normalized_test_data = scaler2.transform(test_data)

train_data = normalized_train_data
test_data  = normalized_test_data
print("norm traindata_shape ",train_data.shape)
#print("norm trainlabels_shape ",train_labels.shape)
print("norm testdata_shape ",test_data.shape)
#print("norm testlabels_shape ",test_labels.shape)


# # linear kernel 


# # her C degeri kadar eleman oluyor satırda
# c_list = [10,100]
# len_c_list = len(c_list)
# parameters = {'kernel':['linear'], 'C':c_list}
# svc = svm.SVC()
# print("grid search started")

# # defult grid search uses 
# clf = GridSearchCV(svc, parameters)
# clf.fit(train_data, train_labels)

# # GridSearchCV(estimator=svm.SVC(),param_grid={'C': [1, 10], 'kernel': ('linear', 'rbf')})
# # print("grid search ended")

# #print(clf.cv_results_)

# for k,v in clf.cv_results_.items():
#     print(k,v)
# print("-----")

# res = []
# for i in range(len_c_list):
#     res.append(0)

# for c in range(len_c_list):
#     cnt= 0

#     for k,v in clf.cv_results_.items():
#         #print(k,v)
#         if(k == "split"+str(cnt)+"_test_score"):
#             res[c] += v[c]
#             cnt+=1


# for i in range(len(res)):
#     res[i] = res[i] / 5


# print(res)

# max_ind = -1
# max_val = -1
# for i in range(len(res)):
#     if(res[i] > max_val):
#         max_val = res[i]
#         max_ind = i

# print("Best Cross Val Accuracy: ",res[max_ind])
# print("Best Cross Val Params: ",c_list[max_ind])
    
# #normalized_train_data = sklearn.preprocessing.normalize(train_data, axis=0)
# exit()
# # rbf kernel

# # her gama değeri için

# gamma_values = [0.00001,0.0001,0.001,0.01,0.1,1]
# c_list = [0.01,0.1,1,10,100]
# len_c_list = len(c_list)
# res = []
# for i in range(len_c_list):
#     res.append(0)


#     for gamma in gamma_values:
#         print("for gamma value : ",gamma)

#     for c in range(len_c_list):
#         cnt= 0

#         for k,v in clf.cv_results_.items():
#             #print(k,v)
#             if(k == "split"+str(cnt)+"_test_score"):
#                 res[c] += v[c]
#                 cnt+=1


#     for i in range(len(res)):
#         res[i] = res[i] / 5


#     print(res)

#     max_ind = -1
#     max_val = -1
#     for i in range(len(res)):
#         if(res[i] > max_val):
#             max_val = res[i]
#             max_ind = i

#     print("gamma: ",gamma)
#     print("Best Cross Val Accuracy: ",res[max_ind])
#     print("Best Cross Val Params: ",c_list[max_ind])
            
        



if(my_kernel == ["linear"]):
    selected_parameters = [{'kernel': my_kernel, 'C': my_c}]
    print("selected_parameters are : ",selected_parameters)

else:
    selected_parameters = [{'kernel': my_kernel,'gamma': my_gamma,
                     'C': my_c}]
    print("selected_parameters are : ",selected_parameters)




# Set the parameters by cross-validation
#tuned_parameters = [{'kernel': ['rbf'],'gamma': [0.00001,0.0001,0.001,0.01,0.1,1],
#                     'C': [0.01,0.1,1,10,100]}]#,
                    #{'kernel': ['linear'], 'C': [1, 10, 100, 1000]}]

score = "accuracy"


print("# Grid search starts ")
print("\n")

clf = GridSearchCV(
    svm.SVC(), selected_parameters, scoring=score#'%s_macro' % score
)
clf.fit(train_data, train_labels)

print("Best parameters\n")
print(clf.best_params_)

print("Grid scores with kfold cross validation k=5 by default\n")

means = clf.cv_results_['mean_test_score']
stds = clf.cv_results_['std_test_score']
for mean, std, params in zip(means, stds, clf.cv_results_['params']):
    print("%0.3f (+/-%0.03f) for %r"
          % (mean, std * 2, params))
   

print("Results on Test Set\n")

y_true, y_pred = test_labels, clf.predict(test_data)
print(classification_report(y_true, y_pred))

