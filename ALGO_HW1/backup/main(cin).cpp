//
//  main.cpp
//  ALGO_HW1
//
//  Created by Sciencethebird on 2019/10/16.
//  Copyright © 2019 Sciencethebird. All rights reserved.
//

#include <iostream>
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
    node():parent(nullptr), left_node(nullptr), right_node(nullptr), content('\0'){}
    node(int idx, char content, node*L, node* R){
        this->idx = idx;
        this->content = content;
        this->left_node = L;
        this->right_node = R;
    }
};

class tree{
public:
    tree():tree_nodes(nullptr){};
    tree(int num_of_node){
        this->num_of_node = num_of_node;
        tree_nodes = new node*[num_of_node+1]; // start from 1
    }
    void add_node(int idx, char content, int Lchild_idx,  int Rchild_idx){
        
          tree_nodes[idx] =
          new node(idx, content, tree_nodes[Lchild_idx], tree_nodes[Rchild_idx]);
          if(idx == num_of_node) root = tree_nodes[idx];
    }
    void print_preoder(){
        print_preoder_utils(this->root);
    }
    int longest_vowel_len(){
        longest_chain_utils(this->root);
        return this->max_vowel_len;
    }
    void print_longest_chain(){
        max_vowel_idx.clear();
       
        cout <<": " <<longest_vowel_len()<< endl << ':';
        for(int i = 0; i< max_vowel_ans.size();i++){
            cout  <<this->max_vowel_ans[i] << " ";
        }cout << endl;
    }
    int longest_chain(node* curr_root, int LR){ // longest chain of a root
        if( curr_root == nullptr || !is_vowel(curr_root->content) ) return 0;
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

    node* get(int idx){
        return tree_nodes[idx];
    }
    
private:
    
    int max_vowel_len = 0;
    int num_of_node;
    node** tree_nodes;
    node* root;
    //node* root;
    
    deque<int> max_vowel_idx;
    deque<int> max_vowel_ans;
    
    // mode 0
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
    

    // print pre-oder
    void print_preoder_utils(node *curr){
        if(curr){
            cout << curr->content << " ";
            print_preoder_utils(curr->left_node);
            print_preoder_utils(curr->right_node);
        }
    }
    
};

int main(int argc, const char * argv[]) {
    
    int num_of_nodes = 0;
    int mode = 0;
    for(int i = 0; i< argc; i++){
        cout << argv[i] << endl;
    }
    string input_file = argv[1];
    string output_file = argv[2];
    cin >> num_of_nodes >> mode;
    tree test(num_of_nodes);
    
    for(int i = 0 ;i < num_of_nodes ; i++){
        int idx, Lchild, Rchild;
        char c;
        cin >> idx >> c >> Lchild >> Rchild;
        test.add_node(idx, c, Lchild, Rchild);
    }
    //cout << test.longest
    //test.print_preoder();
    //for(int i)
    //cout << "ans: "<<test.longest_chain(test.get(9)) << endl;
    //cout << test.longest_chain(test.get(7)) << endl;
    /*
    for(int i = 1; i<=num_of_nodes; i++){
        test.longest_chain_out(test.get(i));
    }*/
    cout << "dddd: "<<test.longest_vowel_len() << endl;
    test.print_longest_chain();
    //test.longest_chain_out();
    //test.longest_vowel_chain();
    return 0;
}
