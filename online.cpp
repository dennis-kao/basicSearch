#include <string>
#include <string.h>
#include <vector>
#include <unistd.h>
#include <string.h>
#include <iostream>
#include <stdio.h>
#include <fstream>
#include <vector>
#include <math.h>
#include <map>

#include "avltree.hpp"

using namespace std;

struct dictNode {

	string term;
	
	/*	not an offset in terms of bytes
		it is the number of lines */
	unsigned int postingOffset;
};

vector<dictNode> termPos;
vector<documentNode> postings;
vector<docDetails> docDict;

int nTerms, nPostings, nDocs;

void parseDictionaryFile() {
	ifstream in;
	in.open(dict);
	string buffer;
	unsigned int postingOffset = 0;

	getline(in, buffer);	//header
	nTerms = stoi(buffer, nullptr, 10);	//	not used since I just read until EOF

	while(getline(in, buffer)) {
		std::istringstream iss(buffer);
	    std::string token, term;
	    int freq;
	    struct dictNode newNode;

	    std::getline(iss, token, ' ');
	    term = token;

	    std::getline(iss, token, ' ');
	    freq = stoi(token, nullptr, 10);

	    newNode.term = term;
	    newNode.postingOffset = postingOffset;

	    termPos.push_back(newNode);
	    postingOffset += freq;
	}

	// for (int i = 0; i < termPos.size(); i++) {
	// 	cout << termPos[i].term + " " + to_string(termPos[i].postingOffset) << endl;
	// }
}

void parsePostingsFile() {
	ifstream in;
	in.open(post);
	string buffer;

	getline(in, buffer);
	nPostings = stoi(buffer, nullptr, 10); //	not used since I just read until EOF

	while(getline(in, buffer)) {
		std::istringstream iss(buffer);
	    std::string token, term;
	    unsigned int freq;
	    int docNo;

	    struct documentNode newNode;

	   	std::getline(iss, token, ' ');
	   	docNo = stoi(token, nullptr, 10);

	   	std::getline(iss, token, ' ');
	   	freq = stoi(token, nullptr, 10);

	   	newNode.docNo = docNo;
	   	newNode.freq = freq;

	   	postings.push_back(newNode);
	}
}

void parseDocIDFile() {

	ifstream in;
	in.open(doc);

	string buffer;

	getline(in, buffer);
	nDocs = stoi(buffer, nullptr, 10); //	not used since I just read until EOF

	while(getline(in, buffer)) {
		std::istringstream iss(buffer);
	    std::string token, term;
	    struct docDetails newNode;

	   	std::getline(iss, token, ' ');
	   	newNode.docID = token;

	   	std::getline(iss, token, ' ');
	   	newNode.titleOffset = stoi(token, nullptr, 10);

	   	std::getline(iss, token, ' ');
	   	newNode.bodyOffset = stoi(token, nullptr, 10);

	   	docDict.push_back(newNode);
	}

	// for (int i = 0; i < docDict.size(); i++) {
	// 	cout << docDict[i].docID + " " + to_string(docDict[i].titleOffset)+ " " + to_string(docDict[i].bodyOffset) << endl;
	// }
}



int getTotalDocFreq(string keyword) {
	
	for(std::vector<dictNode>::iterator it = termPos.begin(); it != termPos.end(); ++it) {

		if (it->term == keyword) {
			if (it != termPos.end()) {
				vector<dictNode>::iterator next = it + 1;
				
				return next->postingOffset - it->postingOffset;
			}
			
			//	if this is the last element in the file, calculate the
			//	doc freq using: n_entries (in postings.txt) - postingOffset
			return nPostings - it->postingOffset;
		}
	}
	
	return 0;
}

vector<documentNode> getDocFreq(string keyword) {
	
	vector<documentNode> docs;
	
	for(std::vector<dictNode>::iterator it = termPos.begin(); it != termPos.end(); ++it) {

		if (it->term == keyword) {
			
			if (it != termPos.end()) {
				vector<dictNode>::iterator next = it + 1;
 				
				for (unsigned int i = it->postingOffset; i < next->postingOffset; i++) {
					
					docs.push_back(postings[i]);
				}
				
				return docs;
			}
			
			else {
				
				for (unsigned int i = it->postingOffset; i < postings.size(); i++) {
					
					docs.push_back(postings[i]);
				}
				
				return docs;
			}	
		}
	}
	
	return docs;	//	return an empty vector
}

double ** initializeMatrix(int colCount, int rowCount) {
	
	double ** t_matrix = new double*[rowCount];	//	2d matrix

	for (int i = 0; i < rowCount; i++) {
		double * newRow = new double[colCount];
		
		for (int j = 0; j < colCount; j++) newRow[j] = 0;
		
		t_matrix[i] = newRow;
	}
	
	return t_matrix;
}

