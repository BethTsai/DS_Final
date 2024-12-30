#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <algorithm>
#include <execution>
using namespace std;

set<int> set_union(const set<int> &a, const set<int> &b);
set<int> set_intersection(const set<int> &a, const set<int> &b);
set<int> set_difference(const set<int> &a, const set<int> &b);

struct Mark{
	set<int> title;
};

struct Node{
	Node *child[26];
	Mark *ptr;
	int data;
	Node(int data) : data(data) {
		for(int i = 0; i < 26; i++) child[i] = nullptr;
		ptr = nullptr;
	}
	Node(){
		for(int i = 0; i < 26; i++) child[i] = nullptr;
		ptr = nullptr;
	}
};

class Trie{
public:
	Node *pre_root, *suf_root;
	set<int> find_exact(string str);
	set<int> find_pre(string str);
	set<int> find_suf(string str);
	set<int> find_child(Node *root);
	void insert(string str, int label);
	Trie(){
		pre_root = new Node;
		suf_root = new Node;
	}

};


void Trie::insert(string str, int label){
	int len = size(str);
	int str_data;
	Node *tmp;
	// build trie
	tmp = this->pre_root;
	for(int i = 0; i < len; i++){
		str_data = (str[i] >= 'A' && str[i] <= 'Z') ? str[i]- 'A' : str[i] - 'a';
		if(tmp->child[str_data] == nullptr){
			tmp->child[str_data] = new Node;
			tmp->child[str_data]->data = str_data;
		}
		tmp = tmp->child[str_data];
	}
	if(tmp->ptr == nullptr){
		tmp->ptr = new Mark;
	}
	tmp->ptr->title.insert(label);

	// build suffix trie
	tmp = this->suf_root;
	for(int i = len-1; i >= 0; i--){
		str_data = (str[i] >= 'A' && str[i] <= 'Z') ? str[i]- 'A' : str[i] - 'a';
		if(tmp->child[str_data] == nullptr){
			tmp->child[str_data] = new Node;
			tmp->child[str_data]->data = str_data;
 		}
		tmp = tmp->child[str_data];
	}
	if(tmp->ptr == nullptr){
		tmp->ptr = new Mark;
	}
	tmp->ptr->title.insert(label);
}

set<int> Trie::find_exact(string str){
	int len = size(str);
	int str_data;
	set<int> tmp;
	Node *finding;
	finding = this->pre_root;
	for(int i = 0; i < len; i++){
		str_data = (str[i] >= 'A' && str[i] <= 'Z') ? str[i]- 'A' : str[i] - 'a';
		if(finding->child[str_data] == nullptr) {
			return tmp;
		}
		finding = finding->child[str_data];
	}
	if(finding != nullptr && finding->ptr != nullptr){
		tmp = finding->ptr->title;
	}
	return tmp;
}

set<int> Trie::find_pre(string str){
	int len = size(str);
	int str_data;
	set<int> tmp;
	Node *finding;
	finding = this->pre_root;
	for(int i = 0; i < len; i++){
		str_data = (str[i] >= 'A' && str[i] <= 'Z') ? str[i]- 'A' : str[i] - 'a';
		if(finding->child[str_data] == nullptr) {
			return tmp;
		}
		finding = finding->child[str_data];
	}
	return find_child(finding);
}

set<int> Trie::find_suf(string str){
	int len = size(str);
	int str_data;
	set<int> tmp;
	Node *finding;
	finding = this->suf_root;
	for(int i = len-1; i >= 0; i++){
		str_data = (str[i] >= 'A' && str[i] <= 'Z') ? str[i]- 'A' : str[i] - 'a';
		if(finding->child[str_data] == nullptr) {
			return tmp;
		}
		finding = finding->child[str_data];
	}
	return find_child(finding);
}

set<int> Trie::find_child(Node *root){
	set<int> all_sets, tmp;
	if(root->ptr != nullptr){
		all_sets = root->ptr->title;
	}
	for(int i = 0; i < 26; i++){
		if(root->child[i] == nullptr){
			continue;
		}
		set<int> b = find_child(root->child[i]);
		tmp = set_union(all_sets, b);
		all_sets = tmp;
	}
	return all_sets;
}

set<int> set_union(const set<int> &a, const set<int> &b){
	set<int> tmp;
	tmp = a;
	for(auto &i : b){
		tmp.insert(i);
	}
	return tmp;
}

set<int> set_intersection(const set<int> &a, const set<int> &b){
	set<int> tmp;
	for(auto &i : a){
		if(b.find(i) != b.end()){
			tmp.insert(i);
		}
	}
	return tmp;
}

set<int> set_difference(const set<int> &a, const set<int> &b){
	set<int> tmp;
	for(auto &i : a){
		if(b.find(i) == b.end()){
			tmp.insert(i);
		}
	}
	return tmp;
}