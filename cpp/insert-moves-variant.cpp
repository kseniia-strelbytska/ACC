#include "header.h"

/*
 Parameters idx and type describe each insert move
 
 idx=index of insertion, such that the resulting relator is
 a.first[0...idx] a.second a.first[idx+1...end]
 
 tag=0 - insert a.second into a.first
 tag=1 - insert a.first into a.second
 tag=2 - insert a.second^{-1} into a.first
 tag=3 - insert a.first^{-1} into a.second
 */

// returns the resulting relator when performing
// an insert move on relator {a}, with parameters idx and tag
node insertmove(node a, int idx, int tag){
    if(tag == 2)
        a.second = inverse(a.second);
    else if(tag == 3)
        a.first = inverse(a.first);
    
    node res = a;
        
    if(tag % 2 == 0){ // 0 or 2
        // insert a.second into a.first
        // to get a resulting relator of a.first[0...idx] a.second a.first[idx+1...end]
        
        res.first.clear();
        
        for(int i = 0; i <= idx; i++)
            res.first.push_back(a.first[i]);
        
        for(auto i: a.second)
            res.first.push_back(i);
        
        for(auto i = idx + 1; i < (int)(a.first.size()); i++)
            res.first.push_back(a.first[i]);
    }
    else{
        // insert a.first into a.second
        // to get a resulting relator of a.second[0...idx] a.first a.second[idx+1...end]
        
        res.second.clear();
        
        for(int i = 0; i <= idx; i++)
            res.second.push_back(a.second[i]);
        
        for(auto i: a.first)
            res.second.push_back(i);
        
        for(auto i = idx + 1; i < (int)(a.second.size()); i++)
            res.second.push_back(a.second[i]);
    }
    
    normalise(res.first);
    normalise(res.second);
    
    return res;
}

// updates the vector {ans} with insert moves of type {tag}
// Complexity O(size(relator1) * size(relator2)) complexity
void add_insertmoves(vector<pair<int, pair<int, int>>> &ans, int tag, node a){
    int n = (int)(a.first.size()), m = (int)(a.second.size());
    
    for(int i = -1; i < n; i++){
        // insert a.second into a.first after index i
        // to get a resulting relator of a.first[0...i] a.second a.first[i+1...end]

        int l1 = i, l2 = 0, r2 = m - 1, r1 = i + 1;
        
        // l1, r1 are pointers on a.first
        // l2, r2 are pointers on a.second
        // boundaries look like this: 0...a.first[l1]-a.second[l2]...a.second-[r2]a.first[r1]...end
        // terms can be cancelled on these boundaries

        // repeat while there are still uncanceled terms in a.second
        while(l2 <= r2){
            bool move = false;
            
            // check if a.first[l1] and a.second[l2] can be cancelled
            if(l1 >= 0 && abs(a.first[l1]) == abs(a.second[l2]) && a.first[l1] != a.second[l2]){
                move = true;
                l1--, l2++;
            }
            
            // check if a.seocnd[r2] and a.first[r1] can be cancelled
            if(r1 < n && l2 <= r2 && abs(a.second[r2]) == abs(a.first[r1]) && a.second[r2] != a.first[r1]){
                move = true;
                r2--, r1++;
            }

            // if not cancelation was done, we stop
            if(!move)
                break;
        }
        
        // if the whole a.second was cancelled,
        // we can continue performing cancelations on relator a.first
        if(l2 > r2){
            // a.first[l1] and a.first[r1] are now neighbouring
            // (since the inserted a.second has been cancelled out completely)
            while(l1 >= 0 && r1 < n && abs(a.first[l1]) == abs(a.first[r1]) && a.first[l1] != a.first[r1]){
                l1--, r1++;
            }
        }

        // calculate how many terms were cancelled out
        int cnt = (i - l1) + (r1 - i - 1);
        cnt += l2 + (m - 1 - r2);
        
        // update the vector {ans}
        ans.push_back({cnt, {i, tag}});
    }
}

// Functions returns a list of pairs {# of cancelations, {idx, tag}}
// (idx and tag format above)
vector<pair<int, pair<int, int>>> rank_insertmoves(node a){
    // inserting a.second into a.first;
    
    vector<pair<int, pair<int, int>>> ans;
    
    add_insertmoves(ans, 0, a);
    add_insertmoves(ans, 1, {a.second, a.first});
    add_insertmoves(ans, 2, {a.first, inverse(a.second)});
    add_insertmoves(ans, 3, {a.second, inverse(a.first)});
    
    sort(ans.rbegin(), ans.rend()); // important: descending order of # of cancellations

    return ans;
}

// ground truth (relies on concat() function). 
// Complexity O(size(relator1) * size(relator2)) complexity
vector<pair<int, pair<int, int>>> rank_insertmoves_truth(node a){
    // inserting a.second into a.first;
    
    int n = (int)(a.first.size()), m = (int)(a.second.size());

    vector<pair<int, pair<int, int>>> ans;
    
    for(int i = -1; i < n; i++){
        // insert after index i

        deque<int> l, mid, r;
        
        for(int j = 0; j <= i; j++)
            l.push_back(a.first[j]);
        
        for(auto j: a.second)
            mid.push_back(j);
        
        for(int j = i + 1; j < n; j++)
            r.push_back(a.first[j]);
        
        l = concat(l, mid);
        l = concat(l, r);
        
        int cnt = n + m - (int)(l.size());
        
        ans.push_back({cnt, {i, 0}});
    }
    
    sort(ans.rbegin(), ans.rend());
    
    return ans;
}

GreedyResult greedy_search_insertmoves(node start, int max_nodes, int max_relator_length){
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
    
    while(!q.empty()){
        auto v = q.top();
        q.pop();
        
//        auto *f = freopen("./output_cpp.txt", "a", stdout);
//        print(v.second);
//        fclose(f);
        
        mx = max(mx, (int)(v.second.first.size()) + (int)(v.second.second.size()));

        // if reached a trivial presentation
        if((int)(v.second.first.size()) + (int)(v.second.second.size()) == 2){
            trivial = true;
            trivial_node = v.second;
            
            break;
        }
        
        auto all_moves = rank_insertmoves(v.second);
        
        int neighbours_found = 0;
        
        for(int move = 0; move < (int)all_moves.size() && neighbours_found < 20; move++){
            auto to = insertmove(v.second, all_moves[move].second.first, all_moves[move].second.second); // node, index, tag
            
            pair<int, int> cost = {(int)(to.first.size()) + (int)(to.second.size()), v.first.second + 1};
            
            // if {to} hasn't been expanded and {cost} is better than current best for {to},
            // then push to the open set
            
            if((int)(to.first.size()) + (int)(to.second.size()) == 2){
                trivial = true;
                trivial_node = to;
                
                parent[to] = {v.second, move};
                
                break;
            }
            
            if((int)(to.first.size()) < max_relator_length && (int)(to.second.size()) < max_relator_length && !used.count(to)){
                neighbours_found += 1;
                
                used.insert(to);
                mp[to] = cost;
                  
                parent[to] = {v.second, move};
                q.push({cost, to});
            }
        }
        
        if((ll)(used.size()) >= max_nodes || trivial)
            break;
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
        
    return make_pair(trivial, path);
}