void deleteMatrix(double ** t_matrix, int rowCount) {
	
	for (int i = 0; i < rowCount; i++) {
		delete(t_matrix[i]);
	}
	
	delete(t_matrix);
}

bool compareDocuments(std::pair<int, int> i, std::pair<int, int> j) {
	
	return (i.second < j.second);
}

string getText(ifstream &in, int offset) {
	
	string buffer;
	string output;
	
	in.clear();
	in.seekg(offset, ios::beg);
	
	while (getline(in, buffer)) {
		if (buffer[0] == '$') break;
		output = output + buffer + '\n';	//	add back the newline removed by getline()
	}
	
	return output;
}

int main() {

	string input = "", token;
	ifstream rawDocData(fullTestPath);
	
	parseDictionaryFile();
	parsePostingsFile();
	parseDocIDFile();

	while (1) {
		
		vector<double> queryWeighting;
		double ** t_matrix;	//	2d matrix
		map<string, int> queryKeywordCount;	//	keeps track of query keyword count
		vector<double> similarity;
		int keywordCounter = 0;
		map<string, int>::iterator it;
		std::vector<std::pair<int, double>> results;

		cout << "Enter a space delimited query, or type \"quit\" or \"q\" to quit." << endl;
		getline(cin, input);
		
		if (input == "q" || input == "quit") {
			break;
		}

		//	process query and count the number of terms
		istringstream iss(input);
		while (std::getline(iss, token, ' '))
		{
		    if (queryKeywordCount.count(token) > 0) {
				//	keyword exists, increment the count
				queryKeywordCount[token] += 1;
			}
			else {
				//	initialize keyword count to 1
				queryKeywordCount[token] = 1;
			}
		}
		
		t_matrix = initializeMatrix(queryKeywordCount.size(), nDocs);
				
		//	fill in the matrix
		for(it = queryKeywordCount.begin(), keywordCounter = 0; it != queryKeywordCount.end(); ++it, ++keywordCounter) {
			
			string kw = it->first;
			vector<documentNode> documentFreqs = getDocFreq(kw);
			unsigned int docFreqSize = documentFreqs.size();
			
			if (docFreqSize != 0) {	//	keyword is in doc
				double termSpecificity = log(nDocs / docFreqSize);
			
				//	now calculate values for each document and place in matrix
				for (unsigned int i = 0; i < docFreqSize; i++) {				
					struct documentNode newNode = documentFreqs[i];
					t_matrix[newNode.docNo][keywordCounter] = newNode.freq * termSpecificity;
				}
				
				//	also, calculate the weightings for the query
				queryWeighting.push_back(it->second * termSpecificity);
			}
			else {	//	not in doc
				//	matrix already initialized to 0
				queryWeighting.push_back(0);
			}
		}
		
		//	now compare the similarity to the query using the inner product
		for (int i = 0; i < nDocs; i++) {
			double innerProduct = 0;
			
			for (unsigned int j = 0; j < queryKeywordCount.size(); j++) {
				innerProduct += queryWeighting[j] * t_matrix[i][j];
			}
			
			similarity.push_back(innerProduct);
		}
		
		deleteMatrix(t_matrix, nDocs);
		
		//	sort and display results
		for (unsigned int i = 0; i < similarity.size(); i++) {
			if (similarity[i] != 0) results.push_back(make_pair(i, similarity[i]));
		}
		
		sort(results.begin(), results.end(), compareDocuments);
		
		if (results.size() > 0) {
			
			bool validResponse = false;
			unsigned int docNoInput;
			
			while (validResponse == false) {
				
				cout << "Select a document from the numbered list: " << endl;
				
				for (unsigned int i = 0; i < results.size(); i++) {
					struct docDetails offsetValues = docDict[results[i].first];
					string title = getText(rawDocData, offsetValues.titleOffset);
					//string body = getText(rawDocData, offsetValues.bodyOffset);
					
					cout << to_string(i+1) + ". " + title;
					//cout << "Body:\n" + body << endl;
				}
				
				cout << "Your selection: ";
				cin >> docNoInput;
				
				if (docNoInput <= 0 || docNoInput >= results.size() + 1) {
					cout << "Invalid input! Please try again!" << endl;
				}
				else {
					validResponse = true;
					
					struct docDetails offsetValues = docDict[results[docNoInput-1].first];
					string body = getText(rawDocData, offsetValues.bodyOffset);
					cout << "Body:\n\n" + body;
				}
			}
			
			break;
		}
		else {
			cout << "No results found :( Try another query?" << endl;
		}
	}

	return 0;
}