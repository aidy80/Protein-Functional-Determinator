
# Methods and testing main to measure diversity of a set of protein sequences
# given pfam and GO identifiers; calculates average n-gram similarity of all
# pairs of sequences (similarity calculated using Jaccard index)
# 
# Note: We can modify this to measure diversity of only a subset if it's
# taking too long to find diversity of entire sequence sets

from nltk.util import ngrams
import numpy as np

# Input:    List of sequences, integer n
# Output:   List of sequences represented as n-grams
# 
# Divides each sequence into n-grams and returns list of n-gram representations

def getNgrams (seqs, n):

    ngLists = []

    for seq in seqs:
        ngLists.append(list(ngrams(seq, n)))

    return ngLists

# Input:    Pair of lists
# Output:   Jaccard similarity
# 
# Calculates the Jaccard similarity between two lists, defined as the size of 
# the intersection of the two lists divided by the size of the union (always a
# value from 0.0 to 1.0)

def jaccard (s1, s2):

    intersection = set(s1).intersection(s2)
    union = set(s1).union(s2)
    jaccardScore = (float(len(intersection)) / float(len(union)))

    return jaccardScore

# Input:    List of sequences, integer n
# Output:   Average similarity of set of sequences
# 
# For each pair of distinct sequences, calculates the Jaccard similarity
# between the n-gram representations of each sequence; calculates and returns
# the average Jaccard similarity across the whole set

def diversity (seqs, n):

    ngLists = getNgrams(seqs, n)
    length = len(seqs)
    scores = []

    for i in range(0, length):
        for j in range(0, length):
            if (i != j):
                score = jaccard(ngLists[i], ngLists[j])
                scores.append(score)

    return np.mean(scores)

# Input:    Pfam and GO identifiers, pfam exclusion variable
# Output:   Diversity of sequences
# 
# Takes a pfam and GO identifier and gathers all sequences matching both
# identifiers; calculates and returns diversity of set of sequences with pfams
# removed if excludePfam is true, diversity of set of whole sequences otherwise

def testMain (pf, go, excludePfam):

    with open("github/" + pf + go) as inFile:

        seqs = []

        for line in inFile.readlines():

            line = line.split()
            pfamStart = int(line[2])
            pfamEnd = int(line[3])

            if (excludePfam):
                seq = line[1][:pfamStart] + line[1][pfamEnd:]
            else:
                seq = line[1]

            seqs.append(seq)

    div = diversity(seqs, 3)
    print div

testMain("PF03265", "GO0004531", True)