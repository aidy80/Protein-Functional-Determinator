import csv
from numpy import mean

### Parses PFam files in order to prep for Clustal Alignment
def getseq():
    protein_fam = "PF00001"
    filename = "pFams/"+protein_fam
    outputfile = "Parsed_"+protein_fam+".txt"
    sequence_list = []
    with open(filename) as fp:
        line = fp.readline()
        while line:
            line = fp.readline()
            sequence = line.split(" ")
            sequence_list.append(sequence[0])
    f = open(outputfile, 'w')
    count = 0
    for seq in sequence_list:
        if seq != " " and count < 4000:
            f.write("\n>Protein"+str(count)+"\n")
            f.write(seq)
            count += 1

if __name__ == '__main__':
    getseq()