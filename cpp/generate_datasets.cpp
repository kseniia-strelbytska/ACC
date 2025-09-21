#include "header.h"

int t[101];

// using a set to store presentations, as after normalisation there can be duplicates
set<node> presentations;

// a recursive function to generate presentations
void generate_p(ll pos, ll val, ll max_len){
    t[pos] = val;
    
    if(pos == max_len){
        // check all divisions into two relators
        for(int l = 1; l < max_len; l++){
            node p;
            
            for(int i = 1; i <= pos; i++){
                if(i <= l)
                    p.first.push_back(t[i]);
                else
                    p.second.push_back(t[i]);
            }
            
            normalise(p.first);
            normalise(p.second);
            
            bool x = false, y = false;
            
            for(auto i: p.first){
                if(abs(i) == 1)
                    x = true;
                else
                    y = true;
            }
            
            for(auto i: p.second){
                if(abs(i) == 1)
                    x = true;
                else
                    y = true;
            }
            
            // check that both generators are present, and neither relator is empty
            if(x && y && !p.first.empty() && !p.second.empty()){
                presentations.insert(p);
            }
        }
    }
    else{
        for(auto i : {-2, -1, 1, 2}){
            generate_p(pos + 1, i, max_len);
        }
    }
}

// generates all presentations of length {max_len} (in simplified form)
// stores result in a global set {presentations}
void initialise_presentations(ll max_len){
    presentations.clear();
    
    for(auto i : {-2, -1, 1, 2})
        generate_p(1, i, max_len);
}

// generates and saves to separate files all presentations of lengths 2 to 9
void generate_all(string dir_path){
    for(int max_len = 2; max_len <= 9; max_len++){
        string file_path = dir_path + "/cpp/datasets/dataset_len" + to_string(max_len) + ".txt";
        ofstream output(file_path);
        
        initialise_presentations(max_len);
        
        for(auto i: presentations){
            // printing into a file
            print(output, i.first);
            print(output, i.second);
        }
    }
}
