#define FILE_EXTENSION ".txt"
#include <fstream>
#include <filesystem>
#include <string>
#include <cstring>
#include <vector>
#include <algorithm>
#include <set>
#include <iostream>
#include "trie.hpp"
namespace fs = std::filesystem;
using namespace std;

// Utility Func
vector<string> getTextFiles(const string& directory) {
    vector<string> files;
    for(const auto& entry : fs::directory_iterator(directory)) {
        if(entry.path().extension() == FILE_EXTENSION) {
            files.push_back(entry.path().string());
        }
    }
    return files;
}

// string parser : output vector of strings (words) after parsing
vector<string> word_parse(vector<string> tmp_string){
	vector<string> parse_string;
	for(auto& word : tmp_string){
		string new_str;
    	for(auto &ch : word){
			if(isalpha(ch))
				new_str.push_back(ch);
		}
		parse_string.emplace_back(new_str);
	}
	return parse_string;
}

vector<string> split(const string& str, const string& delim) {
	vector<string> res;
	if("" == str) return res;

	char * strs = new char[str.length() + 1] ; 
	strcpy(strs, str.c_str());

	char * d = new char[delim.length() + 1];
	strcpy(d, delim.c_str());

	char *p = strtok(strs, d);
	while(p) {
		string s = p; 
		res.push_back(s);
		p = strtok(NULL, d);
	}

	return res;
}

set<int> find_set(string &str, Trie *trie){
	set<int> tmp;
	if(str[0] == '\"'){
		tmp = trie->find_exact(str.substr(1, str.size()-2));
	}
	else if(str[0] == '*'){
		tmp = trie->find_suf(str.substr(1, str.size()-2));
	}
	else if(isalpha(str[0])){
		tmp = trie->find_pre(str);
	}
	else if(str[0] == '<'){
		tmp = {0, 1};
	}
	return tmp;
}


int main(int argc, char *argv[]){

    // INPUT :
	// 1. data directory in data folder
	// 2. number of txt files
	// 3. output route

    string data_dir = argv[1] + string("/");
	string query = string(argv[2]);
	string output = string(argv[3]);

	// Open output file
	ofstream outputFile(output);
	if(!outputFile.is_open()){
		cout << "Error to open output file\n";
		return 0;
	}
	cout << "Output file opened\n";

	// Read File & Parser Example

	string file, title_name, tmp;
	fstream fi;
	vector<string> tmp_string;

	// from data_dir get file ....
	// eg : use 0.txt in data directory
	// fi.open("data/53.txt", ios::in);
	vector<string> textFiles = getTextFiles(data_dir);
	vector<string> Article_title;
	Article_title.resize(10010);

	Trie trie;
    // Process each file
    for(const auto& filepath : textFiles){
        fi.open(filepath, ios::in);
        if(!fi.is_open()){
            cerr << "Failed to open: " << filepath << endl;
            continue;
        }
		// Extract file name without extension
		string filename = fs::path(filepath).stem().string();
		int fileIndex = stoi(filename);

        // GET TITLENAME
        getline(fi, title_name);
        Article_title[fileIndex] = title_name;
        
		// GET TITLENAME WORD ARRAY
		tmp_string = split(title_name, " ");
		vector<string> title = word_parse(tmp_string);
		for(const auto& word : title){
			trie.insert(word, fileIndex);
		}

		// GET CONTENT LINE BY LINE
		while(getline(fi, tmp)){
			// GET CONTENT WORD VECTOR
			tmp_string = split(tmp, " ");

			// PARSE CONTENT
			vector<string> content = word_parse(tmp_string);

			for(auto &word : content){
				trie.insert(word, fileIndex);
			}
		}
        fi.close();
    }
	cout << "Trie built\n";

	// OPEN query.txt
	fi.open(query, ios::in);
	if(!fi.is_open()){
		cerr << "Failed to open: " << query << endl;
	}
	while(getline(fi, tmp)){
		tmp_string = split(tmp, " ");

		// for(auto s : tmp_string)	cout << s << "\n";


		set<int> answer, tmp_set, b;
		answer = find_set(tmp_string[0], &trie);
		for(int i = 1; i < tmp_string.size(); i++){
			cout << tmp_string[i] << "\n";
			if(tmp_string[i] == "+"){
				tmp_set = find_set(tmp_string[i+1], &trie);
				b = set_intersection(answer, tmp_set);
				answer = b;
				i++;
			}
			else if(tmp_string[i] == "-"){
				tmp_set = find_set(tmp_string[i+1], &trie);
				b = set_union(answer, tmp_set);
				answer = b;
				i++;
			}
			else if(tmp_string[i] == "/"){
				tmp_set = find_set(tmp_string[i+1], &trie);
				b = set_difference(answer, tmp_set);
				answer = b;
				i++;
			}
		}
		if(answer.empty()){
			outputFile << "Not Found!\n";
			continue;
		}
		for(auto &index : answer){
			outputFile << Article_title[index] << "\n";
		}
		answer.clear();
	}
	cout << "Query processed\n";
	// fi.open("data/53.txt", ios::in);
    // // GET TITLENAME
	// getline(fi, title_name);

    // // GET TITLENAME WORD ARRAY
    // tmp_string = split(title_name, " ");
	// vector<string> title = word_parse(tmp_string);

	// for(auto &word : title){
	// 	cout << word << " ";
	// }

    // // GET CONTENT LINE BY LINE
	// while(getline(fi, tmp)){

    //     // GET CONTENT WORD VECTOR
	// 	tmp_string = split(tmp, " ");

	// 	// PARSE CONTENT
	// 	vector<string> content = word_parse(tmp_string);

	// 	for(auto &word : content){
	// 		outputFile << word << endl;
	// 	}
	// 	//......
	// }

    // CLOSE FILE
	fi.close();
	outputFile.close();
}


// 1. UPPERCASE CHARACTER & LOWERCASE CHARACTER ARE SEEN AS SAME.
// 2. FOR SPECIAL CHARACTER OR DIGITS IN CONTENT OR TITLE -> PLEASE JUST IGNORE, YOU WONT NEED TO CONSIDER IT.
//    EG : "AB?AB" WILL BE SEEN AS "ABAB", "I AM SO SURPRISE!" WILL BE SEEN AS WORD ARRAY AS ["I", "AM", "SO", "SURPRISE"].
// 3. THE OPERATOR IN "QUERY.TXT" IS LEFT ASSOCIATIVE
//    EG : A + B / C == (A + B) / C

//

//////////////////////////////////////////////////////////
