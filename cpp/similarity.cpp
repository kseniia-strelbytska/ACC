#include "header.h"

void add_qgrams(multiset<deque<int>> &qgrams, deque<int> relator, int q){
    if(q > (int)(relator.size()))
        return;
    
    deque<int> sub; // current substring
    
    for(int i = 0; i < relator.size(); i++){
        sub.push_back(relator[i]);
        
        if((int)(sub.size()) > q)
            sub.pop_front();
        
        if((int)(sub.size()) == q)
            qgrams.insert(sub);
    }
    
    // now consider cyclical qgrams
    // we shouldn't allow popped >= initial_len, as that means qgram is no longer cyclical
    int initial_len = sub.size(), popped = 0;
    
    for(int i = 0; i < relator.size() && popped < initial_len; i++){
        // check whether last term of {sub}} cancels with the current term
        if(!sub.empty() && abs(sub.back()) == abs(relator[i]) && sub.back() != relator[i]){
            sub.pop_back();
            
            // we know that the popped term must have come from inital {sub},
            // as no two neighbouring terms in {relator} cancel each other
            popped++;
            
            continue;
        }
        
        sub.push_back(relator[i]);
           
        if(int(sub.size()) > q){
            popped++;
            sub.pop_front();
        }
        
        if(popped == initial_len)
           break;
        
        if((int)(sub.size()) == q)
            qgrams.insert(sub);
    }
}

long double qgram_distance(deque<int> a, deque<int> b){
    multiset<deque<int>> ma, mb;
    
    for(int q = 5; q <= 11; q++){
        add_qgrams(ma, a, q);
        add_qgrams(mb, b, q);
    }
    
    multiset<deque<int>> m_intersection, m_union;
    set_intersection(ma.begin(), ma.end(), mb.begin(), mb.end(), inserter(m_intersection, m_intersection.begin()));
    set_union(ma.begin(), ma.end(), mb.begin(), mb.end(), inserter(m_union, m_union.begin()));

    long double j = 1;
        
    if(!m_union.empty())
        j = (long double)(m_intersection.size()) / (long double)(m_union.size());
    else if(!m_intersection.empty())
        j = 1.0 / (long double)(m_intersection.size());
        
    return j;
}

long double qgram_distance(node p1, node p2){
//    while(p1.first.size() < 5)
//        p1.first.push_back(0);
//    while(p1.second.size() < 5)
//        p1.second.push_back(0);
//    while(p2.first.size() < 5)
//        p2.first.push_back(0);
//    while(p2.second.size() < 5)
//        p2.second.push_back(0);
    
    long double s1 = (long double)(p1.first.size()) + (long double)(p2.first.size());
    long double s2 = (long double)(p1.second.size()) + (long double)(p2.second.size());
    
    long double res1 = qgram_distance(p1.first, p2.first);
    long double res2 = qgram_distance(p1.second, p2.second);
    
    long double distance = s1 / (s1 + s2) * res1 + s2 / (s1 + s2) * res2;
    
    return distance;
}

int get_distance(node a, node b){
//    a.first = get_smallest_rotation(a.first);
//    a.second = get_smallest_rotation(a.second);
//    
//    b.first = get_smallest_rotation(b.first);
//    b.second = get_smallest_rotation(b.second);
    
    normalise(a.first);
    normalise(a.second);
    normalise(b.first);
    normalise(b.second);
    
    vector<int> p1, p2;
    for(auto i: a.first)
        p1.push_back(i);
    for(auto i: a.second)
        p1.push_back(i);
    
    for(auto i: b.first)
        p2.push_back(i);
    for(auto i: b.second)
        p2.push_back(i);
    
    int n = (int)(p1.size()), m = (int)(p2.size());
    vector<vector<int>> dp(n, vector<int> (m));
    
    for(int i = 0; i < n; i++){
        for(int j = 0; j < m; j++){
            dp[i][j] = (int)(1e9);
            
            if(p1[i] == p2[j]){
                dp[i][j] = min(dp[i][j], (i - 1 >= 0 && j - 1 >= 0) ? dp[i - 1][j - 1] : max(i, j));
            }
            
            dp[i][j] = min(dp[i][j], (i - 1 >= 0) ? 1 + dp[i - 1][j] : 2 + j);
            dp[i][j] = min(dp[i][j], (j - 1 >= 0) ? 1 + dp[i][j - 1] : 2 + i);
        }
    }
    
    return dp[n - 1][m - 1];
}

