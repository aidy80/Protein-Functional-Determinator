class extractPfam():
    def __init__(self):
        self.allSeqs = {}

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
    def parsePfam(self, pfamLabel):
        filename = "pFams/%s" % (pfamLabel)
        currSeqs = []
        with open(filename, "r") as currFamFile:
            for line in currFamFile:
                data = line.split()
                if (len(data) != 1):
                    data[1] = int(data[1])
                    data[2] = int(data[2])
                    currSeqs.append(data)

        return currSeqs

def testMain():
    test = extractPfam()
    currSeqs = test.parsePfam("PF04526")
    for seq in currSeqs:
        print "Entire protein sequence: ", seq[0]
        print "Pfam starting position: ", seq[1]
        print "Pfam ending position: ", seq[2], "\n"

testMain()
