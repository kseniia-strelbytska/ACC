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
        
        pw[0] = pw2[0] = 1;
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

// functions returns a list of pairs {# of cancelations, index of insertion in 1-indexation (negative if inserting an inverse of a realtor}}

vector<pair<int, int>> rank_insertmoves(node a){
    // inserting a.second into a.first;
    
    int n = (int)(a.first.size());
    
    for(int i = 0; i < n; i++){
        
    }
}
