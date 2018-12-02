import math

with open("PF00024.txt") as textFile:
        data = [line.split() for line in textFile]


#Probability function 
#Input:  String of protein characters
#Output: 3 x n List (matrix) containing  (character, number of occurences,length of string)
def prob(seq):
  matrix = [[]]
  duplicate  = []
  length = float(len(seq))
  for i in seq:
    if i in duplicate:
      pass
    else:
      count = float(seq.count(i))
      matrix.append((i,count,len(seq)))
      duplicate.append(i)
  del matrix[0]
  matrix.sort()
  return matrix


#Divergence function 
#Input: (2) a 3 x n matrix containing (character, number of occurences,length of string)
#Output: the K-L divergence of the two sequences represented as a float

def div(matrixP,matrixQ):
  distance = 0.0
  for i in range(len(matrixP)):
    for j in range(len(matrixQ)):
      if matrixP[i][0] == matrixQ[j][0]:
        P_i = matrixP[i][1] / matrixP[i][2]
        Q_i = matrixQ[j][1] / matrixQ[j][2]
        P_Q = P_i / Q_i 
        distance += P_i * math.log10(P_Q)
      else:
        distance += 0 
  return distance


#function that takes as input two strings and returns as output the k-l divergence
def dist(string1,string2):
  matrix1 = prob(string1) 
  matrix2 = prob(string2)
  d = div(matrix1,matrix2)
  return d

distance1 = 0.0
for i in range(len(data)- 1):
  for j in range(len(data) - 1):
    if i == j:
      pass
    else:
      divergence = dist(data[i][0],data[j][0])
      distance1 += divergence

print distance1



