#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <unordered_set>
#include <execution>
using namespace std;

namespace my {
	template<class T> set<T> set_union(const set<T> &a, const set<T> &b);
	set<int> set_intersection(const set<int> &a, const set<int> &b);
	set<int> set_difference(const set<int> &a, const set<int> &b);
};

vector<string> split(const string& str, const string& delim);

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
	set<int> find_wildcard(string str);
	set<int> find_child(Node *root);

	set<Node*> find_substr(Node *root, string str, int idx);
	Node* find_prefix_node(string str);
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
	for(int i = len-1; i >= 0; i--){
		str_data = (str[i] >= 'A' && str[i] <= 'Z') ? str[i]- 'A' : str[i] - 'a';
		if(finding->child[str_data] == nullptr) {
			return tmp;
		}
		finding = finding->child[str_data];
	}
	return find_child(finding);
}
set<int> Trie::find_wildcard(string str){
	int len = str.size();
	vector<string> tmp_string;
	tmp_string = split(str.substr(1, len-2), "*");
	int tmp_str_len = tmp_string.size();

	set<int> ans, tb;
	set<Node*> all_sets, tmp;
	Node* start_node;
	vector<string>::iterator begin = tmp_string.begin();

	if(str[1] != '*'){		// e.g. <ab*c*>
		if(tmp_str_len > 1){
			start_node = this->find_prefix_node(tmp_string[0]);
			all_sets = {start_node};

			for(int i = 1; i < tmp_str_len; i++){
				for(auto &node : all_sets){
					// find the next substring from the previous matched roots
					tmp = move(my::set_union(tmp, this->find_substr(node, *(begin + i), 0)));
				}
				all_sets = move(tmp);
			}

			for(auto &root : all_sets){
				if(str[len-2] == '*'){
					tb = this->find_child(root);
					ans = move(my::set_union(ans, tb));
				}
				else if(str[len-2] != '*' && root->ptr != nullptr){
					ans = move(my::set_union(ans, root->ptr->title));
				}
			}
		}
		else{
			ans = this->find_pre(tmp_string[0]);
		}
	}
	else if(str[1] == '*'){
		start_node = this->pre_root;
		all_sets = this->find_substr(start_node, *begin, 0);

		for(int i = 1; i < tmp_str_len; i++){
			for(auto &node : all_sets){
				// find the next substring from the previous matched roots
				tmp = move(my::set_union(tmp, this->find_substr(node, *(begin + i), 0)));
			}
			all_sets = move(tmp);
		}

		for(auto &root : all_sets){
			if(str[len-2] == '*'){
				tb = this->find_child(root);
				ans = move(my::set_union(ans, tb));
			}
			else if(str[len-2] != '*' && root->ptr != nullptr){
				ans = move(my::set_union(ans, root->ptr->title));
			}
		}
	}
	return ans;
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
		tmp = my::set_union(all_sets, b);
		all_sets = move(tmp);
	}
	return all_sets;
}


set<Node*> Trie::find_substr(Node *root, string str, int idx){
	set<Node *> all_sets, tmp;
	int str_data = (str[idx] >= 'A' && str[idx] <= 'Z') ? str[idx] - 'A' : str[idx] - 'a';
	if(str.size() == idx){
		return {root};
	}
	for(int i = 0; i < 26; i++){
		if(root->child[i] != nullptr && i == str_data){
			tmp = find_substr(root->child[i], str, idx+1);
		}
		for(auto &i : tmp){
			all_sets.insert(i);
		}
		if(root->child[i] != nullptr && idx == 0){
			tmp = find_substr(root->child[i], str, 0);
		}
		for(auto &i : tmp){
			all_sets.insert(i);
		}
	}
	return all_sets;
}

Node* Trie::find_prefix_node(string str){
	int len = size(str);
	int str_data;
	Node *finding;
	finding = this->pre_root;
	for(int i = 0; i < len; i++){
		str_data = (str[i] >= 'A' && str[i] <= 'Z') ? str[i]- 'A' : str[i] - 'a';
		if(finding->child[str_data] == nullptr){
			return nullptr;
		}
		finding = finding->child[str_data];
	}
	return finding;
}

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