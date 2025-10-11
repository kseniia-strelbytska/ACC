#include "header.h"

// returns a pair {True/False whether trivialisation was found, path if True else {}}
pair<bool, vector<int>> greedy_search(node start, int max_nodes, int max_relator_length){
    priority_queue<node_info, vector<node_info>, greater<node_info>> q;
    
    // 'open set'; stores {{k=presentation length, l=length from the start}, node}
    q.push({{(int)(start.first.size()) + (int)(start.second.size()), 0}, start});
    
    // stores best pair (k, l) for each node
    map<node, pair<int, int>> mp;
    mp[start] = q.top().first;
    
    // stores the parent and the previous move for each node
    map<node, pair<node, int> > parent;
    
    // 'closed set'; a set of all expanded nodes (shouldn't be expanded again)
    set<node> used;
    int expanded = 0;
    bool trivial = false;
    
    node trivial_node;
    
    int mx = 0;
    
    while(!q.empty() && expanded < max_nodes){
        auto v = q.top();
        q.pop();
        
        mx = max(mx, (int)(v.second.first.size()) + (int)(v.second.second.size()));
        
        // if reached a trivial presentation
        if((int)(v.second.first.size()) + (int)(v.second.second.size()) == 2){
            trivial = true;
            trivial_node = v.second;
            
            break;
        }
        
        // if the node has laready been expanded
        if(used.count(v.second))
            continue;
        
        expanded++;
        used.insert(v.second);
        
        for(int move = 0; move < 12; move++){
            auto to = ACMove(v.second, move);
            
            // calculate the 'cost' to reach node {to}
            pair<int, int> cost = {(int)(to.first.size()) + (int)(to.second.size()), v.first.second + 1};
            
            // if {to} hasn't been expanded and {cost} is better than current best for {to},
            // then push to the open set
            if((int)(to.first.size()) + (int)(to.second.size()) < max_relator_length && !used.count(to) && (!mp.count(to) || mp[to] < cost)){
                mp[to] = cost;
                  
                parent[to] = {v.second, move};
                q.push({cost, to});
            }
        }
    }
    
    vector<int> path;
        
    if(trivial){
        // trace the path back from the trivial node to the starting node
        while(trivial_node != start){
            path.push_back(parent[trivial_node].second);
            trivial_node = parent[trivial_node].first;
        }
    }

    reverse(path.begin(), path.end());
    
    cout << "Finished Greedy Search. " << (trivial ? "Trivialisation found" : "No trivialisation found") << endl;
        
    return {trivial, path};
}

//int main(){
//    node start = {{-1, 2, 1, -2, -2}, {-1, -1, -2, -2, 1, -2}};
//    
//    auto res = greedy_search(start);
//    
//    cout << res.first << endl;
//    cout << (ll)(res.second.size()) << endl;
//    
//    for(auto i: res.second)
//        cout << i << ' ';
//    cout << endl;
//     
//    return 0;
//}
//
