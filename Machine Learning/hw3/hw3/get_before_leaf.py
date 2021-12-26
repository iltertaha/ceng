import collections
from functools import reduce
from print_tree import *





def delete_keys_from_dict(dict_del, lst_keys):
    for k in lst_keys:
        try:
            del dict_del[k]
        except KeyError:
            pass
    for v in dict_del.values():
        if isinstance(v, dict):
            delete_keys_from_dict(v, lst_keys)

    return dict_del


##
def get_leaf_last(tree,my_path,glob_attr_names):
    # init pointer
    curr = tree
    #my_data = my_data[0]
    #print("tree",tree)
    #print(type(curr))
    # loop until tree ends
    path = my_path.copy()
    #print("pppp",path)
    while(len(path)):
        
        # get attribute node
        #key = list(curr.keys())[0]
        #print("key-> " , key)
        
        # find which branch to go according to my_data
        #glob_ind = attribute_general_index(key,glob_attr_names) 
        #print("glob_ind",glob_ind)
        #print("my_data",my_data)
        #print("selected_branch: ",my_data[glob_ind])
        #print("currbefore",curr)
        id = path.pop(0)
        curr = curr[id]
        #print(curr)
        #print("currafter",curr)
    return curr
##

# used

def iter_paths(tree, parent_path=()):
    for path, node in tree.items():
        current_path = parent_path + (path,)
        if isinstance(node, collections.Mapping):
            for inner_path in iter_paths(node, current_path):
                yield inner_path
        else:
            yield current_path


# tree = {'persons': {'2': 'unacc\n---\nunacc->12\nacc->0', '4': {'lug_boot': {'small': 'unacc\n---\nunacc->4\nacc->0', 'med': 'acc\n---\nacc->3\nunacc->0', 'big': {'safety': {'low': 'unacc\n---\nunacc->2\nacc->0', 'med': 'acc\n---\nacc->2\nunacc->0', 'high': 'unacc\n---\nunacc->1\nacc->0'}}}}, 'more': {'maint': {'vhigh': 'unacc\n---\nunacc->3\nacc->0', 'high': 'unacc\n---\nunacc->1\nacc->0', 'med': 'acc\n---\nacc->1\nunacc->0', 'low': 'acc\n---\nacc->1\nunacc->0'}}}}

# res = list(iter_paths(tree))
# for r in res:
#     print(r)
# print(list(iter_paths(tree)))
# print("-----\n")

def before_leaves(tree):
    leaves = list(iter_paths(tree))
    for leaf in leaves:

        temp_tree = tree
        for l in leaf[:-2]:
            temp_tree = temp_tree[l]

        #print(temp_tree)
        temp_tree[leaf[-1]] = {}


    #print(list(iter_paths(tree)))


# get leaves 

def getFromPath(dictionary, path):
    curr = dictionary
    #path = path.split("/")[1:] # Gets rid of '#' as it's uneccessary 
    while(len(path)):
        key = path.pop(0)
        curr = curr.get(key)
        if (type(curr) is not dict and len(path)):
            print("Path does not exist!")
            return None 
    return curr

#print("test")
#print(getFromPath(tree, ['persons', '4', 'lug_boot', 'big',"safety","low"])) #Your value

def attribute_general_index2(selected,unchanged_attribute_list):
    for i,liste in enumerate(unchanged_attribute_list):
        if(selected == liste):
            return i


# replace leaf with its parent


def resolve_path(dictionary, path):
    return reduce(dict.get, path, dictionary)


def data_fits(attrs,vals,glob_inds,my_data,glob_attr_names):
    ##print("attrs",attrs)
    ##print("vals",vals)
    ##print("my_data",my_data)
    

    for i,ind in enumerate(glob_inds):
        if(my_data[ind] != vals[i]):
            return False
    return True
    

def remove_end(dictionary, path,glob_attr_names,datta):
    ##print("path",path)
    parent_path, last = path[:-4], path[-4]
    parent = resolve_path(dictionary, parent_path)
    #del parent[last]
    
   # set new node
    #print("PATH",path)
    attrs=[]
    vals= []
    glob_inds = []
    for i in range(0,(len(path)-1)-2,2):
        
        attr = path[i]
        val = path[i+1]
        attrs.append(attr)
        vals.append(val)
        glob_ind = attribute_general_index2(attr,glob_attr_names)
        glob_inds.append(glob_ind)


    acc = 0
    total_len = 0 
    unacc = 0
    
    for d in datta:
        # for each data
        if(data_fits(attrs,vals,glob_inds,d,glob_attr_names)):
            #print("fitted_data",d)
            total_len +=1

            if(d[-1].split("-")[0] == "acc"):
                acc +=1
    
    unacc = total_len-acc

    if(acc > unacc):
        parent[last] =  str("acc\n")+"---\n"+"acc->"+str(acc)+"\n"+"acc->"+str(unacc)
    else:
        parent[last] =  str("unacc\n")+"---\n"+"unacc->"+str(unacc)+"\n"+"acc->"+str(acc)
            


    #parent[last] = path[-1]
    #print("ACC",acc)
    #print("UNACC",unacc)
    #print("totallen",total_len)
    return dictionary