// LCS = longest common substring.
int get_lcs(node a, node b){
    /*
     
     Having selected matching, considering first relators (same for a.second):
     We want to rotate a.first and b.first in such a way, so that ab^{-1} has the shortest possible length.
     The # of cancelations = longest suffix of a that matches reverse(prefix) of b.
     Therefore we need to find the longest common substring (LCS) of a and b (and then rotate both,
        so that suffix of a matches prefix of b)
     
     We build SuffixTree and run SuffixTree.getLongestCommonSubstring() on
     a.first+a.first+'#'+b.first+b.first+'$'
     
     LCS on this string will be the longest common substring of a.first+a.first and b.first+b.first
     Every cyclic substring of a.first is present in a.first+a.first.
     If LCS > min(|a.first|, |b.first|) -- trim it to smallest of the two lengths.
     
     */
    
    int mx = 0;
    for(int matching = 0; matching < 2; matching++){
        int total = 0; // sum of LCS for two relators
        
        for(int relator = 0; relator < 2; relator++){
            // denote -2='a', -1='b', 1='c', 2='d'
            map<int, char> mapping;
            mapping[-2] = 'a';
            mapping[-1] = 'b';
            mapping[1] = 'c';
            mapping[2] = 'd';
            
            string a1 = "", a2 = "";
            for(auto g: a.first){
                a1 += mapping[g];
            }
            
            for(auto g: b.first){
                a2 += mapping[g];
            }
                        
            string concats = a1 + a1 + "#" + a2 + a2 + "$";
            
            SuffixTree st;
            strcpy(st.text, concats.c_str());
            st.size1 = 2 * (int)(a1.size()) + 1; // size of the first string
            
            st.buildSuffixTree();
            string s = st.getLongestCommonSubstring();
            st.freeSuffixTreeByPostOrder(st.root);
                        
            total += min({(int)(s.size()), (int)a.first.size(), (int)b.first.size()});
            
            swap(a.first, a.second);
            swap(b.first, b.second);
        }
        
        mx = max(mx, total);
        
        swap(b.first, b.second);
    }
    
    int len = (int)(a.first.size()) + (int)(b.first.size()) + (int)(a.second.size()) + (int)(b.second.size()) - 2 * mx;
    
    return len;
}

// max_nodes = max size of neighbourhood
pair<vector<node>, map<node, pair<node, vector<int>>>> neighbourhood_greedy_search_insertmovesrotate(node start, int max_nodes, int max_relator_length){
    priority_queue<node_info, vector<node_info>, greater<node_info>> q;
    
    // 'open set'; stores {{k=presentation length, l=length from the start}, node}
    q.push({{(int)(start.first.size()) + (int)(start.second.size()), 0}, start});
    
//    int mn = (int)(1e9);
//    for(auto target: all_trivial)
//        mn = min(mn, get_distance(start, target));
//    q.push({{mn, 0}, start});
    
    // stores best pair (k, l) for each node
    map<node, pair<int, int>> mp;
    mp[start] = q.top().first;
    
    // stores the parent and the previous move for each node
    // move is now defined by three numbers: index, tag, # of rotation
    map<node, pair<node, vector<int>>> parent;
    
    // 'closed set'; a set of all expanded nodes (shouldn't be expanded again)
    set<node> used;
    used.insert(start);
    
    int expanded = 0;
    bool trivial = false;
    
    node trivial_node;
    
    int mx = 0;
    
//    node finish = {{-2, -2, -1, -1, -1, -1, 2, 1}, {-1, -2, 1, 2, -1}};
//    node finish2 = {{-2, -2, -1, -1, -1, -1, 2, 1}, {1, -2, -1, 2, 1}};
    
    int tstep = 0;
    
    while(!q.empty()){
        tstep++;
        
        auto v = q.top();
        q.pop();
                
//        auto *f = freopen("./output_cpp.txt", "a", stdout);
//        print(v.second);
//        fclose(f);
        
        mx = max(mx, (int)(v.second.first.size()) + (int)(v.second.second.size()));
        
        // if reached a trivial presentation
//        if((int)(v.second.first.size()) + (int)(v.second.second.size()) == 2){
//            trivial = true;
//            trivial_node = v.second;
//            
//            break;
//        }
        
        auto all_moves = rank_insertmovesrotate(v.second);
        
        int neighbours_found = 0;
        
        for(int move = 0; move < (int)all_moves.size() && neighbours_found < 20; move++){
            auto to = insertmoverotate(v.second, all_moves[move].second[0], all_moves[move].second[1], all_moves[move].second[2]); // node, index, tag
            
            pair<int, int> cost = {(int)(to.first.size()) + (int)(to.second.size()), v.first.second + 1};
            
//            mn = (int)(1e9);
//            for(auto target: all_trivial)
//                mn = min(mn, get_distance(to, target));
//
//            pair<int, int> cost = {mn, v.first.second + 1};
           
            // if {to} hasn't been expanded and {cost} is better than current best for {to},
            // then push to the open set
            
//            if((int)(to.first.size()) + (int)(to.second.size()) == 2){
//                trivial = true;
//                trivial_node = to;
//                
//                parent[to] = {v.second, all_moves[move].second};
//                
//                break;
//            }
            
            if((int)(to.first.size()) < max_relator_length && (int)(to.second.size()) < max_relator_length && !used.count(to)){
                neighbours_found += 1;
                
                used.insert(to);
                mp[to] = cost;
                
                parent[to] = {v.second, all_moves[move].second};
                q.push({cost, to});
            }
        }
        
        if((ll)(used.size()) >= max_nodes || trivial)
            break;
    }
        
    vector<node> neighbourhood;
    
    for(auto i: used)
        neighbourhood.push_back(i);
        
    // parent is essential to be able to reconstruct the path
    return {neighbourhood, parent};
}

