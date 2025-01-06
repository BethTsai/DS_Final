#define FILE_EXTENSION ".txt"
#include <fstream>
#include <filesystem>
#include <string>
#include <cstring>
#include <vector>
#include <set>
#include <iostream>
#include "trie.hpp"
#include "function.hpp"
#define DATA_SIZE 10010
namespace fs = std::filesystem;
using namespace std;

vector<string> getTextFiles(const string& directory){
    vector<string> files;
    for(const auto& entry : fs::directory_iterator(directory)){
        if(entry.path().extension() == FILE_EXTENSION){
            files.push_back(entry.path().string());
        }
    }
    return files;
}

set<int> find_set(string &str, Trie *trie){
	set<int> tmp;
	if(str[0] == '\"'){
		tmp = move(trie->find_exact(str.substr(1, str.size()-2)));
	}
	else if(str[0] == '*'){
		tmp = move(trie->find_suf(str.substr(1, str.size()-2)));
	}
	else if(isalpha(str[0])){
		tmp = move(trie->find_pre(str));
	}
	else if(str[0] == '<'){
		tmp = move(trie->find_wildcard(str));
	}
	return tmp;
}


int main(int argc, char *argv[]){
	ios::sync_with_stdio(0);
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

	// Read File & Parser ===============================================================================

	string file, title_name, tmp;
	fstream fi;
	vector<string> tmp_string;

	vector<string> textFiles = getTextFiles(data_dir);
	vector<string> Article_title;
	Article_title.resize(DATA_SIZE);

    // Process each file ===============================================================================
	Trie trie;
    for(const auto& filepath : textFiles){
        fi.open(filepath, ios::in);
        if(!fi.is_open()){
            cerr << "Failed to open: " << filepath << "\n";
            continue;
        }
		// Extract file name without extension
		string filename = fs::path(filepath).stem().string();
		int file_idx = stoi(filename);

        // GET TITLENAME
        getline(fi, title_name);
        Article_title[file_idx] = title_name;
        
		// GET TITLENAME WORD ARRAY
		tmp_string = split(title_name, " ");
		vector<string> title = word_parse(tmp_string);
		for(const auto& word : title){
			trie.insert(word, file_idx);
		}

		// GET CONTENT LINE BY LINE
		while(getline(fi, tmp)){
			// GET CONTENT WORD VECTOR
			tmp_string = split(tmp, " ");

			// PARSE CONTENT
			vector<string> content = word_parse(tmp_string);

			for(auto &word : content){
				trie.insert(word, file_idx);
			}
		}
        fi.close();
    }

	// OPEN QUERY.TXT and PARSE =========================================================================

	fi.open(query, ios::in);
	if(!fi.is_open()){
		cerr << "Failed to open: " << query << "\n";
	}

	while(getline(fi, tmp)){
		tmp_string = split(tmp, " ");

		set<int> answer, tmp_set;
		answer = find_set(tmp_string[0], &trie);

		for(int i = 1; i < tmp_string.size(); i++){
			if(tmp_string[i] == "+"){
				tmp_set = move(find_set(tmp_string[i+1], &trie));
				answer = move(my::set_intersection(answer, tmp_set));
				i++;
			}
			else if(tmp_string[i] == "-"){
				tmp_set = move(find_set(tmp_string[i+1], &trie));
				answer = move(my::set_difference(answer, tmp_set));
				i++;
			}
			else if(tmp_string[i] == "/"){
				tmp_set = move(find_set(tmp_string[i+1], &trie));
				answer = move(my::set_union(answer, tmp_set));
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
