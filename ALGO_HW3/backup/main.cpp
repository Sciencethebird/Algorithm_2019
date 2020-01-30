//
//  main.cpp
//  HW3
//
//  Created by Sciencethebird on 2020/1/11.
//  Copyright © 2020 Sciencethebird. All rights reserved.
//

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <deque>
using namespace std;

void print_map(vector<vector<int>> map){
    size_t sz = map.size();
    for (int i = 0; i < sz; i++) {
         for (int j = 0; j < sz; j++) {
             cout << map[i][j] <<  right << setw(5);
             //cout << color[i][j] << " ";
         }cout << endl << setw(0);
     }
    cout << endl;
}

deque<int> BFS(const vector<vector<int> > &G, int start) {
    
    deque<int> Q;
    int n = (int) G.size();

    vector<bool> color(n, true);
    vector<int> pi(n, -1);
    
    
    color[start] = false; // gray

    Q.push_back(start);
    while (Q.size() != 0) {
        int u = Q[0];
        Q.pop_front();
        for (int i = 0; i < n; i++) {
            if (G[u][i] != 0) {
                if (color[i] == true) {
                    color[i] = false;
                    pi[i] = u;
                    Q.push_back(i);
                }
            }
        }
    }
    
    // get path
    deque<int> path;
    int t = n-1;
    while(t!=-1){
        path.push_front(t);
        t = pi[t];
    }
   
    return path;
}

bool augment_flow(vector< vector<int> > &G){
    deque<int> path = BFS(G, 0);
    if(path[0] != 0){ // when path not connecting source
        //cout << "no path" << endl;
        return false;
    }
    int critic = 2147483647;
    for(int i = 2;i<path.size()-1;i++){ // find critic path
        if(G[ path[i] ][ path[i+1] ]<critic && G[ path[i] ][ path[i+1] ]>0){
            critic = G[ path[i] ][ path[i+1] ];
        }
    }
    //cout << "critic: "<<critic << endl;
    
    for(int i = 1;i<path.size()-1;i++){ // find critic path
        G[ path[i+1] ][ path[i] ] += critic;
        G[ path[i] ][ path[i+1] ] -= critic;
    }
    return true;
}


int main(int argc, const char * argv[])
{
    fstream file;

    int ware_fac_num = 0, num_of_truck;
    
    vector<int> dir_ware;
    vector<int> stores;

    vector<int> cap_of_v;

    vector< vector<int> > truck_map;
    vector< vector<int> > aug_truck_map; // Bonus

    string input_file = argv[1];
    string output_file = argv[2];
    file.open(input_file.c_str(), ios::in);
    
    // Read Line 1 : Total number of warehouses+stores
    file >> ware_fac_num;
    
    // Read Line 3, 4
    string temp;
    while (getline(file, temp)) {
        // Line 3: List of direct factory
        if (temp.size()>1) {
            istringstream buffer(temp);
            int tt;
            while (buffer >> tt) {
                dir_ware.push_back(tt);
            }
            break;
        }
    }
    
    // Read Line 4: List of stores
    getline(file, temp);
    istringstream buffer(temp);
    int tt;
    while (buffer >> tt) {
        stores.push_back(tt);
    }
    
    
    // Read Line 6: Capacity of warehouses and stores
    cap_of_v.push_back(-1); // cap of s = INF
    for (int i = 0; i < ware_fac_num; i++) {
        string temp;
        file >> temp;
        if (temp[0] == 'I')
            cap_of_v.push_back(-1); //-1 is INF
        else
            cap_of_v.push_back(stoi(temp));
    }
    cap_of_v.push_back(-1); // cap of t = INF
    
    // Read Line 8 : Number of trucks
    file >> num_of_truck;
    
    // truck map init
    int vertex_num = ware_fac_num+2;
    for (int i = 0; i < vertex_num ; i++) {
        vector<int> t(vertex_num, 0);
        truck_map.push_back(t);
    }
    for (int i = 0; i < dir_ware.size(); i++) {
        truck_map[0][dir_ware[i]] = -1; // INF
    }
    
    // build truck map
    for (int i = 0; i < num_of_truck; i++) {
        int s, t, c;
        file >> s >> t >> c;
        truck_map[s][t] = c;
    }
    
    // connect to dummy terminal node (t)
    for(int i = 0; i<stores.size(); i++){
        truck_map[stores[i]][vertex_num-1] = -1;
    }
    
    // init aug truck map
    int aug_vertex_num = 2*vertex_num;
    for (int i = 0; i < aug_vertex_num ; i++) {
        vector<int> t(aug_vertex_num, 0);
        aug_truck_map.push_back(t);
    }
    // build aug truck map
    for (int i = 0; i < vertex_num; i++) {
        for (int j = 0; j < vertex_num; j++) {
            if(i != j){
                aug_truck_map[2*i][2*j]   = 0;
                aug_truck_map[2*i][2*j+1] = 0;
                aug_truck_map[2*i+1][2*j]   = truck_map[i][j];
                aug_truck_map[2*i+1][2*j+1] = 0;
            }else{
                aug_truck_map[2*i][2*j]   = 0;
                aug_truck_map[2*i][2*j+1] = cap_of_v[i];
                aug_truck_map[2*i+1][2*j]   = 0;
                aug_truck_map[2*i+1][2*j+1] = 0;
            }
        }
    }
    
    // find augment flow
    while(augment_flow(aug_truck_map)){}

    long long ans = 0;
    for(int i = 0; i<aug_vertex_num; i++){
        ans+= aug_truck_map[aug_vertex_num-1][i];
    }
    
    // output ans
    file.close();
    file.open(output_file.c_str(), fstream::out);
    file << ans << endl;
    file.close();
    
    return 0;
}