# tree = {'safety': {'low': 'unacc\n---\nunacc->18\nacc->0', 'med': {'doors': {'2': 'unacc\n---\nunacc->3\nacc->0', '3': {'persons': {'2': 'unacc\n---\nunacc->1\nacc->0', '4': 'acc\n---\nacc->2\nunacc->0', 'more': 'acc\n---\nacc->2\nunacc->0'}}, '4': 'unacc\n---\nunacc->2\nacc->0', '5more': {'buying': {'vhigh': 'unacc\n---\nunacc->2\nacc->0', 'high': 'unacc\n---\nacc-> 1\nunacc-> 3\n', 'med': 'unacc\n---\nunacc->1\nacc->0', 'low': 'acc\n---\nacc->1\nunacc->0'}}}}, 'high': {'persons': {'2': 'unacc\n---\nunacc->7\nacc->0', '4': {'maint': {'vhigh': 'unacc\n---\nunacc->1\nacc->0', 'high': 'acc\n---\nacc->1\nunacc->0', 'med': 'acc\n---\nacc->3\nunacc->0', 'low': 'acc\n---\nacc-> 4\nunacc-> 1\n'}}, 'more': 'acc\n---\nacc->4\nunacc->0'}}}}

# a =[('safety', 'med', 'doors', '3', 'persons', '2'),
# ('safety', 'med', 'doors', '3', 'persons', '4'),
# ('safety', 'med', 'doors', '3', 'persons', 'more'),
# ('safety', 'med', 'doors', '5more', 'buying', 'vhigh'),
# ('safety', 'med', 'doors', '5more', 'buying', 'high'),
# ('safety', 'med', 'doors', '5more', 'buying', 'med'),
# ('safety', 'med', 'doors', '5more', 'buying', 'low'),
# ('safety', 'high', 'persons', '4', 'maint', 'vhigh'),
# ('safety', 'high', 'persons', '4', 'maint', 'high'),
# ('safety', 'high', 'persons', '4', 'maint', 'med'),
# ('safety', 'high', 'persons', '4', 'maint', 'low'),
# ('safety', 'med', 'doors', '2'),
# ('safety', 'med', 'doors', '4'),
# ('safety', 'high', 'persons', '2'),
# ('safety', 'high', 'persons', 'more')
# ]

# a = ['safety', 'med', 'doors', '3', 'persons', '2']
# b = ['safety', 'med', 'doors', '3']
# c = ['safety', 'med', 'doors', '3', 'persons', 'more']

#print("asd",getFromPath(tree,a))
#print(get_leaf_last(tree,a,['buying', 'maint', 'doors', 'persons', 'lug_boot', 'safety']))

# print("before_tree")
# print(tree)
# plot_tree(tree,'before_prune',['buying', 'maint', 'doors', 'persons', 'lug_boot', 'safety'])
# x = get_leaf_last(tree,a,['buying', 'maint', 'doors', 'persons', 'lug_boot', 'safety'])
# a.append(x)
# print("a",a)
# tree = del_endpoint(tree,a)

# print("after_tree")
# print(tree)

# plot_tree(tree,'after_prune',['buying', 'maint', 'doors', 'persons', 'lug_boot', 'safety'])

# x = get_leaf_last(tree,b,['buying', 'maint', 'doors', 'persons', 'lug_boot', 'safety'])
# b.append(x)
# tree = del_endpoint(tree,b)

# plot_tree(tree,'after_prune2',['buying', 'maint', 'doors', 'persons', 'lug_boot', 'safety'])

#before_leaves(tree)

# tree = del_endpoint(tree,b.append(getFromPath(tree,b)))
# plot_tree(tree,'after_prune2',['buying', 'maint', 'doors', 'persons', 'lug_boot', 'safety'])


# tree = del_endpoint(tree,['persons', 'more','maint','high','unacc'])
# plot_tree(tree,'after_prune3',['buying', 'maint', 'doors', 'persons', 'lug_boot', 'safety'])