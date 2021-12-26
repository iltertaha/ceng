import math
import copy
import pickle
import sys

from print_tree import *
from scipy.stats import chi2
from get_before_leaf import iter_paths,resolve_path,remove_end,get_leaf_last,data_fits


with open("./hw3_data/dt/data.pkl","rb") as f:
    train_data,test_data,attr_vals_list,attr_names = pickle.load(f)

##print("attr_vals",attr_vals_list)
##print("attr_names",attr_names)
# glob_attr_vals_list = [['a', 'b', 'c'],  # First attribute takes values 'a', 'b', 'c'.
#                   ['x', 'y', 'z']  # Secon attribute takes values 'x', 'y', 'z'.
#                   ]  # The class labels are '1', '2', '3'.




# glob_attr_vals_list = attr_vals_list[:-1].copy()


# glob_attr_names = attr_names.copy()

## example

### global attr_vals_list contains all labels
### global attr names does not contain class label

# glob_attr_vals_list = [["overcast","rainy","sunny"],["hot","cool","mild"],["high","normal","low"],["false","true"],["yes","no"]]

# glob_attr_names = ["outlook","temp","humidity","windy","play"]
# glob_attr_names = glob_attr_names[:-1]

glob_attr_vals_list = attr_vals_list.copy()
glob_attr_names = attr_names.copy()
#glob_attr_names = glob_attr_names[:-1]

def divide(data, attr_index, attr_vals_list):
    """Divides the data into buckets according to the selected attr_index.
    :param data: Current data in the node
    :param attr_index: Selected attribute index to partition the data
    :param attr_vals_list: List of values that attributes may take
    :return: A list that includes K data lists in it where K is the number
     of values that the attribute with attr_index can take
    """

    # select attributes
    selected_attrs = attr_vals_list[attr_index]
    
    # length of attributes
    selected_attr_len = len(attr_vals_list[attr_index])

    res = []

    for _ in range(selected_attr_len):
        res.append([])

    # for each data row
    for _,data_row in enumerate(data):
        # decide which list to append
        for i in range(selected_attr_len):
            if(data_row[attr_index]==selected_attrs[i]):
                res[i].append(data_row)

    #print(res)

    return res

def entropy(data, attr_vals_list):
    """
    Calculates the entropy in the current data.
    :param data: Current data in the node
    :param attr_vals_list: List of values that attributes may take
    (Last attribute is for the labels)
    :return: Calculated entropy (float)
    """

    entrp = 0.0
    labels = attr_vals_list[-1]

    counter =[]
    for _ in range(len(labels)):
        counter.append(0)

    
    for i,data_row in enumerate(data):
        for k,val in enumerate(labels):
            if(data_row[-1] == val):
                counter[k] +=1
    
    #print(counter)

    total_elem = sum(counter)
    if(total_elem == 0 ):
        return 0

    for i in range(len(counter)):
        if( counter[i] / total_elem != 0 ):
            x = counter[i] / total_elem
            entrp += -1 * (x*math.log2(x))

    return entrp



def info_gain(data, attr_index, attr_vals_list):
    """
    Calculates the information gain on the current data when the attribute with attr_index is selected.
    :param data: Current data in the node
    :param attr_index: Selected attribute index to partition the data
    :param attr_vals_list: List of values that attributes may take
    :return: information gain (float), buckets (the list returned from divide)
    """

    total_len = len(data)
    #print(total_len)

    entrp = entropy(data,attr_vals_list)
    #print(entrp)
    splitted_data = divide(data,attr_index,attr_vals_list)

    average_entropy = 0.0

    for subset in splitted_data:
        subset_len = len(subset)
        subset_entrp = entropy(subset,attr_vals_list)

        average_entropy+=(subset_len/total_len)*subset_entrp


    return (entrp - average_entropy),splitted_data






