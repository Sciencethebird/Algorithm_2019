//
//  main.cpp
//  ALGO_HW1
//
//  Created by Sciencethebird on 2019/10/16.
//  Copyright © 2019 Sciencethebird. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <deque>
#include <string>
using namespace std;

bool is_vowel(char c){
    return c == 'a' || c == 'e'|| c == 'i'|| c == 'o'|| c == 'u';
}

struct node{
    int idx;
    char content;
    node* parent;
    node* left_node;
    node* right_node;
    node():parent(NULL), left_node(NULL), right_node(NULL), content('\0'){}
    node(int idx, char content, node*L, node* R){
        this->idx = idx;
        this->content = content;
        this->left_node = L;
        this->right_node = R;
    }
};

class tree{
public:
    tree():tree_nodes(NULL), max_vowel_len(0){};
    tree(int num_of_node){
        this->num_of_node = num_of_node;
        tree_nodes = new node*[num_of_node+10]; // start from 1
        max_vowel_len = 0;
    }
    void add_node(int idx, char content, int Lchild_idx,  int Rchild_idx){
        
          tree_nodes[idx] =
          new node(idx, content, tree_nodes[Lchild_idx], tree_nodes[Rchild_idx]);
          if(idx == num_of_node) root = tree_nodes[idx];
    }
    int longest_vowel_len(){
        longest_chain_utils_0(this->root);
        return this->max_vowel_len;
    }
    void print_longest_chain(fstream &file){
        max_vowel_idx.clear();
        longest_chain_utils(this->root);
        file << this->max_vowel_len -1 << endl << endl;
        if(max_vowel_ans.front() < max_vowel_ans.back() )
            for(int i = 0; i< max_vowel_ans.size();i++)
                file  <<this->max_vowel_ans[i] << endl;
        else
            for(int i = max_vowel_ans.size()-1; i>=0 ;i--)
                file  <<this->max_vowel_ans[i] << endl;
        
    }
    
private:
    
    int max_vowel_len;
    int num_of_node;
    node** tree_nodes;
    node* root;
    //node* root;
    deque<int> max_vowel_idx;
    deque<int> max_vowel_ans;
    
    // mode 0
    void longest_chain_utils_0(node* curr_root){ // visit every node
        if(curr_root){
            if(is_vowel(curr_root->content)){
                int left_longest = longest_chain_0(curr_root->left_node);
                int right_longest = longest_chain_0(curr_root->right_node);
                int curr_len =  1 + left_longest + right_longest;
                if(curr_len > max_vowel_len)
                    this->max_vowel_len = curr_len;
            }
            longest_chain_utils(curr_root->left_node);
            longest_chain_utils(curr_root->right_node);
        }
    }
    int longest_chain_0(node* curr_root){ // longest chain of a root
        if( curr_root == NULL || !is_vowel(curr_root->content) ) return 0;
        int left_chain = longest_chain_0(curr_root->left_node);
        int right_chain = longest_chain_0(curr_root->right_node);
        
        if(left_chain > right_chain) return 1 + left_chain;
         
        else if (left_chain < right_chain) return 1 + right_chain;
        else return 1;
    }
    
    // mode 1
    void longest_chain_utils(node* curr_root){ // visit every node
        if(curr_root){
            if(is_vowel(curr_root->content)){
                int left_longest = longest_chain(curr_root->left_node, 0);
                max_vowel_idx.push_back(curr_root->idx);
                int right_longest = longest_chain(curr_root->right_node, 1);
                int curr_len =  1 + left_longest + right_longest;
                //cout << curr_len << endl;
                if(curr_len > max_vowel_len){
                    this->max_vowel_len = curr_len;
                    this->max_vowel_ans = max_vowel_idx;
                    
                }
                max_vowel_idx.clear();
            }
            longest_chain_utils(curr_root->left_node);
            longest_chain_utils(curr_root->right_node);
        }
    }
    int longest_chain(node* curr_root, int LR){ // longest chain of a root
          if( curr_root == NULL || !is_vowel(curr_root->content) ) return 0;
          if(LR == 0)
              max_vowel_idx.push_front(curr_root->idx);
          else
              max_vowel_idx.push_back(curr_root->idx);
          int left_chain = longest_chain(curr_root->left_node, LR);
          int right_chain = longest_chain(curr_root->right_node, LR);
          
          if(left_chain > right_chain){
              return 1 + left_chain;
          }
          else if (left_chain < right_chain){
              return 1 + right_chain;
          }else{
              return 1;
          }
      }
};

int main(int argc, const char * argv[]) {
    
    int num_of_nodes = 0;
    int mode = 0;
  
    string input_file = argv[1];
    string output_file = argv[2];
    
    fstream file;
    file.open(input_file.c_str(), ios::in);
    
    file >> num_of_nodes >> mode;
    
    tree test(num_of_nodes);
    
    for(int i = 0 ;i < num_of_nodes ; i++){
        int idx, Lchild, Rchild;
        char c;
        file >> idx >> c >> Lchild >> Rchild;
        test.add_node(idx, c, Lchild, Rchild);
    }
    
    file.close();
    file.open(output_file.c_str(), fstream::out);
    
    if(mode == 0)
        file << test.longest_vowel_len()-1 << endl;
    else
        test.print_longest_chain(file);
    
    file.close();
    return 0;
}
