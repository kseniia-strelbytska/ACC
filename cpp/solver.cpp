 // typedef pair<pair<int, int>, node> node_info;

// similarity (default=0), k, l
// similarity is used purely in sortin gthe priority queue
typedef pair<pair<long double, pair<int, int>>, node> extended_node_info;

bool compare_eni(const extended_node_info &a, const extended_node_info &b){
    return a.first.first > b.first.first;
}

GreedyResult solver(node start, node finish, int max_nodes, int max_relator_length){
    auto neigh = neighbourhood_greedy_search_insertmovesrotate(finish, 1e2, max_relator_length);
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
    int expanded = 0, next_check = (int)(1e3);
    bool trivial = false;
    
    node trivial_node;
    
    int mx = 0;
    
    while(!q.empty()){
        if((ll)(used.size()) >= next_check){
            // we prune nodes which are yet to be expanded and are too far away from the neighbourhood of B
            next_check = (ll)(used.size()) + (int)(1e3);

            vector<extended_node_info> q_nodes;
            while(!q.empty()){
                auto v = q.top();
                q.pop();

                // get it's similarity 
                long double mx = 0.0;

                for(auto final_node: neigh){
                    int similarity = get_distance(v.second, final_node);
                    mx = max(mx, similarity);
                }

                q_nodes.push_back({{similarity, v.first.second}, v.second});
            }

            sort(q_nodes.begin(), q_nodes.end());

            // select top-200 and continue search with them only. 
            // in priority queue, we are interested in considering high similarity nodes first
            // so insert 1/ similarity (to consider 1/ s -> min first)
            
            for(int i = 0; i < min(int(q_nodes.size()), 200); i++){
                q_nodes[i].first.first = 1 / q_nodes[i].first.first;

                q.push(q_nodes[i]);
            }
        }

        auto v = q.top();
        q.pop();
        
        mx = max(mx, (int)(v.second.first.size()) + (int)(v.second.second.size()));
        
        // if reached a trivial presentation
        if((int)(v.second.first.size()) + (int)(v.second.second.size()) == 2){
            trivial = true;
            trivial_node = v.second;
            
            break;
        }
        
        auto all_moves = rank_insertmovesrotate(v.second);
        
        int neighbours_found = 0;
        
        for(int move = 0; move < (int)all_moves.size() && neighbours_found < 20; move++){
            auto to = insertmoverotate(v.second, all_moves[move].second[0], all_moves[move].second[1], all_moves[move].second[2]); // node, index, tag
            
            pair<int, int> cost = {min((int)(to.first.size()), (int)(to.second.size())), v.first.second + 1};
            
            // if {to} hasn't been expanded and {cost} is better than current best for {to},
            // then push to the open set
            
            if((int)(to.first.size()) + (int)(to.second.size()) == 2){
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
    
    cout << "Finished Greedy Search. " << (trivial ? "Trivialisation found" : "No trivialisation found") << endl;
        
    return make_pair(trivial, path);
}
