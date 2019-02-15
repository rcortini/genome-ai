import numpy as np
import string

# let's limit ourselves to only lowercase characters
letters = string.ascii_lowercase
nletters = len(letters)
alpha_to_n = {letters[i] : i for i in range(nletters)}
n_to_alpha = {i : letters[i] for i in range(nletters)}

def encode_sentence(sentence_alpha, alpha_to_n) :
    return [alpha_to_n[sentence_alpha[i]] for i in range(len(sentence_alpha))]

def decode_sentence(sentence_n, n_to_alpha) :
    return ''.join([n_to_alpha[sentence_n[i]] for i in range(len(sentence_n))])

# let's then build a function that can generate a random sequence with or
# without a easter_egg sequence (Ruggero)
def generate_sentences(nsentences, sentence_length, alpha_to_n,
                       seed = None, easter_egg = 'ruggero', noise = None) :
    
    # get length of the alphabet
    nletters = len(alpha_to_n)
    
    # init random number generator if seed is given
    if seed is not None :
        np.random.seed(seed)
    
    # generate the sentences
    sentences = np.random.randint(low=0, high=nletters, size=(nsentences, sentence_length))
    
    # if the user doesn't require the sentences to include the easter_egg, we're done,
    # otherwise we have to include the easter_egg at some location
    if easter_egg is not None :
        
        # get the encoding of the easter_egg
        easter_egg_encoding = encode_sentence(easter_egg, alpha_to_n)
        
        # generate a list of integers that will specify at which location in the sentence
        # the easter_egg will be inserted
        easter_egg_length = len(easter_egg)
        easter_egg_location = np.random.randint(low=0,
                                                high=sentence_length-easter_egg_length+1,
                                           size=nsentences)
        for i, loc in enumerate(easter_egg_location) :
            sentences[i, loc:loc+easter_egg_length] = easter_egg_encoding
        
        # now for the noise part: if user requested noise, then we add it
        if noise is not None :
            
            # first, in this case we generate a list of integers that represent
            # the numbers of letters of the easter_egg that we want to mutate. This
            # will be drawn from a uniform distribution between 0 and noise, where
            # noise is passed by the user
            mutation_size = np.random.randint(low = 0, high = noise, size=nsentences)
            
            # we the proceed to the mutation
            for i in range(nsentences) :
                
                # we remember where was the easter_egg in this sentence
                loc = easter_egg_location[i]
                
                # we get the number of letters to mutate
                n = mutation_size[i]
                
                # we pick n letters at random in the easter_egg
                mutation_locations = np.random.randint(low=0, high=easter_egg_length, size=n)
                mutations = np.random.randint(low=0, high=nletters, size=n)
                
                # we then do the mutation
                sentences[i, loc+mutation_locations] = mutations
                
    # at the end, return the generated sentences
    # return sentences, mutation_size
    return sentences

def shuffle_data(data, targets) :
    """
    Takes a nsentences x nwords "data" array and a nsentences-long "targets"
    array and performs a random permutation of the order, preserving the correspondence
    between the row index of the data and the row index of the targets.
    """
    N = data.shape[0]
    perm = np.random.choice(N, size=N, replace=False)
    return data[perm], targets[perm]

def prepare_data(sentence_length, ntrain, nvalid, ntest, alpha_to_n, noise=None) :
    N = ntrain + nvalid + ntest
    
    # generate N sentences without the easter_egg
    sentences_without = generate_sentences(N, sentence_length, alpha_to_n,
                                           easter_egg = None)
    targets_without = np.zeros(N, dtype=np.int32)
    
    # generate N sentences with the easter_egg
    sentences_with = generate_sentences(N, sentence_length, alpha_to_n,
                                           easter_egg = 'ruggero', noise = noise)
    targets_with = np.ones(N, dtype=np.int32)
    
    # now stack everything and shuffle
    data = np.vstack((sentences_without, sentences_with))
    targets = np.concatenate((targets_without, targets_with))
    
    # shuffle the data
    data, targets = shuffle_data(data, targets)
    
    # now we partition data and targets into train, valid, and test sets
    train_data = data[:2*ntrain, :]
    train_targets = targets[:2*ntrain]
    valid_data = data[2*ntrain:2*(ntrain+nvalid), :]
    valid_targets = targets[2*ntrain:2*(ntrain+nvalid)]
    test_data = data[2*(ntrain+nvalid):, :]
    test_targets = targets[2*(ntrain+nvalid):]
    
    # one further step is required, then return
    train_data = np.expand_dims(train_data, axis = 2)
    valid_data = np.expand_dims(valid_data, axis = 2)
    test_data = np.expand_dims(test_data, axis = 2)
    return train_data, train_targets,\
           valid_data, valid_targets,\
           test_data, test_targets
