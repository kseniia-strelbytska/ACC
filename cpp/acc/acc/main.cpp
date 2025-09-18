#include <iostream>
#include <vector>
#include <string>
#include <math.h>
#include <cmath>
#include <iomanip>
#include <cstdio>
#include <algorithm>
#include <numeric>
#include <map>
#include <set>
#include <queue>
#include <stack>
#include <deque>
#include <bitset>
#include <cstring>
#include <unordered_map>
#include <filesystem>
#include <fstream>

using namespace std;
typedef long long ll;
typedef pair<deque<int>, deque<int>> node;
typedef pair<pair<int, int>, node> node_info;

int const max_nodes = (ll)(1e4);

/*
0. r_1 --> r_1 r_0
1. r_0 --> r_0 r_1^{-1} -> r_0 -> r_0^{-1}
2. r_1 --> r_1 r_0^{-1} -> r_1 -> r_1^{-1}
3. r_0 --> r_0 r_1
4: r_1 --> x_0^{-1} r_1 x_0
5: r_0 ---> x_1^{-1} r_0 x_1
6: r_1 --> x_1^{-1} r_1 x_1
7: r_0 ---> x_0 r_0 x_0^{-1}
8: r_1 --> x_0 r_1 x_0^{-1}
9: r_0 --> x_1 r_0 x_1^{-1}
10: r_1 --> x_1 r_1 x_1^{-1}
11: r_0 --> x_0^{-1} r_0 x_0
 
 */

void print(node a){
    for(auto i: a.first)
        cout << i << ' ';
    cout << "| ";
    
    for(auto i: a.second)
        cout << i << ' ';
    cout << endl;
}

void print(deque<int> a){
    for(auto i: a)
        cout << i << ' ';
    cout << endl;
}

deque<int> concat(deque<int> a, deque<int> b){
    while(!a.empty() && !b.empty() && abs(a.back()) == abs(b[0]) && a.back() != b[0]){
        a.pop_back();
        b.pop_front();
    }
    
    for(auto i: b)
        a.push_back(i);
    
    return a;
}

void normalise(deque<int> &a){
    while((int)(a.size()) >= 2 && abs(a[0]) == abs(a[1]) && a[0] != a[1]){
        a.pop_front();
        a.pop_front();
    }
    
    while((int)(a.size()) >= 2 && abs(a[(int)(a.size()) - 1]) == abs(a[(int)(a.size()) - 2]) && a[(int)(a.size()) - 1] != a[(int)(a.size()) - 2]){
        a.pop_back();
        a.pop_back();
    }
}

// convert a line of numbers into a presentation (word)
deque<int> convert(string line){
    deque<int> a;
    
    int idx = 0;
    while(idx < (ll)(line.size())){
        if(line[idx] == ' '){
            idx++;
            continue;
        }
        
        int number = 1;
        
        if(line[idx] == '-'){
            number = -1;
            idx++;
        }
        
        number *= (line[idx] - '0');
        idx++;
        
        a.push_back(number);
    }
    
    return a;
}

vector<node> load_presentations(string file_path){
    ifstream stream(file_path.c_str());
    
    vector<node> presentations;
        
    if(stream.is_open()){
        string line1, line2;
        
        while(getline(stream, line1)){
            getline(stream, line2);
            
            node presentation;
            
            presentation.first = convert(line1);
            presentation.second = convert(line2);
            
            presentations.push_back(presentation);
        }
        
        stream.close();
    }
    else{
        cout << "Error" << endl;
    }
    
    return presentations;
}

string get_file_path(int max_len){
    return "/Users/kseniia/Desktop/programming/Projects/acc/cpp/datasets/dataset_len" + to_string(max_len) + ".txt";
}

int main(){
    for(int max_len = 3; max_len <= 3; max_len++){
        string file_path = get_file_path(max_len);
        
        cout << file_path << endl;
        
        auto res = load_presentations(file_path);
        
        for(auto i: res)
            print(i);
    }
     
    return 0;
}

