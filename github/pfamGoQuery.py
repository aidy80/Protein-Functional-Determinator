import os

#Function to parse through given pfam files and return a 2D list with the
#protein sequence information

#Takes a pfamLabel (string with the pfam filename. i.e. PF04526). This
#method will then parse the file and return a 2D list corresponding to the
#file
#
#In the returned 2D list, each entry (sublist) corresponds to a single
#protein. It will have the proteins sequence (string form) in the 0th
#index, followed by the starting location of the pfam itself in the 1st
#index and the ending location of the pfam in the 2nd index.
#
#See test main below for an example
#
#Let Aidan know if you have any questions
def parsePfam(pfamLabel):
    filename = "pFams/%s" % (pfamLabel)
    currSeqs = []
    with open(filename, "r") as currFamFile:
        for line in currFamFile:
            data = line.split()
            if (len(data) != 1):
                data[2] = int(data[2])
                data[3] = int(data[3])
                currSeqs.append(data)

    return currSeqs

#Go through go file and return all of the GO labels that it contains in a list
def parseGo(protLabel):
    filename = "../bigFiles/goLabels/xaa/%s" % (protLabel)
    if not os.path.exists(filename):
        print protLabel + " Does not exist"
        return []
    with open(filename, "r") as goFile:
        for line in goFile:
            print line.split()
            return line.split()

#Query for all protein sequences with a given pfam domain in addition to a
#specific go label. Returns a 2D list. This is a list of, essentially, structs
#which each represent a certain protein sequence. This struct has the protein's
#name, followed by its amino acid sequence, then the starting and ending
#indexes of the pfam domain itself.
def parsePfamGo(pfamLabel, goLabel):
    filename = "../bigFiles/pFams/%s" % (pfamLabel)
    currSeqs = []
    if not os.path.exists(filename):
        print pfamLabel + "Does not exist"
        return
    with open(filename, "r") as currFamFile:
        for line in currFamFile:
            data = line.split()
            if (len(data) != 1):
                goLabels = parseGo(data[0])
                if(goLabel in goLabels):
                    print "HERE"
                    data[2] = int(data[2])
                    data[3] = int(data[3])
                    currSeqs.append(data)
                else:
                    print goLabel + " not found\n"

    return currSeqs

#Go through all go labels and make them into a dictionary object. Key=protein
#name, value = list of all go labels
def parseGOLabels():
    GoSeq = {}
    with open("goa_human.gaf") as goFile:
        for line in goFile:
            data = line.split()
            if (data[0] == "UniProtKB"):
                if data[1] not in GoSeq.keys():
                    GoSeq[data[1]] = [data[3]]
                else:
                    GoSeq[data[1]].append(data[3])
        
    return GoSeq
    
#Write everything in a currSeqs 2D list (created in parsePfamGo) into a file
def writeResults(filename, currSeqs):
    with open(filename, "w") as resultFile:
        for protein in currSeqs:
            resultFile.write(protein[0] + " " + protein[1] + " " + \
                               str(protein[2]) + " " + str(protein[3]) + "\n")


#EXAMPLE!
def testMain():
    pf = "PF03265"
    go = "GO:0004531"
    currSeqs = parsePfamGo(pf, go)
    for seq in currSeqs:
        print "Protien name: ", seq[0]
        print "Entire protein sequence: ", seq[1]
        print "Pfam starting position: ", seq[2]
        print "Pfam ending position: ", seq[3], "\n"
    writeResults("results/" + pf + go, currSeqs)

testMain()
