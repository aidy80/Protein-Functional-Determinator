
import csv
from numpy import mean

PFAM = 1
CONS = 2

# read CODACrelations.csv to compile consensus scores for each pfam-GO
# association; compute mean consensus score for each pfam (across all
# relevant associations) and return database of pfams and mean scores

def getScores ():

    # dictionary for each pfam

    pfamScores = {}

    # read through all associations and compile lists of CODAC scores
    # associated with each unique pfam

    with open("CODACrelations.txt", "rb") as inputFile:

        # turn file to 2D list of data; shave off header row

        associations = list(csv.reader(inputFile, delimiter=";"))
        associations = associations[1:]

        for row in associations:

            pfam = row[PFAM]
            consensusScore = float(row[CONS])

            if (pfam in pfamScores):
                pfamScores[pfam].append(consensusScore)
            else:
                pfamScores[pfam] = [consensusScore]

    # average all scores associated with each pfam and edit dictionary

    for pfam in pfamScores:
        scores = pfamScores[pfam]
        meanScore = mean(scores)
        pfamScores[pfam] = meanScore

    return pfamScores

# read pfam-score association dictionary; output to file for later reading

def outputScores (pfamScores):

    with open("pfamCODACscores.csv", "wb") as outputFile:

        writer = csv.writer(outputFile, delimiter=",")

        for pfam in pfamScores:
            row = [pfam, pfamScores[pfam]]
            writer.writerow(row)

# driver

if __name__ == '__main__':

    pfamScores = getScores()
    outputScores(pfamScores)