def gain_ratio(data, attr_index, attr_vals_list):
    """
    Calculates the gain ratio on the current data when the attribute with attr_index is selected.
    :param data: Current data in the node
    :param attr_index: Selected attribute index to partition the data
    :param attr_vals_list: List of values that attributes may take
    :return: gain_ratio (float), buckets (the list returned from divide)
    """
    
    # calculate intrinsic info of the attribute
    intrinsic_info_of_selected_attribute = 0.0


    # calculate gain ratio 

    information_gain,splitted_data = info_gain(data,attr_index,attr_vals_list)

    total_len = len(data)

    for subset in splitted_data:
        # numerator
        subset_len = len(subset)

        #x = min(0.0,subset_len/total_len)
        if(total_len > 0 and subset_len >0.0):

            
            intrinsic_info_of_selected_attribute += (-1) * (subset_len / total_len) * math.log2(subset_len/total_len)


    if(intrinsic_info_of_selected_attribute == 0 ):
        return 0,splitted_data
    else:

        return (information_gain/intrinsic_info_of_selected_attribute), splitted_data






def gini(data, attr_vals_list):
    """
    Calculates the gini index in the current data.
    :param data: Current data in the node
    :param attr_vals_list: List of values that attributes may take
    (Last attribute is for the labels)
    :return: Calculated gini index (float)
    """
    gini_ind = 0.0
    labels = attr_vals_list[-1]

    counter =[]
    for _ in range(len(labels)):
        counter.append(0)

    
    for i,data_row in enumerate(data):
        for k,val in enumerate(labels):
            if(data_row[-1] == val):
                counter[k] +=1
    

    total_elem = sum(counter)
    if(total_elem == 0 ):
        return 0

    for i in range(len(counter)):
        if( counter[i] / total_elem != 0 ):
            x = counter[i] / total_elem
            gini_ind +=  x**2

    return 1-gini_ind


def avg_gini_index(data, attr_index, attr_vals_list):
    """
    Calculates the average gini index on the current data when the attribute with attr_index is selected.
    :param data: Current data in the node
    :param attr_index: Selected attribute index to partition the data
    :param attr_vals_list: List of values that attributes may take
    :return: average gini index (float), buckets (the list returned from divide)
    """
    
    splitted_data = divide(data,attr_index,attr_vals_list)

    gini_gain = 0.0

    total_len = len(data)

    for subset in splitted_data:
        subset_len = len(subset)
        gini_gain += (subset_len/total_len)*gini(subset,attr_vals_list)

    return gini_gain,splitted_data

