#include "header.h"

deque<int> get_smallest_rotation(deque<int> a){
    deque<int> mn = a;
    
    for(int i = 0; i < int(a.size()); i++){
        a.push_front(a.back());
        a.pop_back();
        
        auto b = a;
        normalise(b);
        
        if(int(b.size()) < int(mn.size()))
            mn = b;
        
        mn = min(mn, b);
    }
    
    return mn;
}

void analyse(vector<node> presentations, string sol_path){
    ifstream stream(sol_path.c_str());
    
    map<node, int> nodes;
    map<deque<int>, int> relators;
    
    vector<int> path_length(1000);
            
    if(stream.is_open()){
        int n, ind = -1;
        
        vector<vector<node>> paths;
        vector<vector<vector<int>>> all_moves;
        vector<int> lens;
        
        while(stream >> n){
            ind++;
            
            lens.push_back(n);
            
            vector<vector<int>> moves;
            for(int i = 0; i < n; i++){
                int a, b, c;
                stream >> a >> b >> c;
                
                moves.push_back({a, b, c});
            }
            
            all_moves.push_back(moves);
            
            node start = presentations[ind];
            
            vector<node> interm_nodes = {start};

            for(auto i: moves){
                start = insertmoverotate(start, i[0], i[1], i[2]);
                interm_nodes.push_back(start);
            }
            
            paths.push_back(interm_nodes);
            
            for(auto &i: interm_nodes){
                i.first = get_smallest_rotation(i.first);
                i.second = get_smallest_rotation(i.second);
                
                nodes[i]++;
                relators[i.first]++;
                relators[i.second]++;
            }
            
            path_length[n]++;
        }
        stream.close();

//        ofstream out("/Users/kseniia/Desktop/programming/Projects/ACC/results/solved_paths_rotate_MS_pathlenhistogram.txt");
//        
//        for(int i = 0; i < 400; i++){
//            out << int(presentations[i].first.size()) + int(presentations[i].second.size()) << ' ' << lens[i] << endl;
//        }
//        
//        out.close();
//        
//        ofstream out2("/Users/kseniia/Desktop/programming/Projects/ACC/results/solved_paths_rotate_MS_explicit_paths_smallestrotation.txt");
//        
//        for(auto i: paths){
//            out2 << (int)(i.size()) - 1 << endl;
//            
//            for(auto j: i){
//                j.first = get_smallest_rotation(j.first);
//                j.second = get_smallest_rotation(j.second);
//                print(out2, j);
//            }
//        }
//        out2.close();
        // CHANGE: FOR SMALLEST ROTATIONS
        
        vector<pair<int, node>> nodes_s;
        vector<pair<int, deque<int>>> relators_s;
        
        for(auto i: nodes)
            nodes_s.push_back({i.second, i.first});
        
        for(auto i: relators)
            relators_s.push_back({i.second, i.first});
        
        sort(nodes_s.rbegin(), nodes_s.rend());
        sort(relators_s.rbegin(), relators_s.rend());
        
        ofstream out3("/Users/kseniia/Desktop/programming/Projects/ACC/results/solved_paths_rotate_MS_analyse.txt");
        
        for(int i = 0; i < 40; i++){
            out3 << nodes_s[i].first << ' ';
            print(out3, nodes_s[i].second);
        }
        
        out3 << endl;
        for(int i = 0; i < 40; i++){
            out3 << relators_s[i].first << ' ';
            print(out3, relators_s[i].second);
        }
        
        out3.close();
        
//        ofstream out4("/Users/kseniia/Desktop/programming/Projects/ACC/results/solved_paths_rotate_MS_explicit_paths_plus_moves_smallestrotation.txt");
//        
//        int path_ind = -1;
//        for(auto i: paths){
//            path_ind++;
//            
//            out4 << (int)(i.size()) - 1 << endl;
//
//            int move_ind = -2;
//            for(auto j: i){
//                move_ind++;
//                
//                if(move_ind >= 0){
//                    for(auto p: all_moves[path_ind][move_ind])
//                        out4 << p << ' ';
//                    out4 << endl;
//                }
//                
//                j.first = get_smallest_rotation(j.first);
//                j.second = get_smallest_rotation(j.second);
//                print(out4, j);
//            }
//        }
//        out4.close();
    }
    else{
        cout << "Error when opening " << sol_path << endl;
    }
}
