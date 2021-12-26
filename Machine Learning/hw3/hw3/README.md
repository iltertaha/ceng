# CENG 499 HW 3 README FILE

This homework belongs to İlter Taha Aktolga 2236891 

Sample outputs can be found under the tree_result, svm1_results, svm2_results, svm3_results, svm4_results folders.

Please read this document with gedit or some other editor which supports markdown **to see highlighted parts. **
### USED LIBRARIES 
- pickle
- copy
- math
- sys
- matplotlib.pyplot
- functools
- sklearn
- numpy 
- **scipy** (**IMPORTANT**: Since I have asked the assistant, it is only used for the chi2.cdf    lookup. It is same as copying the related rows from the chi square table. All the necessary calculations for chi square is done by myself. See dt.py for implementation. ) 

## Environment
- Extract hw3.zip to folder
- Make sure you have all the necessary libraries installed.
- Notice that hw3_data  folder is under hw3 folder.
- Run the necessary part to view corresponding results.
- *PDF report* can be found in the hw3 directory.

### Decision Tree ### 
- **As mentioned in the announcement there are 2 different implementation of DT:** my   implementation uses the current subset to find most common values. 
- Also note that if a leaf is not pure, it may not contain any data but the average of the majority. 
- dt.py contains given functions. 
- dt_mini_test.py uses functions in dt.py
- get_before_leaf.py and  print_tree.py  contains helper functions
- My id3 algorithm is also implemented in dt.py
- Accuracies will be printed to stdout for each tree while running the python file. 
- When you run the dt.py with command:
   `python3 dt.py` it will show the accuracies for each 5 tree   
   and will plot and save the trees as png format under the  
   hw3 folder with informative names.

### SVM 
- All parts have different python files namely svm1.py, svm2.py, svm3.py and svm4.py.
- They should be executed under hw3 folder where hw3_data also resides.
- All the parts can be executed with commands:
  - `python3 <filename>.py`

#### SVM Part 1 
  - When runned with `python3 svm1.py`
     - Plots the svm result with different cvalues and saves them.
	 - To see the results look under the hw3 folder after execution is completed.
	 - svm1.py uses drawing function from draw.py  
	 
#### SVM Part 2
- When runned with `python3 svm2.py`
   - Plots the svm result with different kernels.
   - Saves results under hw3 folder.
   - To see the results check the plots after execution.  ,
   
#### SVM Part 3
- Please make the following necessary changes to run the file.
- Before running this part:
   - Open svm3.py with editor
   -  Find the lines (line 12, line 13, line 14):
       - Set `my_kernel` value to only one item which is inside a list.  **(line 12)**
	       -  ex: `my_kernel = ["linear" ]`
	       - Don't put more than one value inside my_kernel list.
   -  Set `my_gamma` value to list which contains gamma values that will be tested.  **(line 13)**   
       - ex: `my_gamma = [0.00001,0.0001,0.001,0.01,0.1,1]`  
	   
   -  Set `my_c`  value to list which contains c values that will be tested. **(line 14)**  
       - ex: `my_c = [0.01,0.1,1,10,100]`  
	   
- After making necessary changes run with  `python3 svm3.py > "output_file.txt` 
    -  It is better to redirect the output to file since it prints results as table.  
	   
#### SVM Part 4 
- Run with command `python3 svm4.py`
- It will save 4 different confusion matrix under hw3 directory.
- Accuracy and other performance metrics will be printed to the stdout 

