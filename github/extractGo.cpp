//Function to parse through given pfam files and return a 2D list with the
//protein sequence information

#include <string>
#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <unordered_map>

using namespace std;

//vector<string> chunks{"xab", "xac", "xad", "xae", "xaf", "xag", "xah"};
vector<string> writeChunks{"xab"};
vector<string> readChunks{"xaa"};

//Result struct. Represents a given protein sequence where a pfam
//is located. 
struct proteinSeq {
	string name; //pfamseq_acc 
	string seq; //Amino acid sequence
	int start; //start index of pfam domain
	int end; //end index of pfam domain
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
 * Parse a goFile and return a vector filled with all of the go labels it
 * contains
 */
vector<string> parseGo(string protLabel) {
	ifstream goFile;
	int tries = (int)readChunks.size() - 1;
	while (!goFile.is_open() && tries >= 0) {
		goFile.open("../bigFiles/goLabels/" + readChunks.at(tries) + "/" + protLabel);
		tries--;
	}
	if (!goFile.is_open()) {
		cerr << "Could not find sequence " + protLabel + "\n";	
	}

	string labels;
	getline(goFile, labels);
	vector<string> protLabels = split(labels, ' ');
	goFile.close();

	return protLabels;
}

/* 
 * Run a query - specifying a pfam and a GO function. A vector of protein
 * sequences structs will be returned representing the sequences that have both
 * the pfam and the GO function
 */
vector<proteinSeq>  parsePfamGo(string pfamLabel, string goLabel) {
    string filename = "../bigFiles/pFams/" + pfamLabel;
	vector<proteinSeq> currSeqs;
	ifstream proteinFile;
	proteinFile.open(filename);
	if (!proteinFile.is_open())	{
		cout << "Could not find " << filename << "\n";
	}

	string currLine;
	ifstream goFile;
	while(getline(proteinFile, currLine)) {
		vector<string> data = split(currLine, ' ');
		if (data.size() != 1) {
			vector<string> goLabels = parseGo(data.at(0));
			if (find(goLabels.begin(), goLabels.end(), goLabel) != goLabels.end()) {
				proteinSeq newSeq;
				newSeq.name = data.at(0);
				newSeq.seq = data.at(1);
				newSeq.start = stoi(data.at(2));
				newSeq.end = stoi(data.at(3));
				currSeqs.push_back(newSeq);
			}
		}
	}

	proteinFile.close();

    return currSeqs;
}

/*
 * Go through all GO/sequence relationships and write to files all of the
 * information that this file contains. See the goLabels directory for the
 * results of this function
 */
void parseGOLabels(string filename, string outDir) {
	ifstream allGo;
	allGo.open(filename);
	if (!allGo.is_open()) {
		cout << filename << " did not exist" << "\n";	
	}

	string currLine;
	string currProtein = "";
	ofstream goFile;
	while(getline(allGo, currLine)) {
		vector<string> data = split(currLine, '\t');
        if (!data.at(0).compare("UniProtKB")) {
			if (!(!currProtein.compare(data.at(1)))) { //If they are not equal
				if (goFile.is_open()) {
					goFile.close();
				}
				currProtein = data.at(1);	
				goFile.open(outDir + "/" + data.at(1));
			}
			goFile << data.at(4) << " ";
		}
	}

	allGo.close();
}

void writeResults(string filename, vector<proteinSeq> currSeqs) {
	ofstream newResult;
	newResult.open(filename);
	for (proteinSeq currProtein : currSeqs) {
		newResult << currProtein.name << " " << currProtein.seq << " " 
						<< currProtein.start << " " << currProtein.end << "\n";
	}
	if (newResult.is_open()) {
		cout << "Writing of " << filename << " was successful!\n";	
	} else {
		cout << "Writing of " << filename << " was a failure :(\n";	
	}
	newResult.close();
}

//EXAMPLE main!
int main() {
	for (string chunk : writeChunks) {
		parseGOLabels("/Volumes/My Passport/" + chunk, "../bigFiles/goLabels/" + chunk);
	}

	vector<string> allPfam{"03261", "03265"};
	vector<string> allGo{"0016534", "0004531"};

	for (size_t i = 0; i < allPfam.size(); i++) {
		vector<proteinSeq> currSeqs = parsePfamGo("PF" + allPfam.at(i),"GO:" + allGo.at(i));
		writeResults("results/PF" + allPfam.at(i) + "GO" + allGo.at(i), currSeqs);
	}
		return 0;
}