def chi_squared_test(data, attr_index, attr_vals_list):
    """
    Calculated chi squared and degree of freedom between the selected attribute and the class attribute
    :param data: Current data in the node
    :param attr_index: Selected attribute index to partition the data
    :param attr_vals_list: List of values that attributes may take
    :return: chi squared value (float), degree of freedom (int)
    """
    if(not len(data)):
        return "no_data","no_data"
    selected_attribs = attr_vals_list[attr_index]

    # create observation table

    # create obs rows 
    obs_rows = []
    for i in range(len(attr_vals_list[-1])):
        obs_rows.append([])

    obs_row_sums = []
    for _ in range(len(attr_vals_list[-1])):
        obs_row_sums.append(0)



    for _ in range(len(selected_attribs)):
        for row in obs_rows:
            row.append(0)

    
    obs_column_sums = []
    for _ in range(len(selected_attribs)):
        obs_column_sums.append(0)

    

    splitted_data = divide(data,attr_index,attr_vals_list)

    #print("splitted data",splitted_data)

    # for each attribute subset
    for subset_ind,subset in enumerate(splitted_data):
        # each data in subset
        for d in subset:
            # decide which class it belongs to
            for j in range(len(attr_vals_list[-1])):
                #print(d[-1],attr_vals_list[-1][j])
                if(d[-1] == attr_vals_list[-1][j]):
                    
                    obs_rows[j][subset_ind]+=1

    #print(obs_rows)

    for i in range(len(obs_rows)):
        column_length = len(obs_rows[i])
        row_sum = 0
        for j in range(column_length):
            row_sum+= obs_rows[i][j]
        
        obs_row_sums[i] = row_sum

    #print("obs_row_sum",obs_row_sums)



    for i in range(len(obs_rows[0])):
        col_length = len(obs_rows)
        col_sum = 0
        for j in range(col_length):
            col_sum+= obs_rows[j][i]
        
        obs_column_sums[i] = col_sum

    #print("obs_column_sum",obs_column_sums)

    total_sum = sum(obs_row_sums)

    #print(total_sum)



    # calculate expectation table 

    exp_rows = []
    for i in range(len(attr_vals_list[-1])):
        exp_rows.append([])

    exp_row_sums = []
    for _ in range(len(attr_vals_list[-1])):
        exp_row_sums.append(0)



    for _ in range(len(selected_attribs)):
        for row in exp_rows:
            row.append(0)

    
    exp_column_sums = []
    for _ in range(len(selected_attribs)):
        exp_column_sums.append(0)


    # for each row
    for i in range(len(obs_row_sums)):
        # for each column
        for j in range(len(obs_column_sums)):
            temp = (obs_row_sums[i] * obs_column_sums[j]) /total_sum
            exp_rows[i][j] = temp 

    #print(exp_rows)


    # apply formula and get result

    res = 0.0

    # (obs - exp )**2 / exp
 
    for i in range(len(obs_row_sums)):
        for j in range(len(obs_column_sums)):
            if(exp_rows[i][j]):
                res+= (obs_rows[i][j] - exp_rows[i][j])**2 / exp_rows[i][j]

    empty_row = 0
    empty_column = 0
    
    for i in range(len(obs_row_sums)):
        zero_flag = 1
        for j in range(len(obs_column_sums)):
            if(exp_rows[i][j]):
                zero_flag = 0
        if(zero_flag):
            empty_row -= 1


    for i in range(len(obs_column_sums)):
        zero_flag = 1
        for j in range(len(obs_row_sums)):
            if(exp_rows[j][i]):
                zero_flag = 0
        if(zero_flag):
            empty_column -= 1


    df = (len(obs_row_sums)+empty_row-1) * (len(obs_column_sums)+empty_column-1)

    return res,df


# checks if the data contains only one class
def isPure(data):

    # if no data 
    if(not len(data)):
        #checkTODO
        return False
    
    # if data, select first class
    first_class = data[0][-1]
    # if different class found 
    # indicates not pure
    for i in range(1,len(data)):
        if(data[i][-1] !=  first_class):
            return False
    
    return True



def calculate_accuracy(tree,my_data,glob_attr_names):
    # init pointer
    curr = tree
    #my_data = my_data[0]
    #print("tree",tree)
    #print(type(curr))
    # loop until tree ends
    while(type(curr) is dict):
        
        # get attribute node
        key = list(curr.keys())[0]
        #print("key-> " , key)
        
        # find which branch to go according to my_data
        glob_ind = attribute_general_index(key,glob_attr_names) 
        #print("glob_ind",glob_ind)
        #print("my_data",my_data)
        #print("selected_branch: ",my_data[glob_ind])
        #print("currbefore",curr)
        curr = curr[key][my_data[glob_ind]]
        #print("currafter",curr)



    #print("currend",curr)
    score = curr.split("\n")[0]
    #print("predicted",score)
    #print("ground",my_data[-1])

    if(score == my_data[-1]):
        return 1
    else:
        #print(my_data)
        #print("hata")
        #exit()
        return 0


def acc(tree,val_data,glob_attr_names):
    total_accs = 0
    total_len = len(val_data)
    #print("test_len",total_len)
    #print("test_1",test_data[0])

    # loop over all test_data
    for d in val_data:
        #print("d:",d)
        res = calculate_accuracy(tree,d,glob_attr_names)
        #print(res)
        if(res == 1):
            total_accs+=1
    
    return total_accs/total_len





