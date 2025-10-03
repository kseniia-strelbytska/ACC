#include "header.h"

struct prefixHash{
    vector<ll> pref, pw;
    vector<ull> pref_ull, pw_ull;
    
    // the first hash will be calculated modulo 1e9 + 7,
    // and the second modulo 2^64 (automatic in unsigned long long)
    ll const mod = (ll)(1e9 + 7), base = 7;
    
    void build(deque<int> a){
        ll n = a.size();
        
        pw.resize(n + 1);
        pw_ull.resize(n + 1);
        
        pref.resize(n);
        pref_ull.resize(n);
        
        // calculate all base^p for 0 <= p <= n in pw[p]
        pw[0] = pw_ull[0] = 1;
        for(int i = 1; i <= n; i++){
            pw[i] = (pw[i - 1] * base) % mod;
            pw_ull[i] = pw_ull[i - 1] * base;
        }
        
        for(int i = 0; i < n; i++){
            if(i != 0)
                pref[i] = (pref[i - 1] * base) % mod, pref_ull[i] = pref_ull[i - 1] * base;
            
            // make generators positive by adding 3
            pref[i] = (pref[i] + (a[i] + 3)) % mod;
            pref_ull[i] += a[i] + 3;
        }
    }
    
    pair<ll, ull> get_hash(ll l, ll r){
        ll ans = pref[r];
        ull ans_ull = pref_ull[r];
        
        if(l != 0){
            ans = (ans - (pref[l - 1] * pw[r - l + 1]) % mod) % mod;
            if(ans < 0)
                ans += mod;
            
            ans_ull -= pref_ull[l - 1] * pw_ull[r - l + 1];
        }
        
        return {ans, ans_ull};
    }
};

// prints nodes along the path from start
void show_path_insertmovesrotate(node start, vector<vector<int>> path){
    // apply moves in {path} to node {start} sequentially and print all visited nodes
    print(cout, start);
    
    for(auto i: path){
        start = insertmoverotate(start, i[0], i[1], i[2]);
        cout << "MOVE " << i[0] << ' ' << i[1] << ' ' << i[2] << endl;
        print(cout, start);
    }
}

node insertmoverotate(node a, int idx, int tag, int rotation){
    node backup = a;
        
    if(tag == 2)
        a.second = inverse(a.second);
    else if(tag == 3)
        a.first = inverse(a.first);
    
    while(rotation--){ // perform rotations as needed
        if(tag % 2 == 0){
            a.second.push_front(a.second.back()); // rotate by one to the right
            a.second.pop_back();
        }
        else{
            a.first.push_front(a.first.back()); // rotate by one to the right
            a.first.pop_back();
        }
    }
    
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
    
    if(tag == 0 || tag == 2)
        res.second = backup.second;
    else
        res.first = backup.first;
    
    return res;
}

// different variant: assume that a is already rotated as needed, and the tag has been applied
// All we need to do is implement fast insertion evaluation and run it for every insertion index
// ans: {score=#of cancellations, {index of insertion, tag, how many rotations were performed on the donor}}

