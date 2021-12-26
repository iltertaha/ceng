import numpy as np
import string

def vocabulary(data):
    """
    Creates the vocabulary from the data.
    :param data: List of lists, every list inside it contains words in that sentence.
                 len(data) is the number of examples in the data.
    :return: Set of words in the data
    """
    vocab = set()

    
    table = str.maketrans('', '', string.punctuation)
    


    for _,sentence in enumerate(data):
        stripped_sentence = [w.translate(table) for w in sentence]
        for word in stripped_sentence:

            # extract words
            #extracted = ''.join([i for i in word if i.isalpha()])
            #vocab.add(extracted)
            vocab.add(word)


    return vocab


def get_count_of_word(selected,train_data,train_labels,clsname):
    # returns the number of elements in that class
    count = 0
    for i,sentence in enumerate(train_data):
        # if class is selected
        if(train_labels[i]==clsname):
            count+=sentence.count(selected)
    
    
    #print("clsname ",clsname,"selected word:",selected,"cnt ",count)

    return count


def get_count_of_sentence(train_data,train_labels,clsname):
    # returns the number of elements of that class name
    count = 0
    for i,sentence in enumerate(train_data):
        # if class is selected
        if(train_labels[i]==clsname):
            count+=len(sentence)
    
    return count


def get_pure_prob(train_data,train_labels,clsname):
    selected = 0
    for i,d in enumerate(train_data):
        if(train_labels[i] == clsname):
            selected+=1
    
    return selected/len(train_data)


def train(train_data, train_labels, vocab):
    """
    Estimates the probability of a specific word given class label using additive smoothing with smoothing constant 1.
    :param train_data: List of lists, every list inside it contains words in that sentence.
                       len(train_data) is the number of examples in the training data.
    :param train_labels: List of class names. len(train_labels) is the number of examples in the training data.
    :param vocab: Set of words in the training set.
    :return: theta, pi. theta is a dictionary of dictionaries. At the first level, the keys are the class names. At the
             second level, the keys are all of the words in vocab and the values are their estimated probabilities.
             pi is a dictionary. Its keys are class names and values are their probabilities.
    """

    theta = dict()
    pi = dict()

    for clsname in set(train_labels):
        
        temp = dict()

        for word in vocab:
            numer = 1 + get_count_of_word(word,train_data,train_labels,clsname)
            denum = len(vocab) + get_count_of_sentence(train_data,train_labels,clsname)

            res = numer/denum
            temp[word] = res

        theta[clsname] = temp

    for k in set(train_labels):
        pi[k] = get_pure_prob(train_data,train_labels,k)



    #print("theta",theta)



    return theta, pi

def calculate_score(test_d,theta,pi,vocab):
    # highest_score = -1
    # highest_name = ""

    result = list()
    
    for clsname in theta.keys():
        
        adder = 0

        for word in vocab:

            howmany =  test_d.count(word)

            adder+= howmany * np.log(theta[clsname][word])

        hx = np.log(pi[clsname]) + adder

        result.append((hx,clsname))

        # if(hx > highest_score):
        #     highest_name = hx
        #     highest_name = clsname

    
    return result



def test(theta, pi, vocab, test_data):
    """
    Calculates the scores of a test data given a class for each class. Skips the words that are not occurring in the
    vocabulary.
    :param theta: A dictionary of dictionaries. At the first level, the keys are the class names. At the second level,
                  the keys are all of the words in vocab and the values are their estimated probabilities.
    :param pi: A dictionary. Its keys are class names and values are their probabilities.
    :param vocab: Set of words in the training set.
    :param test_data: List of lists, every list inside it contains words in that sentence.
                      len(test_data) is the number of examples in the test data.
    :return: scores, list of lists. len(scores) is the number of examples in the test set. Every inner list contains
             tuples where the first element is the score and the second element is the class name.
    """

    scores = list()

    for i,d in enumerate(test_data):

        reslist = calculate_score(d,theta,pi,vocab)

        scores.append(reslist)


    #print("scores",scores)
    return scores


if __name__ == "__main__":


    ########### READING DATA ###########
    
    print("Reading train_data.txt")
    traindata = open('./hw4_data/news/train_data.txt', 'r') 
    train_data_lines = traindata.readlines() 
    
    count = 0
    # Strips the newline character 
    # for line in train_data_lines: 
    #     print("Line{}: {}".format(count, line.strip())) 

    print("total train sentence: ",len(train_data_lines))
    
    print("Reading train_labels.txt")

    trainlabels = open('./hw4_data/news/train_labels.txt', 'r') 
    train_label_lines = trainlabels.readlines() 
    
    count = 0
    # Strips the newline character 
    # for line in train_label_lines: 
    #     print("Line{}: {}".format(count, line.strip())) 

    print("total train label: ",len(train_label_lines))

    print("Reading test_data.txt")
    
    testdata = open('./hw4_data/news/test_data.txt', 'r') 
    test_data_lines = testdata.readlines() 
    
    count = 0
    # Strips the newline character 
    # for line in test_data_lines: 
    #     print("Line{}: {}".format(count, line.strip())) 

    print("total test sentence",len(test_data_lines))
    
    testlabels = open('./hw4_data/news/test_labels.txt', 'r') 
    test_label_lines = testlabels.readlines() 
    
    count = 0
    # Strips the newline character 
    # for line in test_label_lines: 
    #     print("Line{}: {}".format(count, line.strip())) 

    print("total test label: ",len(test_label_lines))

    ###############################################
    # train_data_lines = train_data_lines[:20]
    # train_label_lines = train_label_lines[:20]
    # test_data_lines = test_data_lines[:20]
    # test_label_lines = test_label_lines[:20]

    print("Creating vocabulary")

    train_sentences = []
    train_label_words = []

    #print(train_sentences)
    for line in train_data_lines:
        splitted = line.split()
        #print((splitted))
        #print("splitted : ",splitted)
        train_sentences.append(splitted)


    for line in train_label_lines:
        train_label_words.append(line.strip())

    vocab = vocabulary(train_sentences)
    print("Vocabulary contains",len(vocab),"words")

    # training
    print("training started")

    import time
    tic = time.time()
    theta, pi = train(train_sentences, train_label_words, vocab)
    toc = time.time()
    
    print("training ended.")
    print("train lasted",toc-tic,"seconds")

    # testing
    test_sentences = []
    test_label_words = []

    for line in test_data_lines:
        splitted = line.split()
        test_sentences.append(splitted)

    for line in test_label_lines:
        test_label_words.append(line.strip())


    print("started testing")
    tic = time.time()
    scores = test(theta, pi, vocab, test_sentences)
    toc = time.time()
    print("test lasted",toc-tic,"seconds")

    
    corrects = 0

    #import sys
    for i,score in enumerate(scores):
        
        numbers= []
        labels = []
        for tp in score:
            numbers.append(tp[0])
            labels.append(tp[1])
            
            #print("numbers",numbers)
            #print("labels",labels)
        
        maxind = numbers.index(max(numbers))
        predicted = labels[maxind]

        if(predicted==test_label_words[i]):
            corrects+=1

    
    print("Test Accuracy: ",corrects/len(test_label_lines))