// recreates path from a -> b
pair<vector<node>, deque<vector<int>>> get_path(map<node, pair<node, vector<int>>> &parent, node a, node b){
    vector<node> nodes = {b};
    deque<vector<int>> path;
    
    while(b != a){
        path.push_back(parent[b].second);
        b = parent[b].first;
        
        nodes.push_back(b);
    }
    
    reverse(nodes.begin(), nodes.end());
    reverse(path.begin(), path.end());
    
    return {nodes, path};
}

// we know start -> move -> finish
// we need to find start <- movereverse <- finish
// and return both paths in terms of the original AC' moves
pair<deque<int>, deque<int>> convert_moves(node start, node finish, vector<int> move){
    // we know the sequence of moves: index, tag, rotate.
    
    deque<int> moves, reversed_moves;
    
    // mapping of conjugation to AC' moves
    map<int, int> conjugation_first;
    conjugation_first[-2] = 5;
    conjugation_first[-1] = 11;
    conjugation_first[1] = 7;
    conjugation_first[2] = 9;
    
    map<int, int> conjugation_second;
    conjugation_second[-2] = 6;
    conjugation_second[-1] = 4;
    conjugation_second[1] = 8;
    conjugation_second[2] = 10;
    
    int tag = move[1], rotation = move[2];
    
    cout << move[0] << ' ' << tag << ' ' << rotation << endl;
    
    if(tag % 2 == 0){
        // we insert start.second into start.first
        
        // we will convert an insert-rotate move into a set of simple AC' moves (the 12 ones used in the classical greedy search)
        
        vector<int> terms;
        
        for(int rotate = 0; rotate < rotation; rotate++){ // perform rotations as needed
            // to rotate one last term we apply a conjugation = that term
            terms.push_back(start.second.back());
            
            moves.push_back(conjugation_second[start.second.back()]);
            reversed_moves.push_back(conjugation_second[-start.second.back()]);
            
            start.second.push_front(start.second.back()); // rotate by one to the right
            start.second.pop_back();
        }
                
        // step 1: cyclic shift to the right = conjugation
        for(int i = (int)(start.first.size()) - 1; i > move[0]; i--){
            moves.push_back(conjugation_first[start.first[i]]);
            reversed_moves.push_back(conjugation_first[-start.first[i]]);
        }
        
        // step 2: multiplication by a.second (rotated as needed)
        // if tag=2 (inverse on a.second) type 1, else type 3
        moves.push_back((tag == 2 ? 1 : 3));
        reversed_moves.push_back((tag == 2 ? 3 : 1));
        
        // step 3: cyclic shift to the left (i.e. shifting back to initial)
        for(int i = move[0] + 1; i < (int)(start.first.size()); i++){
            moves.push_back(conjugation_first[-start.first[i]]);
            reversed_moves.push_back(conjugation_first[start.first[i]]);
        }
        
        // step 4: return the a.second back to its original form (cyclic shifts to the left)
        for(int term = (int)(terms.size()) - 1; term >= 0; term--){
            moves.push_back(conjugation_second[-terms[term]]);
            reversed_moves.push_back(conjugation_second[terms[term]]);
        }
    }
    else{
        // we insert start.first into start.second
        
        // we will convert an insert-rotate move into a set of simple AC' moves (the 12 ones used in the lassical greedy search)
        
        vector<int> terms;
        
        for(int rotate = 0; rotate < rotation; rotate++){ // perform rotations as needed
            // to rotate one last term we apply a conjugation = that term
            terms.push_back(start.first.back());
            
            moves.push_back(conjugation_first[start.first.back()]);
            reversed_moves.push_back(conjugation_first[-start.first.back()]);
            
            start.first.push_front(start.first.back()); // rotate by one to the right
            start.first.pop_back();
        }
        
        // step 1: cyclic shift to the right = conjugation
        for(int i = (int)(start.second.size()) - 1; i > move[0]; i--){
            moves.push_back(conjugation_second[start.second[i]]);
            reversed_moves.push_back(conjugation_second[-start.second[i]]);
        }
        
        // step 2: multiplication by a.first (rotated as needed)
        // if tag=3 (inverse on a.second) type 2, else type 0
        moves.push_back((tag == 3 ? 2 : 0));
        reversed_moves.push_back((tag == 3 ? 0 : 2));
        
        // step 3: cyclic shift to the left (i.e. shifting back to initial)
        for(int i = move[0] + 1; i < (int)(start.second.size()); i++){
            moves.push_back(conjugation_second[-start.second[i]]);
            reversed_moves.push_back(conjugation_second[start.second[i]]);
        }
        
        // step 4: return the a.first back to its original form (cyclic shifts to the left)
        for(int term = (int)(terms.size()) - 1; term >= 0; term--){
            moves.push_back(conjugation_first[-terms[term]]);
            reversed_moves.push_back(conjugation_first[terms[term]]);
        }
    }
    
    reverse(reversed_moves.begin(), reversed_moves.end());
    
    return {moves, reversed_moves};
}