void add_insertmovesrotate(vector<pair<int, vector<int>>> &ans, node a, int tag, int rotation){
    int n = (int)(a.first.size()), m = (int)(a.second.size());
    
    int addition = n + m;
    
    // After cyclic shift, we might be able to cancel some terms in a.first or a.second
    normalise(a.first);
    normalise(a.second);
        
    n = int(a.first.size()), m = int(a.second.size());
    
    addition -= n + m;
    addition /= 2;
    
    // {addition} = (how many terms were cancelled in total) / 2

    for(auto &i: a.second)
        i = -i; // because to get a cancellation, we need an inverse of the term to match a term in a.first
    
    /*
     Hashing is used to quickly compare substrings. Build: O(N), Query: O(1)
     Building four prefix hashing:
     
     h_first : hash of a.first
     h_first_r : reversed hash of h.first, where every term is inversed
     h_second : hash of a.second
     h_second_r : reversed hash of h.second, where every term is inversed
     
     */
    
    prefixHash h_first, h_first_r, h_second, h_second_r;
    h_first.build(a.first);
    h_second.build(a.second);
    h_second_r.build(deque(a.second.rbegin(), a.second.rend()));
    
    for(auto &i: a.second)
        i = -i;
    for(auto &i: a.first)
        i = -i;
    
    h_first_r.build(deque(a.first.rbegin(), a.first.rend()));
    
    for(auto &i: a.first)
        i = -i;
    
    for(int i = -1; i < n; i++){
        ll l = 0, r = i + 1;
        
        // Binary Search 1: find the maximum length {l} where
        // a.first[i - l + 1...i] and a.second[0...l-1] have l cancelations
        
        while(l < r){
            ll mid = (l + r + 1) >> 1;
            
            /*
             compare a.first[i - mid + 1...i] and a.second[0...mid-1]
             */
            
            auto hash1 = h_first.get_hash(i - mid + 1, i);
            auto hash2 = h_second_r.get_hash(m - mid, m - 1);
                        
            if(hash1 == hash2)
                l = mid;
            else
                r = mid - 1;
        }
        
        ll cancelations = l;
        
        l = 0, r = n - i - 1;
        
        // BS 2: find the right hand side # of cancellations
        
        while(l < r){
            ll mid = (l + r + 1) >> 1;
            
            /*
             compare a.first[i + 1...i + mid] and a.second[m - mid...m - 1]
             */
            
            auto hash1 = h_first.get_hash(i + 1, i + mid);
            auto hash2 = h_second_r.get_hash(0, mid - 1);
            
            if(hash1 == hash2)
                l = mid;
            else
                r = mid - 1;
        }
        
        l = min(l, m - cancelations);
                
        cancelations += l;

        // if the whole of a.second was canceled out,
        // we can now see if the left and right parts of a.first have some terms cancelling out.
        if(cancelations == m){
            ll lhs = i + 1 - (cancelations - l), rhs = (n - i) - 1 - l;
                        
            if(lhs != 0 && rhs != 0){
                // can try to cancel further. // a.first[0..x][y..n-1]
                
                ll pos_i = i - (cancelations - l);
                ll pos_i1 = i + 1 + l;
                
                l = 0, r = min(lhs, rhs);
                while(l < r){
                    ll mid = (l + r + 1) >> 1;
                    
                    // compare a.first [pos_i1, pos_i1 + mid - 1]
                    auto hash1 = h_first.get_hash(pos_i - mid + 1, pos_i);
                    auto hash2 = h_first_r.get_hash(n - 1 - (pos_i1 + mid - 1), n - 1 - pos_i1);
                    
                    if(hash1 == hash2)
                        l = mid;
                    else
                        r = mid - 1;
                }
                
                cancelations += l;
            }
        }
        
        cancelations += addition;

        ans.push_back({2 * cancelations, {i, tag, rotation}});
    }
}

vector<pair<int, vector<int>>> rank_insertmovesrotate(node a){
    vector<pair<int, vector<int>>> ans;
    
    for(int order = 0; order < 2; order++){
        // order = 0  {a.first,  a.second}
        // order = 1 {a.second, a.first};
        
        for(int inverse = 0; inverse < 2; inverse++){
            // inverse = 0 {r1, r2}
            // inverse = 1 {r1, r2^{-1}}
            
            int tag = (order == 0 ? 0 : 1);
            
            if(inverse)
                tag += 2;
                        
            // rotation to the right
            for(int rotation = 0; rotation < (int)(a.second.size()); rotation++){
                add_insertmovesrotate(ans, a, tag, rotation);
                
                a.second.push_front(a.second.back()); // rotate by one to the right
                a.second.pop_back();
            }
            
            reverse(a.second.begin(), a.second.end());
            for(auto &r: a.second)
                r = -r;
        }
        
        swap(a.first, a.second);
    }
    
    sort(ans.rbegin(), ans.rend());
    
    return ans;
}

