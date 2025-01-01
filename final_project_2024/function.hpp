#ifndef FUNCTION_HPP
#define FUNCTION_HPP

#include <iostream>
#include <vector>
#include <string>
#include <set>
using namespace std;

namespace my {
	template<class T> set<T> set_union(const set<T> &a, const set<T> &b);
	set<int> set_intersection(const set<int> &a, const set<int> &b);
	set<int> set_difference(const set<int> &a, const set<int> &b);
};

vector<string> split(const string& str, const string& delim);
vector<string> word_parse(const vector<string> &tmp_string);

template <class T>
set<T> my::set_union(const set<T> &a, const set<T> &b){
	set<T> tmp;
	tmp = a;
	for(auto &i : b){
		tmp.insert(i);
	}
	return tmp;
}

set<int> my::set_intersection(const set<int> &a, const set<int> &b){
	set<int> tmp;
	for(auto &i : a){
		if(b.find(i) != b.end()){
			tmp.insert(i);
		}
	}
	return tmp;
}

set<int> my::set_difference(const set<int> &a, const set<int> &b){
	set<int> tmp;
	for(auto &i : a){
		if(b.find(i) == b.end()){
			tmp.insert(i);
		}
	}
	return tmp;
}
// string parser : output vector of strings (words) after parsing
vector<string> word_parse(const vector<string> &tmp_string){
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

#endif