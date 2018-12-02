//Function to parse through given pfam files and return a 2D list with the
//protein sequence information

#include <string>
#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <unordered_map>

using namespace std;

struct proteinSeq {
	string name;
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
			newSeq.name = data.at(0);
			newSeq.seq = data.at(1);
			newSeq.start = stoi(data.at(2));
			newSeq.end = stoi(data.at(3));
			currSeqs.push_back(newSeq);
		}
	}

    return currSeqs;
}

vector<proteinSeq>  parsePfamGo(string pfamLabel, string goLabel, unordered_map<string, vector<string> > GoSeq) {
	(void) goLabel;
	(void) GoSeq;
    string filename = "pFams/" + pfamLabel;
	vector<proteinSeq> currSeqs;
	ifstream proteinFile;
	proteinFile.open(filename);
	string currLine;
	while(getline(proteinFile, currLine)) {
		vector<string> data = split(currLine, ' ');
		if (data.size() != 1) {
			//if (GoSeq.contains(data.at(0)) && find(GoSeq[data.at(0)].begin(), 
			//			GoSeq[data.at(0)].end(), goLabel) != GoSeq[data.at(0)].end()) {
			if (find(GoSeq[data.at(0)].begin(), 
						GoSeq[data.at(0)].end(), goLabel) != GoSeq[data.at(0)].end()) {
				proteinSeq newSeq;
				newSeq.name = data.at(0);
				newSeq.seq = data.at(1);
				newSeq.start = stoi(data.at(2));
				newSeq.end = stoi(data.at(3));
				currSeqs.push_back(newSeq);
			}
		}
	}

    return currSeqs;
}

unordered_map<string, vector<string> > parseGOLabels() {
    unordered_map<string, vector<string> > GoSeq;
	string filename = "goa_human.gaf";
	ifstream proteinFile;
	proteinFile.open(filename);
	string currLine;
	while(getline(proteinFile, currLine)) {
		vector<string> data = split(currLine, ' ');
        if (data.at(0) == "UniProtKB") {
			GoSeq[data.at(1)].push_back(data.at(2));
		}
	}
        
    return GoSeq;
}
    

//EXAMPLE!
int main() {
	unordered_map<string, vector<string> > GoSeq = parseGOLabels();

	/*
    vector<proteinSeq> currSeqs = parsePfam("PF04526");
    for (size_t i = 0; i < currSeqs.size(); i++) {
        cout << "Protein sequence name: " << currSeqs.at(i).name << "\n";
        cout << "Entire protein sequence: " << currSeqs.at(i).seq << "\n";
        cout << "Pfam starting Position: " << currSeqs.at(i).start << "\n";
        cout << "Pfam ending Position: " << currSeqs.at(i).end << "\n\n";
	}
	*/
	vector<proteinSeq> currSeqs = parsePfamGo("PF04526", "GO:0004531", GoSeq);
    for (size_t i = 0; i < currSeqs.size(); i++) {
        cout << "Protein sequence name: " << currSeqs.at(i).name << "\n";
        cout << "Entire protein sequence: " << currSeqs.at(i).seq << "\n";
        cout << "Pfam starting Position: " << currSeqs.at(i).start << "\n";
        cout << "Pfam ending Position: " << currSeqs.at(i).end << "\n\n";
	}
	return 0;
}