def most_common_value(data,attr_index,attr_vals_list):

    #selected_attr_vals = attr_vals_list[attr_index]
    
    classes = []
    for d in data:
        
        classes.append(d[-1])

    # how many from each class 
    class_vs_count = [[x,classes.count(x)] for x in set(classes)]

    max_ind = -1
    max_val = -1

    for k in class_vs_count:
        if(k[1] > max_val):
            max_val = k[1]
            max_ind = k[0]

    #print(class_vs_count)
    #return max_ind

    # prepare output

    res = ""

    for d in class_vs_count:
        res+= str(d[0])+"-> "+str(d[1])+"\n"

    res = max_ind+"\n"+"---\n"+res

    #return str(class_vs_count)
    #print("res",res)
    return res
    #return max_ind

def attribute_general_index(selected,unchanged_attribute_list):
    for i,liste in enumerate(unchanged_attribute_list):
        if(selected == liste):
            return i


def find_best_attribute(data,attr_vals_list,criterion,attr_names):
    #returns the index of the best attribute in the given attr_vals_list

    # returned, x  = info_gain(data,0,attr_vals_list)
    # print(returned)
    # returned, x  = info_gain(data,1,attr_vals_list)
    # print(returned)
    # returned, x  = info_gain(data,2,attr_vals_list)
    # print(returned)
    # returned, x  = info_gain(data,3,attr_vals_list)
    # print(returned)
    # print("---------------\n")

    max_info_gain_index = -1
    max_info_gain_val = -1

    min_info_gain_index = -1
    min_info_gain_val = sys.maxsize

    for i,_ in enumerate(attr_vals_list):
        if(criterion == "inf_gain"):
            #ind = attribute_general_index(attr_vals_list[i],glob_attr_vals_list)
            ind = attribute_general_index(attr_names[i],glob_attr_names)
            new_gain, _ = info_gain(data,ind,glob_attr_vals_list)
            #print("label: ",attr_names[i],"val:",new_gain)

            if(new_gain > max_info_gain_val):
                max_info_gain_val = new_gain
                max_info_gain_index = i

        elif(criterion == "gain_ratio"):
            # attr_vals list i globale cevir
            ind = attribute_general_index(attr_names[i],glob_attr_names)
            new_gain, _ = gain_ratio(data,ind,glob_attr_vals_list) 

            if(new_gain > max_info_gain_val):
                max_info_gain_val = new_gain
                max_info_gain_index = i

        elif(criterion == "average_gini"):
            ind = attribute_general_index(attr_names[i],glob_attr_names)
            new_gain, _ = avg_gini_index(data,ind,glob_attr_vals_list)

            if(new_gain < min_info_gain_val):
                min_info_gain_val = new_gain
                min_info_gain_index = i
        

    if(criterion == "average_gini"):
        res = attribute_general_index(attr_names[min_info_gain_index],glob_attr_names)
        #print("min_info_gain_index",min_info_gain_index)

        label = glob_attr_names[res]
        return min_info_gain_index,res,label

    else:
        res = attribute_general_index(attr_names[max_info_gain_index],glob_attr_names)
        #print("max_info_gain_index",max_info_gain_index)
        label = glob_attr_names[res]
        return max_info_gain_index,res,label
    #print("selected",label)

    
    


