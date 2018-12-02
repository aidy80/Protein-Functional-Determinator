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

def parsePfamGo(pfamLabel, goLabel, GoSeq):
    filename = "pFams/%s" % (pfamLabel)
    currSeqs = []
    with open(filename, "r") as currFamFile:
        for line in currFamFile:
            data = line.split()
            if (len(data) != 1):
                if (data[0] in GoSeq.keys() and goLabel in GoSeq[data[0]]):
                    data[2] = int(data[2])
                    data[3] = int(data[3])
                    currSeqs.append(data)

    return currSeqs



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
    



#EXAMPLE!
def testMain():
    """
    currSeqs = parsePfam("PF04526")
    for seq in currSeqs:
        print "Protien name: ", seq[0]
        print "Entire protein sequence: ", seq[1]
        print "Pfam starting position: ", seq[2]
        print "Pfam ending position: ", seq[3], "\n"
        """

    GoSeq = parseGOLabels()
    currSeqs = parsePfamGo("PF03265", "GO:0004531", GoSeq)
    for seq in currSeqs:
        print "Protien name: ", seq[0]
        print "Entire protein sequence: ", seq[1]
        print "Pfam starting position: ", seq[2]
        print "Pfam ending position: ", seq[3], "\n"

testMain()
