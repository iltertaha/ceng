# import pydot

# def draw(parent_name, child_name):
#     edge = pydot.Edge(parent_name, child_name)
#     graph.add_edge(edge)

# def visit(node, parent=None):
#     for k,v in node.items():
#         if isinstance(v, dict):
#             # We start with the root node whose parent is None
#             # we don't want to graph the None node
#             if parent:
#                 draw(parent, k)
#             visit(v, k)
#         else:
#             draw(parent, k)
#             # drawing the label using a distinct name
#             node_to = pydot.Node(k, k+'_'+v, shape='box')
#             graph.add_node(node_to)
            

# graph = pydot.Dot(graph_type='digraph')
# visit(menu)
# graph.write_png('example1_graph.png')

import pydot
import time










GLOBAL_CNT = 0

def treecreator(graph, dictionary,attr_names, parent_node=None,parent_label = None):
    
    # for each key in dict 
    global GLOBAL_CNT
    for k in dictionary.keys():

        if parent_node is not None:



            # if dict
            if isinstance(dictionary[k], dict):
                if(k in attr_names):
                    #print("o bir attr : ",k)
                    pass
                
                
                if(k in attr_names):
                    from_name = parent_node.get_name().replace("\"", "") + '_' + str(k)+str(time.time())+str(GLOBAL_CNT)
                    #from_name =   '_' + str(k)+str(time.time())+str(GLOBAL_CNT)
                    GLOBAL_CNT+=1
                    from_label = str(k)

                    res = list(dictionary[k].keys())[0] 
                    node_from = pydot.Node(from_name, label=k)
                    graph.add_node(node_from)
                    graph.add_edge( pydot.Edge(parent_node, node_from,label=parent_label) )
                    
                    
                    treecreator(graph, dictionary[k],attr_names, node_from)
                else:
                    res = list(dictionary[k].keys())[0] 
                    #print("res",res)
                    #print("dict",dictionary[k])
                    parent_label = k
                    treecreator(graph, dictionary[k],attr_names, parent_node,parent_label)
                

            # leaf node
            else: 

                # print leaf node
                to_name = str(k) + '_' + str(dictionary[k]+str(parent_node)+str(parent_label)+str(time.time()))+ str(GLOBAL_CNT)
                GLOBAL_CNT+=1 # unique name
                #print("dict[k] =",dictionary[k])
                #print("to_name",to_name)
                to_label = str(dictionary[k])

                node_to = pydot.Node(to_name, label=to_label, shape='box')
                graph.add_node(node_to)

                # add edge from parent
                graph.add_edge(pydot.Edge(parent_node, node_to,label=k))

                #node_from.set_name(to_name)

        else:

            from_name =  str(k)+str(time.time())+str(GLOBAL_CNT)
            GLOBAL_CNT+=1
            from_label = str(k)

            node_from = pydot.Node(from_name, label=from_label)
            graph.add_node(node_from)
            treecreator(graph, dictionary[k],attr_names, node_from)


def plot_tree(tree, name,attr_names):

    # first you create a new graph, you do that with pydot.Dot()
    graph = pydot.Dot(graph_type='graph')

    treecreator(graph, tree,attr_names)

    graph.write_png(name+'.png')


#plot_tree(tree,'name')