vector<node> f = {{{-2, -2, 1, 2, -1, -2, 1}, {-2, -2, -2, -2, 1, -2, -2, -2, 1, 1}},
    
    {{-2, -2, 1, 2, -1, -2, 1}, {-2, -2, -2, 1, -2, -1, -2, 1, 1}},

    {{-2, -2, 1, 2, -1, -2, 1 }, {-2, -2, -2, 1, -2, -2, -1, 2, 2, 1}},

    {{-2, -2, 1, 2, -1, -2, 1 }, { -2, -2, 1, -2, -1, -1, 2, 2, 1 }},

    {{-2, -2, 1, 2, -1, -2, 1 }, { -2, -2, 1, -2, 1, -2, -1, -1, 2, 1 }},

    {{-2, -2, 1, 2, -1, -2, 1 }, { -2, -2, 1, -2, 1, -2, 1, -2, -1 }},

    {{-2, -2, 1, 2, -1, -2, 1 }, { -2, -1, -2, 1, -2, -1, 2, 1, -2, 1 }},

    {{-2, -2, 1, 2, -1, -2, 1 }, { -2, -2, -1, 2, -1, 2, 1, -2, 1 }},

    {{-2, -2, 1, 2, -1, -2, 1 }, { -2, -2, -1, -2, 1, 1 }}
};

