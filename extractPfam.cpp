//Function to parse through given pfam files and return a 2D list with the
//protein sequence information

#include <string>
#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>

using namespace std;

struct proteinSeq {
	string seq;
	int start;
	int end;
};

typedef struct proteinSeq proteinSeq;

//split a large stringed sentence into a vector of words
vector<string> split(string& s, char delimiter) {
    vector<string> words;
    string currWord;
    basic_istringstream<char> wordStream(s);
    while (getline(wordStream, currWord, delimiter)) {
        words.push_back(currWord); 
    }
    return words;
}



/*
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
*/
vector<proteinSeq> parsePfam(string pfamLabel) {
    string filename = "pFams/" + pfamLabel;
    vector<proteinSeq> currSeqs;
	ifstream proteinFile;
	proteinFile.open(filename);
	string currLine;
	while(getline(proteinFile, currLine)) {
		vector<string> data = split(currLine, ' ');
		if (data.size() != 1) {
			proteinSeq newSeq;
			newSeq.seq = data.at(0);
			newSeq.start = stoi(data.at(1));
			newSeq.end = stoi(data.at(2));
			currSeqs.push_back(newSeq);
		}
	}

    return currSeqs;
}

//EXAMPLE!
int main() {
    vector<proteinSeq> currSeqs = parsePfam("PF04526");
    for (size_t i = 0; i < currSeqs.size(); i++) {
        cout << "Entire protein sequence: " << currSeqs.at(i).seq << "\n";
        cout << "Pfam starting Position: " << currSeqs.at(i).start << "\n";
        cout << "Pfam ending Position: " << currSeqs.at(i).end << "\n\n";
	}
	return 0;
}
