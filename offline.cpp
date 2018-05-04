#include <string>
#include <string.h>
#include <vector>
#include <unistd.h>
#include <string.h>
#include <iostream>
#include <stdio.h>
#include <fstream>
#include <vector>

#include "avltree.hpp"

using namespace std;

vector<docDetails> docList;

//	global root
struct avl_node *root = NULL;

void printTextUntilTag(ifstream &in, int offset) {
	
	string buffer;
	
	in.clear();
	in.seekg(offset, ios::beg);
	
	while (getline(in, buffer)) {
		if (buffer[0] == '$') break;
		cout << buffer << endl;
	}
}

void printDocList(vector<docDetails> docList) {
	
	ifstream input;
	input.open(fullTestPath);

	for ( auto i = docList.begin(); i != docList.end(); i++ ) {
	    std::cout << "Doc id " + i->docID << std::endl;
		
	    std::cout << "Title offset " + to_string(i->titleOffset) << std::endl;
		printTextUntilTag(input, i->titleOffset);
		
	    std::cout << "Body offset " + to_string(i->bodyOffset) << std::endl;
		printTextUntilTag(input, i->bodyOffset);	
	}
}

avlTree parseDocumentText(string docPath) {

	//	Stores terms
	avlTree avl;

	//	Document dictionary
	string docID = "NaN";
	int docNo = -1;	//	documents start at 0
	unsigned int titleOffset = 0;
	unsigned int bodyOffset = 0;

	//	for parsing
	string line;
	ifstream in;

	in.open(docPath);

	if (in.is_open()) {

		while(getline(in, line)) {

			if (line[0] == '$') {
				if (line[1] == 'D') {	//	$DOC tag detected

					std::istringstream iss(line);
				    std::string token;

				    std::getline(iss, token, ' ');	//	meta tag
				    std::getline(iss, token, ' ');	//	docid
				    docID = token;
					docNo++;
				}
				else if (line[1] == 'B') {	//	$BODY
					bodyOffset = (int) in.tellg();

					//	record document to dictionary in body code block
					//	since body is the last element to be read
					struct docDetails newDocDetails;
					newDocDetails.docID = docID;
					newDocDetails.titleOffset = titleOffset;
					newDocDetails.bodyOffset = bodyOffset;

					docList.push_back(newDocDetails); 
				}
				else if (line[1] == 'T') {	//	$TITLE
					titleOffset = (int) in.tellg();
				}
			}
			else {	//	it is a term, so insert it

				//	tokenize
				std::istringstream iss(line);
			    std::string token;
			    while (std::getline(iss, token, ' '))
			    {
			        root = avl.insert(root, token, docNo);
			    }
			}
		}

		return avl;
	}
	else {
		cout << "Error! Could not open a document file." << endl;
		return avl;
	}
}

void generateDocID() {
	ofstream out(doc);

	out << to_string(docList.size()) + "\n";

	for (unsigned int i = 0; i < docList.size(); i++) {

		out << docList[i].docID + " " + to_string(docList[i].titleOffset) + " " + to_string(docList[i].bodyOffset) + "\n";
	}
}

int main()
{

	for (int i = 1; i <= 1; i++) {	

		//	for loop and strings are just to iterate through document files
		string doc = test + "_" + to_string(i);
		avlTree avl = parseDocumentText(doc);

		//	print avl tree to files
		avl.generateDictionary(root);
		avl.generatePostings(root);
		generateDocID();
	}

	return 0;
}