// deoth starts at 1
pair<bool, deque<int>> guided_exploration(node start, node finish, int depth){
    if(depth >= 3)
        return {false, {}};
    
    ofstream mainout("/Users/kseniia/Desktop/programming/Projects/ACC/results/console_output.txt");
    
    print(mainout, start);
    print(mainout, finish);
    
    auto neigh1 = neighbourhood_greedy_search_insertmovesrotate(start, (int)(5000), 18);
    auto neigh2 = neighbourhood_greedy_search_insertmovesrotate(finish, (int)(500), 18);
    
    for(auto node: neigh1.second){
        print(mainout, node.first);
    }

    set<int> st1, st2;
    for(auto i: neigh1.first){
        for(int j = 0; j < f.size(); j++)
            if(f[j] == i)
                st1.insert(j);
    }
    
    for(auto i: neigh2.first){
        for(int j = 0; j < f.size(); j++)
            if(f[j] == i)
                st2.insert(j);
    }
    
    cout << "Neigh 1:" << endl;
    for(auto i: st1)
        cout << i << ' ';
    cout << endl;
    
    cout << "Neigh 2:" << endl;
    for(auto i: st2)
        cout << i << ' ';
    cout << endl;
    
    vector<pair<long double, pair<node, node>>> candidates;
    
    for(auto i: neigh1.first){
        for(auto j: neigh2.first){
            if(i == j){
                // p1.first = nodes, p1.second = path
                auto p1 = get_path(neigh1.second, start, i);
                auto p2 = get_path(neigh2.second, finish, j);
                                
                deque<int> path;
                // converting start -> i in terms of AC' moves
                for(int node = 0; node < int(p1.first.size()) - 1; node++){
                    auto moves = convert_moves(p1.first[node], p1.first[node + 1], p1.second[node]);
                    
                    for(auto move: moves.first)
                        path.push_back(move);
                }
                
                // converting j -> finish in terms of AC' moves
                // we use the reversed_moves from conver_moves function
                for(int node = int(p2.first.size()) - 2; node >= 0; node--){
                    auto moves = convert_moves(p2.first[node], p2.first[node + 1], p2.second[node]);
                    
                    for(auto move: moves.second)
                        path.push_back(move);
                }
                
                return {true, path};
            }
            
            long double distance = qgram_distance(i, j);
//            distance -= (i.first.size() + i.second.size() + j.first.size() + j.second.size());
            
            candidates.push_back({distance, {i, j}});
        }
    }
    
    cout << "Finished traversing pairs " << (int)(candidates.size()) << endl;
    
//    ofstream fout("/Users/kseniia/Desktop/programming/Projects/ACC/results/qgramdistance.txt");
//    for(auto i: candidates)
//        fout << fixed << setprecision(10) << i.first << ' ';
//    fout << endl;
//    fout.close();
    
    sort(candidates.rbegin(), candidates.rend());
    
    ll ind = -1;
    
    // we can now try to select 10-20 most similar pairs
    // and run guided_exploration for them
    
    for(int i = 0; i < min((int)(candidates.size()), 100); i++){
        cout << fixed << setprecision(10) << candidates[i].first << endl;
        print(cout, candidates[i].second.first);
        print(cout, candidates[i].second.second);
        cout << "_____starting GE " << depth << endl;
        
        auto result = guided_exploration(candidates[i].second.first, candidates[i].second.second, depth + 1);
        cout << "_____finished GE " << depth << endl;
        
        if(result.first == true){
            // found a path!
            
            // describing moves
            
            deque<int> path;
            auto p1 = get_path(neigh1.second, start, candidates[i].second.first);
            auto p2 = get_path(neigh2.second, finish, candidates[i].second.second);
            
            // converting start -> i in terms of AC' moves
            for(int node = 0; node < int(p1.first.size()) - 1; node++){
                auto moves = convert_moves(p1.first[node], p1.first[node + 1], p1.second[node]);
                
                for(auto move: moves.first)
                    path.push_back(move);
            }
            
            for(auto move: result.second)
                path.push_back(move);
            
            // converting j -> finish in terms of AC' moves
            // we use the reversed_moves from conver_moves function
            for(int node = int(p2.first.size()) - 2; node >= 0; node--){
                auto moves = convert_moves(p2.first[node], p2.first[node + 1], p2.second[node]);
                
                for(auto move: moves.second)
                    path.push_back(move);
            }
            
            return {true, path};
        }
    }
    
    return {false, {}};
}

// similarity (default=0), k, l
// similarity is used purely in sortin gthe priority queue
typedef pair<pair<long double, pair<int, int>>, node> extended_node_info;

bool compare_eni(const extended_node_info &a, const extended_node_info &b){
   return a.first.first > b.first.first;
}

