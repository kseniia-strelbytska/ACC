#include "header.h"

// extracts the filename from file path 
string get_name(string file_path){
    string s;
    
    for(int i = (int)(file_path.size()) - 1; i >= 0 && file_path[i] != '/'; i--)
        s += file_path[i];
    
    reverse(s.begin(), s.end());
    
    return s;
}

// runs greedy search on every presentation from {presentations}
// saves trivialised presentations and their paths
// prints the number of solved out of total 
void evaluate(vector<node> presentations, string output_presentation, string output_path, int max_nodes, int max_relator_length){
    vector<node> s_presentations;
    vector<vector<int>> s_paths;
    
    ll solved = 0, total = (ll)(presentations.size()), ind = 0;
    
    for(auto i: presentations){
        ind++;
        auto result = greedy_search(i, max_nodes, max_relator_length);
        
        if(result.first == true){
            solved++;
            
            s_presentations.push_back(i);
            s_paths.push_back(result.second);
        }
        
        cout << "Solved: " << solved << "/" << ind << endl;

//        if((ind + 1) % 100 == 0){ // store intermediate results
//            FILE *f = freopen(output_presentation.c_str(), "w", stdout);
//            for(auto i: s_presentations)
//                print(i);
//            
//            fclose(f);
//            
//            FILE *f1 = freopen(output_path.c_str(), "w", stdout);
//            for(auto i: s_paths){
//                for(auto j: i)
//                    cout << j << ' ';
//                cout << endl;
//            }
//            
//            fclose(f1);
//        }
        
        if(ind == 551)
            break;
    }
    
    ofstream out_presentation(output_presentation);
    for(auto i: s_presentations)
        print(out_presentation, i);
        
    ofstream out_path(output_path);
    for(auto i: s_paths){
        for(auto j: i)
            out_path << j << ' ';
        out_path << endl;
    }
        
    cout << solved << ' ' << total << ' ' << fixed << setprecision(3) << ((ld)solved / (ld)total) << endl;
}
