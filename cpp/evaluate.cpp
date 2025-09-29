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

void random_shuffle(vector<node> &presentations){
    srand(1);
    
    vector<int> idx;
    for(int i = 0; i < (int)presentations.size(); i++)
        idx.push_back(i);
    
    vector<node> shuffled;
    
    while((int)idx.size() > 0){
        int pos = rand() % (int)idx.size();
        
        shuffled.push_back(presentations[pos]);
        swap(idx[pos], idx[(int)idx.size() - 1]);
        idx.pop_back();
    }
    
    presentations = shuffled;
}

void evaluate(pair<bool, vector<int>> (*greedy_search_variant)(node, int, int), vector<node> presentations, string output_presentation, string output_path, int max_nodes, int max_relator_length){
    
//    random_shuffle(presentations);
    
    vector<node> s_presentations;
    vector<vector<int>> s_paths;
    
    ll solved = 0, total = (ll)(presentations.size()), ind = 0;
    int solved_small = 0, solved_large = 0;
    
    bool const test_large = false;
    
    for(auto i: presentations){
        ind++;
                
//            print(cout, i);
        auto result = greedy_search_variant(i, max_nodes, max_relator_length);
        
        if(result.first == true){
            solved++;
            
            s_presentations.push_back(i);
            s_paths.push_back(result.second);
        }
        
        cout << "Solved: " << solved << "/" << ind << endl;
        
        if(ind >= 600)
            break;
    }
    cout << "Result: " << solved_small << "/400 " << solved_large << "/17" << endl;
    
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
