#include<bits/stdc++.h>
using namespace std;


struct Node{
    Node* links[26];
    bool flag = false;
    
    bool containsKey(char ch){
        return links[ch - 'a'];
    }
    
    Node* get(char ch){
        return links[ch - 'a'];
    }
    
    void put(char ch, Node* node){
        links[ch - 'a'] = node;
    }
    
    void setEnd(){
        flag = true;
    }
    
    bool isEnd(){
        return flag;
    }
};


class Trie{
    Node* root;
    
    public:
    Trie(){
        root = new Node();
    }
    
    void insert(string word){
        Node* temp = root;
        for(char ch : word){
            if(temp->containsKey(ch) == false) temp->put(ch, new Node());
            temp = temp->get(ch);
        }
        temp->setEnd();
    }
    
    bool allPrefixPresent(string s){
        Node* node = root;
        for(char ch : s){
            if(node->containsKey(ch) == false) return false;
            node = node->get(ch);
            if(node->isEnd() == false) return false;
        }
        return true;
    }
    
    bool isPresent(string word){
        Node* node = root;
        for(char ch : word){
            if(!node->containsKey(ch)) return false;
            node = node->get(ch);
        }
        return node->isEnd();
    }
};



//longest word with all prefixes
string completeString(vector<string>& nums) {
    Trie trie;
    for(string word : nums) trie.insert(word);

    string longestStr = "";
    for(string s : nums){
        if(trie.allPrefixPresent(s)){
            if(s.size() > longestStr.size()) longestStr = s;
            else if(s.size() == longestStr.size() && s < longestStr) longestStr = s;
        }
    }

    return longestStr == "" ? "None" : longestStr;
}




//no of distinct substrings
int countDistinctSubstring(string s){
	//TC: O(n^2), SC: difficult to compute as varies significantly on input size and len of string
    Node* root = new Node();
    int cnt = 1;
    for(int i = 0; i < s.size(); i++){
        Node* node = root;
        for(int j = i; j < s.size(); j++){
            if(!node->containsKey(s[j])){
                node->put(s[j], new Node());
                cnt++;
            }
            node = node->get(s[j]);
        }
    }
    return cnt;
}



//max xor of 2 numbers
struct binaryNode{
    binaryNode* links[2]; //0, 1
    
    bool containsKey(int num){
        return links[num];
    }

    void put(int ind, binaryNode* node){
        links[ind] = node;
    }

    binaryNode* get(int ind){
        return links[ind];
    }
};

class binaryTrie{
    binaryNode* root;

public: 
    binaryTrie(){
        root = new binaryNode();
    }

    void insert(int num){
        binaryNode* temp = root;
        for(int i = 31; i >= 0; i--){
            int ind = (num >> i) & 1;
            if(!temp->containsKey(ind)) temp->put(ind, new binaryNode());
            temp = temp->get(ind);
        }
    }

    int getMax(int num){
        binaryNode* temp = root;
        int ans = 0;
        for(int i = 31; i >= 0; i--){
            int bit = (num >> i) & 1;
            
            if(temp->containsKey(1-bit)){
                temp = temp->get(1-bit);
                ans = (ans << 1) + 1;
            }
            else{
                temp = temp->get(bit);
                ans = ans << 1;
            }
        }
        return ans;
    }
};

int findMaximumXOR(vector<int>& nums) {
	//brute
    //TC: O(n^2)
/*
    int max_xor = 0;
    for(int i = 0; i < nums.size(); i++){
        for(int j = i+1; j < nums.size(); j++){
            max_xor = max(max_xor, nums[i] ^ nums[j]);
        }
    }
    return max_xor;
*/


    //optimal
    //fixed trie with len of 32 for each number
    binaryTrie trie;
    for(int i : nums) trie.insert(i);
    int max_xor = 0;
    for(int i : nums) max_xor = max(max_xor, trie.getMax(i));
    return max_xor;
}


static bool compare(pair<int, vector<int>> &a, pair<int, vector<int>> &b){
    return a.second[1] < b.second[1];
}


//max xor queries
vector<int> maximizeXor(vector<int>& nums, vector<vector<int> >& queries) {
    //brute
/*
    vector<int> ans;
    for(int i = 0; i < queries.size(); i++){
        int max_xor = -1;
        for(int j : nums){
            if(j <= queries[i][1]) max_xor = max(max_xor, queries[i][0] ^ j);
        }
        ans.push_back(max_xor);
    }
    return ans;
*/


    //optimal
    binaryTrie trie;
    sort(nums.begin(), nums.end());
    vector<pair<int, vector<int>>> queries_ind;
    for(int i = 0; i < queries.size(); i++) queries_ind.push_back({i, queries[i]});
    sort(queries_ind.begin(), queries_ind.end(), compare);
    int nums_ind = 0;
    vector<int> ans(queries.size());
    for(int i = 0; i < queries_ind.size(); i++){
        while(nums[nums_ind] <= queries_ind[i].second[1] && nums_ind < nums.size()){
            trie.insert(nums[nums_ind++]);
        }
        ans[queries_ind[i].first] = trie.getMax(queries_ind[i].second[0]);
    }
    return ans;
}