GreedyResult solver(node start, node finish, int max_nodes, int max_relator_length){
   auto neigh = neighbourhood_greedy_search_insertmovesrotate(finish, 1e2, max_relator_length).first;
    set<node> finishing;
    for(auto i: neigh)
        finishing.insert(i);
    
   // neighbourhood of B

   priority_queue<extended_node_info, vector<extended_node_info>, greater<extended_node_info>> q;
   
   // 'open set'; stores {{k=presentation length, l=length from the start}, node}
   q.push({{(long double)(1e9), {min((int)(start.first.size()), (int)(start.second.size())), 0}}, start});
   
   // stores best pair (k, l) for each node
   map<node, pair<int, int>> mp;
   mp[start] = q.top().first.second;
   
   // stores the parent and the previous move for each node
   // move is now defined by three numbers: index, tag, # of rotation
   map<node, pair<node, vector<int>>> parent;
   
   // 'closed set'; a set of all expanded nodes (shouldn't be expanded again)
   set<node> used;
   // next_check -- when size of used reaches >= next_check, we prune nodes
   int expanded = 0, next_check = (int)(1e5);
   bool trivial = false;
   
   node trivial_node;
   
   int mx = 0;
   
   while(!q.empty()){
       if((ll)(used.size()) >= next_check){
           cout << "Pruning " << next_check << endl;
           // we prune nodes which are yet to be expanded and are too far away from the neighbourhood of B
           next_check = (ll)(used.size()) + (int)(1e5);

           vector<extended_node_info> q_nodes;
           while(!q.empty()){
               auto v = q.top();
               q.pop();

               // get it's similarity
               long double mx = 0.0;

               for(auto final_node: neigh){
                   long double similarity = get_distance(v.second, final_node);
                   mx = max(mx, similarity);
               }

               q_nodes.push_back({{mx, v.first.second}, v.second});
           }

           sort(q_nodes.rbegin(), q_nodes.rend());

           // select top-200 and continue search with them only.
           // in priority queue, we are interested in considering high similarity nodes first
           // so insert 1/ similarity (to consider 1/ s -> min first)
           
           vector<bool> present(f.size());

           for(int i = 0; i < min(int(q_nodes.size()), (int)(1e4)); i++){
               q_nodes[i].first.first = (int)(1e9);

               q.push(q_nodes[i]);
               for(int j = 0; j < f.size(); j++)
                   if(f[j] == q_nodes[i].second)
                       present[j] = true;
           }
           
           for(int j = 0; j < f.size(); j++)
               if(present[j])
                   cout << j << ' ';
           cout << endl;
       }

       auto v = q.top();
       q.pop();
       
       mx = max(mx, (int)(v.second.first.size()) + (int)(v.second.second.size()));
       
       // if reached a trivial presentation
       if((int)(v.second.first.size()) + (int)(v.second.second.size()) == 2 || finishing.count(v.second)){
           trivial = true;
           trivial_node = v.second;
           
           break;
       }
       
       auto all_moves = rank_insertmovesrotate(v.second);
       
       int neighbours_found = 0;
       
       for(int move = 0; move < (int)all_moves.size() && neighbours_found < 20; move++){
           auto to = insertmoverotate(v.second, all_moves[move].second[0], all_moves[move].second[1], all_moves[move].second[2]); // node, index, tag
           
           pair<int, int> cost = {min((int)(to.first.size()), (int)(to.second.size())), v.first.second.second + 1};
           
           // if {to} hasn't been expanded and {cost} is better than current best for {to},
           // then push to the open set
           
           if((int)(to.first.size()) + (int)(to.second.size()) == 2 || finishing.count(to)){
               trivial = true;
               trivial_node = to;
               
               parent[to] = {v.second, all_moves[move].second};
               
               break;
           }
           
           if((int)(to.first.size()) < max_relator_length && (int)(to.second.size()) < max_relator_length && !used.count(to)){
               used.insert(to);
               mp[to] = cost;
               
               parent[to] = {v.second, all_moves[move].second};
               q.push({{(long double)(1e9), cost}, to});
           }
       }
       
       if((ll)(used.size()) >= max_nodes || trivial)
           break;
   }
       
   vector<vector<int>> path;
       
   if(trivial){
       // trace the path back from the trivial node to the starting node
       while(trivial_node != start){
           path.push_back(parent[trivial_node].second);
           trivial_node = parent[trivial_node].first;
       }
   }

   reverse(path.begin(), path.end());
   
   cout << "Finished Greedy Search. " << (trivial ? "Trivialisation found" : "No trivialisation found") << " " << (ll)(used.size()) << ' ' << "expanded" << endl;
       
   return make_pair(trivial, path);
}
