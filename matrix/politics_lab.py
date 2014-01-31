voting_data = list(open("voting_record_dump109.txt"))

## Task 1

def create_voting_dict():
    senatorVotes = dict()
    for senator in voting_data:
        senatorData = senator.split()
        senatorName = senatorData[0]
        votes = senatorData[3:]
        votesList = []
        for vote in votes:
            votesList.append(int(vote))
        senatorVotes[senatorName] = votesList
    return senatorVotes
    

## Task 2

def policy_compare(sen_a, sen_b, voting_dict):
    sen_a_votes = voting_dict[sen_a]
    sen_b_votes = voting_dict[sen_b]
    sim = 0 
    for i in range(len(sen_a_votes)):
        sim += sen_a_votes[i] * sen_b_votes[i]
    return sim


## Task 3

def most_similar(sen, voting_dict):
    max = -100 
    most = ""
    for senator in voting_dict.keys():
         if senator == sen:
             continue
         sim = policy_compare(sen, senator, voting_dict)
         if (sim > max):
             max = sim
             most = senator
    return most
    

## Task 4

def least_similar(sen, voting_dict):
    min = 100 
    least = ""
    for senator in voting_dict.keys():
         if senator == sen:
             continue
         sim = policy_compare(sen, senator, voting_dict)
         if (sim < min):
             min = sim
             least = senator
    return least
    

## Task 5

most_like_chafee = most_similar("Chafee", create_voting_dict())
least_like_santorum = least_similar("Santorum", create_voting_dict())

# Task 6

def find_average_similarity(sen, sen_set, voting_dict):
    average = 0
    for senator in sen_set:
        if senator == sen:
            continue
        sim = policy_compare(sen, senator, voting_dict)
        average += sim
    return average/len(sen_set)


def get_dem_set():
    s = set()
    for line in voting_data:
        if line.split()[1] == 'D':
            s.add(line.split()[0])
    return s

def find_most_average_Democrat():
    d = create_voting_dict()
    average = -100
    most = ""
    for senator in d.keys():
        avg_sim = find_average_similarity(senator, get_dem_set(),d)
        if  avg_sim > average:
            average = avg_sim
            most = senator
    return senator
        
most_average_Democrat = find_most_average_Democrat() # give the last name (or code that computes the last name)

# Task 7

def find_average_record(sen_set, voting_dict):
    l = []
    for senator in sen_set:
        if len(l) == 0:
            l = voting_dict[senator]
        else:
            for i in range(len(l)):
                l[i] += voting_dict[senator][i]

    for i in range(len(l)):
        l[i] = l[i] / len(sen_set)

    return l

average_Democrat_record = find_average_record(get_dem_set(), create_voting_dict()) 

# Task 8

def bitter_rivals(voting_dict):
    """
    Input: a dictionary mapping senator names to lists representing
           their voting records
    Output: a tuple containing the two senators who most strongly
            disagree with one another.
    Example: 
        >>> voting_dict = {'Klein': [-1,0,1], 'Fox-Epstein': [-1,-1,-1], 'Ravella': [0,0,1]}
        >>> bitter_rivals(voting_dict)
        ('Fox-Epstein', 'Ravella')
    """
    senator_pairs = [ (x,y) for x in voting_dict.keys() for y in voting_dict.keys() if x != y ]
    dis_sim = 100
    dis_pair = ("","")
    for pair in senator_pairs:
        sim = policy_compare(pair[0], pair[1], voting_dict)
        if sim < dis_sim :
            dis_sim = sim
            dis_pair = pair
    return dis_pair