def id3(data,attr_index,attr_vals_list,attr_names,pre_prune_flag,criter):
    
    # Create a root node for the tree
    tree = {}

    # If all examples are same type, Return the single-node tree Root, with label.
    if(isPure(data)):
        #print(data)
        #print("--")
        #print(data[0][-1])
        #print("data",data)
        #return str(data[0][-1])
        res = str(data[0][-1])
        res += "\n"+ "---\n"+ data[0][-1]+"->"+str(len(data))
        another = ""
        for d in glob_attr_vals_list[-1]:
            if(d != data[0][-1]):
                another = d

        res += "\n"+ another +"->"+str(0)
        return res #+str(data)

    
    # If number of predicting attributes is empty, then Return the single node tree Root,
    # with label = most common value of the target attribute in the examples

    if(not len(attr_vals_list)):
        return str(most_common_value(data,attr_index,attr_vals_list))
    

    # Otherwise
    
    # A  = The Attribute that best classifies examples.
    
    # inf_gain,gain_ratio,average_gini

    criterion = criter
    # local,global ind, global label
    temp_att_temp = copy.deepcopy(attr_vals_list)
    A, will_be_printed,label = find_best_attribute(data,attr_vals_list,criterion,attr_names)

    

    #print(A,will_be_printed)
    tree[label] = {}

    #debugforcountif(label == "buying"):
        #tree[label] = "ilter"
        #print("preprune: ",most_common_value(data,A,attr_vals_list))
        #exit()
        #return str("ilter")
        
        #return str(len(data))+"b*"+most_common_value(data,attr_index,attr_vals_list)

    if(pre_prune_flag):
        chi_val,df_val = chi_squared_test(data,will_be_printed,glob_attr_vals_list)

        if(chi_val != "no_data"):
            calculated = chi2.cdf(chi_val,df_val)
            if(calculated< 0.9):
                #tree[label] = "ilter"
                return most_common_value(data,A,attr_vals_list)
                #print("haha")
                
                #exit()
                #return str("ilter")
                #print("hello")
                #return str(len(data))+"m*"+most_common_value(data,attr_index,attr_vals_list)

        # Decision Tree attribute for Root = A

    subsets = divide(data,will_be_printed,glob_attr_vals_list)
    # print("data for subs",data)
    # print("A for subs",A)
    # print("attr_val_list for subs",attr_vals_list)
    # print("subs",subsets)

    for i,value in enumerate(attr_vals_list[A]):
        
        # no examples in the subset
        if(not len(subsets[i])):
            #print("val1 ",value)
            temp_att = copy.deepcopy(attr_vals_list)
            tree[label][value] = most_common_value(data,A,temp_att)

        else:
            #print("this attributes branch: ",label)
            #print("val2 ",value)
            copy_list = copy.deepcopy(attr_vals_list)
            copy_attr_names = copy.deepcopy(attr_names)

            #print("selected",copy_attr_names[A])
            
            #print(copy_list)
            
            del copy_list[A]
            del copy_attr_names[A]
            
            tree[label][value] = id3(subsets[i],A,copy_list,copy_attr_names,pre_prune_flag,criter)



    return tree




