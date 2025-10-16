#include "header.h"

// takes presentations {a, b}: pairs of nodes a and b, trying to get distance to correlate with len(path(a, b))
// ans lens: ground truth distance between nodes in each pair
// returns {m1, m2, m3, total number of pairs}
vector<int> order_comparator(vector<pair<node, node>> presentations, vector<int> lens){
    // motivation: for two pairs of nodes a1 and a2, an ideal similarity metric f
    // will have f(a1) < f(a2) iff distance a1 > distance a2
    // therefore we are checking which of the metrics has a higher proportion of node pairs ordered correctly.
    
    int n = (int)(presentations.size());
    
    int total = n * (n - 1) / 2;
    int qscore_correct = 0, editdistance_correct = 0, lcsdistance_correct = 0;

    for(int i = 0; i < n; i++){
        for(int j = i + 1; j < n; j++){
            long double qscorei = qgram_distance(presentations[i].first, presentations[i].second);
            long double qscorej = qgram_distance(presentations[j].first, presentations[j].second);
            
            int edit_distancei = get_distance(presentations[i].first, presentations[i].second);
            int edit_distancej = get_distance(presentations[j].first, presentations[j].second);
            
            int lcs_distancei = get_lcs(presentations[i].first, presentations[i].second);
            int lcs_distancej = get_lcs(presentations[j].first, presentations[j].second);

            // -1 = p[i] is closer together than p[j]
            // 0 = equidistant
            // 1 = p[i] is farther apart than p[j]
            
            int sign0 = (lens[i] < lens[j] ? -1 : (lens[i] == lens[j] ? 0 : 1));
            int sign1 = (qscorei < qscorej ? -1 : (qscorei == qscorej ? 0 : 1));
            int sign2 = (edit_distancei > edit_distancej ? -1 : (edit_distancei == edit_distancej ? 0 : 1));
            int sign3 = (lcs_distancei > lcs_distancej ? -1 : (lcs_distancei == lcs_distancej ? 0 : 1));
            
            if(sign0 == sign1)
                qscore_correct++;
            if(sign0 == sign2)
                editdistance_correct++;
            if(sign0 == sign3)
                lcsdistance_correct++;
        }
        
        cout << i << endl;
    }
    
    return {qscore_correct, editdistance_correct, lcsdistance_correct, total};
}

void compare_by_order(vector<vector<node>> all_paths){
    srand(1);
    
    int tot = 0;
    
    for(auto i: all_paths)
        tot += (int)(i.size());
    
    cout << tot << endl; // 9852
    
    vector<pair<node, node>> presentations;
    vector<int> lens;
    
    for(auto path: all_paths){
        for(int i = 0; i < path.size(); i++){
            for(int j = i + 1; j < path.size(); j++){
                presentations.push_back({path[i], path[j]});
                lens.push_back(j - i);
            }
        }
    }
    
    vector<pair<node, node>> sampled_presentations;
    vector<int> sampled_lens;
    
    int sample_size = (int)(2e3);
    
    for(int sample = 0; sample < sample_size; sample++){
        int idx = rand() % (int)(presentations.size());
        
        sampled_presentations.push_back(presentations[idx]);
        sampled_lens.push_back(lens[idx]);
        
        swap(presentations[idx], presentations[presentations.size() - 1]);
        presentations.pop_back();
        
        swap(lens[idx], lens[lens.size() - 1]);
        lens.pop_back();
    }
        
    auto result = order_comparator(sampled_presentations, sampled_lens);
    
    for(auto i: result)
        cout << i << ' ';
    cout << endl;
}

// format: -1 2 1 -2 -2 | -1 2

// to test
node presentation(string line){
    node ans;
    
    bool separated = false;
    
    for(int i = 0; i < line.size(); i++){
        if(line[i] == ' ')
            continue;
        
        if(line[i] == '|'){
            separated = true;
            swap(ans.first, ans.second);
            
            continue;
        }
        
        int sign = 1;
        if(line[i] == '-'){
            sign = -1;
            i++;
        }
        
        ans.second.push_back(sign * (line[i] - '0'));
    }
    
    return ans;
}

vector<vector<node>> extract_paths(string path){
    ifstream in(path.c_str());
    
    vector<vector<node>> all_paths;
    
    if(in.is_open()){
        int n;
        while(in >> n){
            vector<node> vec;
            
            string line;
            getline(in, line);
            
            for(int i = 0; i < n + 1; i++){
                string line;
                getline(in, line);
                                
                vec.push_back(presentation(line));
            }
            
            all_paths.push_back(vec);
        }
    }
    else{
        cout << "Error when opening " << path << endl;
    }
    
    return all_paths;
}

struct node_pair{
    int distance;
    long double qgram;
    int edit, lcs, path, ind1, ind2;
    
    void print(ostream &stream){
        stream << distance << ' ' << fixed << setprecision(5) << qgram;
        stream << fixed << setprecision(0) << ' ' << edit << ' ' << lcs << ' ' << path << ' ' << ind1 << ' ' << ind2 << endl;;
    }
};

// comparator; sorts in decreasing order of qgram scores (i.e. more similar first)
bool node_pair_qgram(const node_pair &a, const node_pair &b){
    return a.qgram > b.qgram;
}

void analysis(vector<vector<node>> all_paths, string dir_path){
    vector<node_pair> all_pairs;
    
    int mx = 0;
    
    // ind1 and ind2 are 1-indexed when appended
    
    for(int path = 0; path < (int)all_paths.size(); path++){
        cout << path << endl;
        for(int ind1 = 0; ind1 < all_paths[path].size(); ind1++){
            for(int ind2 = ind1 + 1; ind2 < all_paths[path].size(); ind2++){
                all_pairs.push_back({ind2 - ind1,
                    qgram_distance(all_paths[path][ind1], all_paths[path][ind2]),
                    get_distance(all_paths[path][ind1], all_paths[path][ind2]), get_lcs(all_paths[path][ind1], all_paths[path][ind2]), path + 1, ind1 + 1, ind2 + 1});
            }
        }
    }
    
    // save all pairs (distances) to a file to access via python plotting script
    ofstream out(dir_path + "/results/all_pairs.txt");
    for(auto p: all_pairs){
        p.print(out);
    }
    
    return;
    
    // for each distance, stores {similarity, node pair}
    // path lengths no longer than 300
    vector<vector< node_pair >> qgram(300);
    
    for(auto i: all_pairs){
        qgram[i.distance].push_back(i);
    }
    
    for(int i = 0; i < 300; i++)
        if(!qgram[i].empty()){
            sort(qgram[i].begin(), qgram[i].end(), node_pair_qgram);
        }
    
    for(int i = 0; i <= 10; i++){
        qgram[5][i].print(cout);
    }
}
