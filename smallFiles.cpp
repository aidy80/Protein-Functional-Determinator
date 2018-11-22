#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <tuple>

using namespace std;

#define MAXLABEL 100000000
#define MAXPROT 10000000
#define LABELSTART 0
#define PROTSTART 0

struct pfam {
	string label;
	vector<string> proteins;
};

typedef basic_istringstream<char> istringstream;
typedef struct pfam pfam;

vector<string> split(string& s, char delimiter) {
    vector<string> words;
    string currWord;
    basic_istringstream<char> wordStream(s);
    while (getline(wordStream, currWord, delimiter)) {
        words.push_back(currWord); 
    }
    return words;
}

void writeFile(pfam *currFam) {
	const string path = "pFams/" + currFam->label;
	ofstream newFile(path);	
	for (size_t i = 0; i < currFam->proteins.size(); i++) {
		newFile << currFam->proteins.at((int)i) + "\n";
	}
}

vector< tuple<string, string> > getProteinLabels() {
    ifstream relations;
    relations.open("Pfam-A.regions.tsv");

    if (!relations.is_open()) {
        cerr << "relations WHOOPS!";
    }

	vector< tuple<string, string> > proteinLabels;

	int count = 0;
    string line; 
    while (getline(relations, line) && count <= MAXLABEL) {
		if (count >= LABELSTART) {
			vector<string> words = split(line, '	');
			proteinLabels.push_back(make_tuple(words.at(0), words.at(4)));
		}
		count++;
    }

	return proteinLabels;
}

unordered_map<string, vector<string> > getAllPfams(
							vector< tuple<string, string> >& proteinLabels) {
	unordered_map<string, vector<string>> allPfams;

	ifstream proteinFile;
	proteinFile.open("pfamseq");

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
		else if(trueMatch) {
			cout << get<0>(proteinLabels.at(numProtein)) << " "  << proteinName << "\n";
			vector<string> labels = split(get<1>(proteinLabels.at(numProtein)), ' ');
			for (string label : labels) {
				allPfams[label].push_back(line);
			}
			numProtein++;
			trueMatch = false;
		}

		count++;
	}

	return allPfams;
}

void outputFiles(unordered_map<string, vector<string> > &allPfams) {
	(void) allPfams;	
	for (auto it = allPfams.begin(); 
					it != allPfams.end(); it++) {
		const string path = "pFams/" + (string)it->first;
		ofstream newFile(path);
		newFile << it->first << "\n";
		for (string protein: it->second) {
			newFile << protein << "\n";	
		}
	}
}

vector<tuple<string, string> > mergeRepeats(vector<tuple<string, string> > currLabels) {
	vector<tuple<string, string> > newLabels;
	string lastProtein = "";
	string lastLabel = "";
	for (tuple<string, string> relation: currLabels) {
		if (lastProtein.compare(get<0>(relation)) == 0) {
			lastLabel += " ";
			lastLabel += get<1>(relation);
		} else {
			if (lastLabel != "") 
				newLabels.push_back(make_tuple(lastProtein, lastLabel));
			lastProtein = get<0>(relation);
			lastLabel = get<1>(relation);
		}
	}
	newLabels.push_back(make_tuple(lastProtein, lastLabel));

	return newLabels;
}

int main() {
	vector< tuple<string, string> > proteinLabels = getProteinLabels();

	sort(proteinLabels.begin(), proteinLabels.end());
	proteinLabels = mergeRepeats(proteinLabels);

	unordered_map<string, vector<string> > allPfams = getAllPfams(proteinLabels);

	outputFiles(allPfams);

	return 0;
}