def postprune(tree,val_data,glob_attr_names,datta):
    # while a useful replacing can be done
    killme= 0 
    highest_acc = acc(tree,val_data,glob_attr_names)
    highest_tree  = tree.copy()
    cnt = 0
    deleted= 0
    while True:
        #print("cnt",cnt)
        #print("highest_tree",highest_tree)
        cnt+=1
        # get all paths to leaves
        pathlist = []

        # get path lists
        ttree = highest_tree.copy()
        pathlist = iter_paths(ttree)
        
        # get valid paths 
        valid_pathlist = []
        for p in pathlist:
            if(len(p) >3):
                valid_pathlist.append(p)

        # no valid path stop
        if(len(valid_pathlist) == 0 ):
            break
        else:
            pathlist = valid_pathlist
                
        #print("path_list",pathlist)
        

        # try deeper leaves first 
        pathlist = sorted(pathlist,key=len,reverse=True)

        # stop when no change
        changed = False
        # for p in pathlist:
        #     print(p)

        
        

        # for each leaf

        for path in pathlist:
            # backup the old tree
            ##old_tree = tree.copy()
            # old acc
            #old_acc = acc(tree,val_data,glob_attr_names)
            #print("highest _acc",highest_acc)

            # put one leaf to parents position
            #try:
            #print("before_prune",old_tree)

            selected_path = copy.deepcopy(list(path))
            #print("sel,",selected_path)
            try:
             
                t2tree = copy.deepcopy(highest_tree)
                
                temp_tree = copy.deepcopy(highest_tree)
                x = get_leaf_last(t2tree,selected_path,glob_attr_names)
                selected_path.append(x)
                
                pruned_tree = remove_end(temp_tree,selected_path,glob_attr_names,datta)
                #print("tree pruned successfully")
                #print(selected_path)
                pruned_acc = acc(pruned_tree,val_data,glob_attr_names)
                #print("pruned_acc",pruned_acc)

            except:
                #print("HATA OLDU",selected_path)
                try:
                    x = get_leaf_last(t2tree,selected_path,glob_attr_names)
                    #print(x)
                except:
                    #print("leafe erisemedim")
                    pruned_acc = 0
                pruned_acc = 0
            
            
            
            
            #exit()
            #except IndexError:
                #print("error")
                #print(path)
                #exit()
                #killme = 1
                #break
            
            # calculate accuracy 
            
           
            
            # if accuracy improved
            if(pruned_acc > highest_acc):
                # continue 
                #print("after_prune",pruned_tree)
                #print("DELETED",path)

                deleted+=1
                #print(pruned_tree)
                highest_acc = pruned_acc
                changed = True
                highest_tree = copy.deepcopy(pruned_tree)
                
                
                #exit()
                #killme=1
                #break
            else:
                pass
                # recover the old tree
                #tree = old_tree.copy()


        if(killme==1):
            #print("killed")
            #exit()
            break

        # nothing new 
        if(not changed):
            # die hard
            print("not changed, breaking")
            #print("highest_final_acc",highest_acc)
            break

        

    # final tree
    print("MOVED",deleted,"leaf to upper nodes")
    #print("CNT",cnt)
    
    return highest_tree


