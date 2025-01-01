#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <set>
#include "trie.hpp"
using namespace std;

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

int main(){
	Trie trie;
	trie.insert("apple", 0);
	trie.insert("banana", 0);
	string a = "<*abc*>";
	vector<string> tmp_string = split(a.substr(1, a.size()-2), "*");
	for(auto s : tmp_string)	cout << s << "\n";
	cout << "done";
	return 0;
}