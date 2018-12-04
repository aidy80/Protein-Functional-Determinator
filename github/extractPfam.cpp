/*
 * Program to find each pfam and the sequences that it occurs in. These 
 * sequences are then accordingly sent to the directory pFam
 */

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <tuple>
#include <limits.h>

using namespace std;

#define MAXLABEL 100000000
#define MAXPROT 1000000
#define LABELSTART 1
#define PROTSTART 0

struct protein {
	string name;
	string seq;
	int start;
	int end;
};

typedef basic_istringstream<char> istringstream;
typedef tuple<string, string, int, int > pfamProtein;
typedef tuple<string, string, vector<int>, vector<int> > combinedPfamProtein;
typedef struct protein protein;

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

//Go through the protein/pfam relationship file and 
//add the data for each protein/pfam relationsion to a vector of pfamProtein
//structs
vector<pfamProtein> getProteinLabels() {
    ifstream relations;
    relations.open("/Volumes/My Passport/Pfam-A.regions.tsv");

    if (!relations.is_open()) {
        cerr << "relations WHOOPS!";
    }

	vector<pfamProtein> proteinLabels;

	int count = 0;
    string line; 
    while (getline(relations, line) && count <= MAXLABEL) {
		if (count >= LABELSTART) {
			vector<string> words = split(line, '	');
			int start = stoi(words.at(5));
			int end = stoi(words.at(6));
			proteinLabels.push_back(make_tuple(words.at(0), words.at(4), start, end));
		}
		count++;
    }

	relations.close();

	return proteinLabels;
}

//Go through the protein/pfam relationships and combined them such that there
//is a unique protein in each entry, no repeats. To do so, each protein may
//have many pfams
vector<combinedPfamProtein> mergeRepeats(vector<pfamProtein> currLabels) {
	vector<combinedPfamProtein> newLabels;
	string lastProtein = "";
	string lastLabel = "";
	vector<int> lastStart;
	vector<int> lastEnd;
	for (pfamProtein relation: currLabels) {
		if (lastProtein.compare(get<0>(relation)) == 0) {
			lastLabel += " ";
			lastLabel += get<1>(relation);
			lastStart.push_back(get<2>(relation));
			lastEnd.push_back(get<3>(relation));
		} else {
			if (lastLabel != "") 
				newLabels.push_back(make_tuple(lastProtein, lastLabel, lastStart, lastEnd));

			lastStart.clear();
			lastEnd.clear();

			lastProtein = get<0>(relation);
			lastLabel = get<1>(relation);
			lastStart.push_back(get<2>(relation));
			lastEnd.push_back(get<3>(relation));
		}
	}
	newLabels.push_back(make_tuple(lastProtein, lastLabel, lastStart, lastEnd));

	return newLabels;
}

//Go through all of the protein sequences and get is so that there is a
//vector of data relating the idea that each pfam label is directly linked with 
//many different protein sequences
unordered_map<string, vector<protein> > getAllPfams(
									vector<combinedPfamProtein>& proteinLabels) {
	unordered_map<string, vector<protein>> allPfams;

	ifstream proteinFile;
	proteinFile.open("/Volumes/My Passport/pfamseq");

	if (!proteinFile.is_open()) {
        cerr << "protein file WHOOPS!";
    }

	string line;
	string proteinName;

	int numProtein = 0;
	int count = 0;
	bool trueMatch = false;
	while (getline(proteinFile, line) && count <= MAXPROT) {
		if (line.substr(0, 1) == ">") {
			proteinName = line.substr(1, line.find(" ") - 3);
			if (!proteinName.compare(get<0>(proteinLabels.at(numProtein)))) {
				trueMatch = true;
			}
		}
		else if (trueMatch) {
			string currProteinName = get<0>(proteinLabels.at(numProtein));
			vector<string> labels = split(get<1>(proteinLabels.at(numProtein)), ' ');
			vector<int> start = get<2>(proteinLabels.at(numProtein));
			vector<int> end = get<3>(proteinLabels.at(numProtein));
			for (size_t i = 0; i < labels.size(); i++){
				protein newProtein;
				newProtein.name = currProteinName;
				newProtein.seq = line;
				newProtein.start = start.at(i);
				newProtein.end = end.at(i);
				allPfams[labels.at(i)].push_back(newProtein);
			}
			numProtein++;
			trueMatch = false;
		}

		count++;
	}

	proteinFile.close();

	return allPfams;
}

//Output to files the relationships between proteins and pfams. Each file
//represents a given pfam and the protein sequences that it contains
void outputFiles(unordered_map<string, vector<protein> > &allPfams) {
	for (auto it = allPfams.begin(); 
					it != allPfams.end(); it++) {
		const string path = "../bigFiles/pFams/" + (string)it->first;
		ofstream newFile(path);
		newFile << it->first << "\n";
		for (protein currProtein: it->second) {
			newFile << currProtein.name << " " << currProtein.seq << " " 
						<< currProtein.start << " " << currProtein.end << "\n";	
		}
	}
}

//Main to collect information about the relationships between pfams and the
//sequences that they occur in. Data is then written in many files in the
//directory pfams.
int main() {
	vector<pfamProtein> proteinLabels = getProteinLabels();

	sort(proteinLabels.begin(), proteinLabels.end());
	vector<combinedPfamProtein> combinedProteinLabels = mergeRepeats(proteinLabels);

	unordered_map<string, vector<protein> > allPfams = getAllPfams(combinedProteinLabels);

	outputFiles(allPfams);

	return 0;
}