if __name__ == "__main__":
    
        
    # attr_vals_list = [['a', 'b', 'c'],  # First attribute takes values 'a', 'b', 'c'.
    #                   ['x', 'y', 'z']  # Secon attribute takes values 'x', 'y', 'z'.
    #                   ]  # The class labels are '1', '2', '3'.

    # data = [['a', 'x', '1'],
    #         ['a', 'y', '1'],
    #         ['b', 'x', '2'],
    #         ['c', 'x', '1'],
    #         ['c', 'y', '2']]


    attr_vals_list = attr_vals_list[:-1]
    #attr_names = attr_names[:-1]

    train_data = train_data[:]
    val_offset = int(len(train_data)*0.8)

    data = train_data[:val_offset] 
    val_data = train_data[val_offset:] 

    #data = train_data[:]

    # example

    # attr_vals_list and attr_names dont contain the labels

    # attr_vals_list = [["overcast","rainy","sunny"],["hot","cool","mild"],["high","normal","low"],["false","true"],["yes","no"]]
    # attr_vals_list = attr_vals_list[:-1]
    # attr_names = ["outlook","temp","humidity","windy","play"]
    # attr_names = attr_names[:-1]

    # data = [["overcast","hot","high","false","yes"],
    # ["overcast","cool","normal","true","yes"],
    # ["overcast","mild","high","true","yes"],
    # ["overcast","hot","normal","false","yes"],
    # ["rainy","mild","high","false","yes"],
    # ["rainy","cool","normal","false","yes"],
    # ["rainy","cool","normal","true","no"],
    # ["rainy","mild","normal","false","yes"],
    # ["rainy","mild","high","true","no"],
    # ["sunny","hot","high","false","no"],
    # ["sunny","hot","high","true","no"],
    # ["sunny","mild","high","false","no"],
    # ["sunny","cool","normal","false","yes"],
    # ["sunny","mild","normal","true","yes"]]

    pre_prune_flag = 0
    criter = "inf_gain"
    tree = id3(train_data,None,attr_vals_list,attr_names,pre_prune_flag,criter)


    print("tree with inf_gain criterion")
    #print(tree)
    plot_tree(tree,'tree_inf_gain_nopruning',attr_names)
    print("acc",acc(tree,test_data,glob_attr_names))

    criter = "average_gini"
    #tree = id3(train_data,None,attr_vals_list,attr_names,pre_prune_flag,criter)
    #print("acc",acc(tree,test_data,glob_attr_names))

    print("tree with average_gini criterion")
    #print(tree)
    plot_tree(tree,'tree_average_gini_nopruning',attr_names)
    print("acc",acc(tree,test_data,glob_attr_names))

    criter = "gain_ratio"
    tree = id3(train_data,None,attr_vals_list,attr_names,pre_prune_flag,criter)
    print("tree with gain_ratio criterion")
    #print(tree)
    plot_tree(tree,'tree_gain_ratio_nopruning',attr_names)
    print("acc",acc(tree,test_data,glob_attr_names))


    #postprune()


    # splitted_data = divide(data,0,attr_vals_list)
    # for d in splitted_data:
    #     print(d)

    # returned, x  = info_gain(data,0,attr_vals_list)
    # print(returned)
    # returned, x  = info_gain(data,1,attr_vals_list)
    # print(returned)
    # returned, x  = info_gain(data,2,attr_vals_list)
    # print(returned)
    # returned, x  = info_gain(data,3,attr_vals_list)
    # print(returned)
    # print("---------------\n")
    # print(len(splitted_data))
    # returned, x = info_gain(splitted_data[1],1,attr_vals_list)
    # print("outlooksub rainy temp: ",returned)
    # returned, x = info_gain(splitted_data[1],2,attr_vals_list)
    # print("outlooksub rainy humd: ",returned)
    # returned, x = info_gain(splitted_data[1],3,attr_vals_list)
    # print("outlooksub rainy wind: ",returned)
    # print("x[0] = ",x[0])
    # print(isPure(x[0]))
    # print("x[1] = ",x[1])
    # print(isPure(x[1]))

    # # returned, x = info_gain(splitted_data[0],3,attr_vals_list)
    # # print("outlooksub: ",returned)
    # #plot_tree(tree,"test_ilter")
    # returned, x = info_gain(splitted_data[2],1,attr_vals_list)
    # print("outlooksub sunny temp: ",returned)
    # returned, x = info_gain(splitted_data[2],2,attr_vals_list)
    # print("outlooksub sunny humd: ",returned)
    # print("x[0] = ",x[0])
    # print(isPure(x[0]))
    # print("x[1] = ",x[1])
    # print(isPure(x[1]))
    # print("x[2] = ",x[2])
    # print(isPure(x[2]))
    # returned, x = info_gain(splitted_data[2],3,attr_vals_list)
    # print("outlooksub sunny wind: ",returned)

    #attr_names               ['buying', 'maint', 'doors', 'persons', 'lug_boot', 'safety']
    #a = calculate_accuracy(tree,[['high', 'high', '3', 'more', 'big', 'med', 'unacc']],glob_attr_names)

    #print(a)

    print("------\n")

    #print(acc(tree,train_data,glob_attr_names))


    pre_prune_flag = 1
    tree = id3(train_data,None,attr_vals_list,attr_names,pre_prune_flag,criter)
    print("PREPURINING tree with gain_ratio criterion")
    #print(tree)
    print("acc",acc(tree,test_data,glob_attr_names))
    plot_tree(tree,'tree_gain_ratio_prepruning',attr_names)

    pre_prune_flag = 0
    print("POSTPURINING tree with gain_ratio criterion")
    #print("tree",tree)
    tree = id3(data,None,attr_vals_list,attr_names,pre_prune_flag,criter)
    tree = postprune(tree,val_data,glob_attr_names,data)
    print("acc",acc(tree,test_data,glob_attr_names))
    plot_tree(tree,'tree_gain_ratio_postpruning',attr_names)
