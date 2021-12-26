import copy


def forward(A, B, pi, O):
    """
    Calculates the probability of an observation sequence O given the model(A, B, pi).
    :param A: state transition probabilities (NxN)
    :param B: observation probabilites (NxM)
    :param pi: initial state probabilities (N)
    :param O: sequence of observations(T) where observations are just indices for the columns of B (0-indexed)
        N is the number of states,
        M is the number of possible observations, and
        T is the sequence length.
    :return: The probability of the observation sequence and the calculated alphas in the Trellis diagram with shape
             (N, T) which should be a numpy array.
    """


    # result trellis which will be returned
    res = list()
    obs_length = len(O)
    state_length = len(A[0])

    # add dimension to trellis considering the number of states
    for _ in range(state_length):
        res.append([])

    # temporary trellis for calculations 
    trellis = []

    # first dimension in the trellis will be as long as the number of observations
    for _ in range(obs_length):
        trellis.append([])

    # for each observation dimension
    for x in range(len(trellis)):
        for _ in range(state_length):
            trellis[x].append([]) 

    # create trellis diagram 
    for ind in range(state_length):
        first_obs = O[0]
        get_obs = B[ind][first_obs]
        trellis[0][ind] = pi[ind] * get_obs


    # iterate over trellis and calculate probabilities
    for i in range(1, obs_length):
            
        for j in range(state_length):
            

            # calculate the sum as we did in recitation
            total = 0
            for k in range(state_length):
                obs_prob = B[j][O[i]]
                calc_prob = trellis[i-1][k] * A[k][j]
                total+= obs_prob * calc_prob
            
            # assign it to the trellis's corresponding location
            trellis[i][j] = total
            
    
    
    # convert the result trellis into the expected format in the sample out
    # rows and columns inverted
    for col in trellis:
        for key, value in enumerate(col):
            res[key].append(value)

   
    # get the final probability
    last_item_index = len(O)-1

    # sum the probability for the final
    final_probability  = 0.0
    for i in range(state_length):
        final_probability += trellis[last_item_index][i]
    
    # prints for debugging
    #print("prob:",final_probability)
    #print("trellis:",trellis)
    #print("res:",res)


    # return final probability and the resulted trellis

    return final_probability,res




def viterbi(A, B, pi, O):
    """
    Calculates the most likely state sequence given model(A, B, pi) and observation sequence.
    :param A: state transition probabilities (NxN)
    :param B: observation probabilites (NxM)
    :param pi: initial state probabilities(N)
    :param O: sequence of observations(T) where observations are just indices for the columns of B (0-indexed)
        N is the number of states,
        M is the number of possible observations, and
        T is the sequence length.
    :return: The most likely state sequence with shape (T,) and the calculated deltas in the Trellis diagram with shape
             (N, T). They should be numpy arrays.
    """


    # will be used as return trellis as in the forward implementation above
    res = list()


    # temporary trellis to make calculations
    # calculation is done as in the recitation material
    # where states are columnwise and obs are rowwise

    trellis = list()
    
    # stores the best path 
    best_sequence = list()
    

    # setting the trellis where first dim will be observations
    for _ in range(len(O)):
        trellis.append([])
        
    
    state_length = len(A[0])
    obs_length = len(O)

    # for each observation there will be n state
    # second dim is num of states
    for x in range(len(trellis)):
        for _ in range(state_length):
            trellis[x].append([]) 


    # set the resulting trellis' and path list's dimensions
    # more straightforward
    # there may be constructor for direct initialization, anyways

    for _ in range(state_length):
        res.append([])
        best_sequence.append([])


    # calculating the first observation probabilities 
    # also inits path
    for j in range(state_length):
        get_obs = B[j][O[0]]
        trellis[0][j] = pi[j] * get_obs
        best_sequence[j] = [j]
    

    # for each observation
    for i in range(1, obs_length):

        # create and add dimensions to an array which 
        # will be used as a temporary sequence holder
        temp_seq = []
        for _ in range(state_length):
            temp_seq.append([])

        
        # for each state
        for j in range(state_length):

            max_prob = -1
            max_prob_ind = -1
            # calculate the probability
            # get max
            for k in range(state_length):
                obs_prob = B[j][O[i]]
                calc_prob = trellis[i-1][k] * A[k][j]
                current_prob = obs_prob * calc_prob

                if(current_prob > max_prob):
                    max_prob = current_prob
                    max_prob_ind = k
            
            # update the best path with the state
            # backing track
            new_seq = best_sequence[max_prob_ind] + [j]   

            

            # assign it to the trellis' corresponding location
            trellis[i][j] = max_prob
            
            temp_seq[j] = new_seq
              
 
        best_sequence = copy.deepcopy(temp_seq)

    # find the highest probability
    ind = 0 if len(O)==1 else i
 
    max_prob = -1
    max_prob_ind = -1

    # put all probabilities into one list and take the maximum
    # corresponding index will be the corresponding state

    compare_list = []
    for col in range(state_length):
        compare_list.append(trellis[ind][col])
        
    for i,val in enumerate(compare_list):
        if(val > max_prob):
            max_prob = val
            max_prob_ind = i

    # invert the trellis to meet with the output format
    for col in trellis:
        for key, value in enumerate(col):
            res[key].append(value)



    # prints for debugging
    # print("prob",prob)
    # print("path",best_path[state])
    # print("vit",trellis)
    #print("best sequence",best_sequence)

    # return the best path found with backtracking and the resulted trellis
    return (best_sequence[max_prob_ind],res)