GreedyResult greedy_search_insertmovesrotate(node start, int max_nodes, int max_relator_length){
    priority_queue<node_info, vector<node_info>, greater<node_info>> q;
    
    // 'open set'; stores {{k=presentation length, l=length from the start}, node}
    q.push({{min((int)(start.first.size()), (int)(start.second.size())), 0}, start});
    
    // stores best pair (k, l) for each node
    map<node, pair<int, int>> mp;
    mp[start] = q.top().first;
    
    // stores the parent and the previous move for each node
    // move is now defined by three numbers: index, tag, # of rotation
    map<node, pair<node, vector<int>>> parent;
    
    // 'closed set'; a set of all expanded nodes (shouldn't be expanded again)
    set<node> used;
    int expanded = 0;
    bool trivial = false;
    
    node trivial_node;
    
    int mx = 0;
    
//    node finish = {{-2, -2, -1, -1, -1, -1, 2, 1}, {-1, -2, 1, 2, -1}};
//    node finish2 = {{-2, -2, -1, -1, -1, -1, 2, 1}, {1, -2, -1, 2, 1}};
    
    while(!q.empty()){
        auto v = q.top();
        q.pop();
        
//        print(cout, v.second);
        
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

int get_distance(node a, node b){
    a.first = get_smallest_rotation(a.first);
    a.second = get_smallest_rotation(a.second);
    
    b.first = get_smallest_rotation(b.first);
    b.second = get_smallest_rotation(b.second);
    
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

GreedyResult distance_greedy_search_insertmovesrotate(node start, int max_nodes, int max_relator_length){
    // change: storing in a set instead of priority queue
    set<node_info> q;
    
    // 'open set'; stores {{k=presentation length, l=length from the start}, node}
    
    // stores best pair (k, l) for each node
    map<node, pair<int, int>> mp;
    mp[start] = (*q.begin()).first;
    
    map<node, int> depth;
    depth[start] = 0;
    
    // stores the parent and the previous move for each node
    // move is now defined by three numbers: index, tag, # of rotation
//    map<node, pair<node, vector<int>>> parent;
    
    // 'closed set'; a set of all expanded nodes (shouldn't be expanded again)
    set<node> used;
    int expanded = 0;
    bool trivial = false;
    
    node trivial_node;
    
    int mx = 0;
    
    node finish = {{-2, -2, -1, -1, -1, -1, 2, 1}, {-1, -2, 1, 2, -1}};
    node finish2 = {{-2, -2, -1, -1, -1, -1, 2, 1}, {1, -2, -1, 2, 1}};
    
    int distance = min(get_distance(start, finish), get_distance(start, finish2));
    
    q.insert({{distance + min(start.first.size(), start.second.size()), 0}, start});
//    q.insert({{min(start.first.size(), start.second.size()), 0}, start});

    while(!q.empty()){
        auto v = *q.begin();
        q.erase(q.begin());
        
        print(cout, v.second);
                
//        auto *f = freopen("./output_cpp.txt", "a", stdout);
//        print(v.second);
//        fclose(f);
        
        mx = max(mx, (int)(v.second.first.size()) + (int)(v.second.second.size()));

//        if(v.second == finish || v.second == finish2){
//            trivial = true;
//            trivial_node = v.second;
//            
//            break;
//        }
        
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
            
            int distance = min(get_distance(to, finish), get_distance(to, finish2));
            
            pair<int, int> cost = {distance + min(to.first.size(), to.second.size()), v.first.second + 1};
//            pair<int, int> cost = {min(to.first.size(), to.second.size()), v.first.second + 1};
            
            // if {to} hasn't been expanded and {cost} is better than current best for {to},
            // then push to the open set
            
            if((int)(to.first.size()) + (int)(to.second.size()) == 2){
                trivial = true;
                trivial_node = to;
                
//                parent[to] = {v.second, all_moves[move].second};
                
                break;
            }
            
            if(to.first.size() == 1 || to.second.size() == 1){
                /*
                 We want to prune off nodes where one of the relators is the generator (or its inverse)
                 And the other relator comprises of other generator to some power.
                 EG
                 -2 -2 -1 2 2 2 -1 2| 2
                 is clearly not trivial, but the search still tries to apply many operations on r1
                 */
                if(to.second.size() == 1)
                    swap(to.first, to.second);
                
                int cnt = 0;
                
                for(auto i: to.second){
                    if(abs(i) == abs(to.first[0]))
                        continue;
                    
                    if(i < 0)
                        cnt--;
                    else
                        cnt++;
                }
                
                if(cnt != 1)
                    continue;
                else{
                     // add application of moves to remove relators physically.
                    trivial = true;
                    trivial_node = to;
                    
                    break;
                }
            }
            
            if(depth[v.second] + 1 < 100 && (int)(to.first.size()) < max_relator_length && (int)(to.second.size()) < max_relator_length && !used.count(to)){
                depth[to] = depth[v.second] + 1;
                neighbours_found += 1;
                
                used.insert(to);
                mp[to] = cost;
                  
//                parent[to] = {v.second, all_moves[move].second};
                q.insert({cost, to});
            }
        }
        
        if((ll)(used.size()) >= max_nodes || trivial)
            break;
    }
    
    vector<vector<int>> path;
        
//    if(trivial){
//        // trace the path back from the trivial node to the starting node
//        while(trivial_node != start){
//            path.push_back(parent[trivial_node].second);
//            trivial_node = parent[trivial_node].first;
//        }
//    }

    reverse(path.begin(), path.end());
    
    cout << "Finished Greedy Search. " << (trivial ? "Trivialisation found" : "No trivialisation found") << endl;
        
    return make_pair(trivial, path);
}
