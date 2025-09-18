#include "header.h"

// extracts the filename from file path 
string get_name(string file_path){
    string s;
    
    for(int i = (int)(file_path.size()) - 1; i >= 0 && file_path[i] != '/'; i--)
        s += file_path[i];
    
    reverse(s.begin(), s.end());
    
    return s;
}

void evaluate(vector<node> presentations, string output_pr, string output_pa, int max_nodes, int max_relator_length){
    string output_presentation = "/Users/kseniia/Desktop/programming/Projects/acc/results/" + output_pr;
    string output_path = "/Users/kseniia/Desktop/programming/Projects/acc/results/" + output_pa;
    
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
        cout.flush();
        
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
        
        if(ind == 604)
            break;
    }
    
    FILE *f = freopen(output_presentation.c_str(), "w", stdout);
    for(auto i: s_presentations)
        print(i);
    
    fclose(f);
    
    FILE *f1 = freopen(output_path.c_str(), "w", stdout);
    for(auto i: s_paths){
        for(auto j: i)
            cout << j << ' ';
        cout << endl;
    }
    
    fclose(f1);
    
    cout << solved << ' ' << total << ' ' << fixed << setprecision(3) << ((ld)solved / (ld)total) << endl;
}
