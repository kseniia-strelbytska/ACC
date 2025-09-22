#include "header.h"

// prints a node into a given stream

void print(ostream &stream, node a){
    for(auto i: a.first)
        stream << i << ' ';
    stream << "| ";
    
    for(auto i: a.second)
        stream << i << ' ';
    stream << endl;
}

// prints a relator into a given stream
void print(ostream &stream, deque<int> a){
    for(auto i: a)
        stream << i << ' ';
    stream << endl;
}


// returns an inverse of the relator {a}
deque<int> inverse(deque<int> a){
    for(auto &i: a){
        i = -i;
    }
    
    reverse(a.begin(), a.end());
    return a;
}

// returns product of {a} and {b}, simplified
deque<int> concat(deque<int> a, deque<int> b){
    // if two relators -- at the end of {a} and at the start of {b} --
    // simplify to identity, then cancel them
    while(!a.empty() && !b.empty() && abs(a.back()) == abs(b[0]) && a.back() != b[0]){
        a.pop_back();
        b.pop_front();
    }
    
    // concatenate b to a
    for(auto i: b)
        a.push_back(i);
    
    return a;
}

// returns simplified {a} (with identity terms canceled out)
void normalise(deque<int> &a){
    deque<int> result;
    
    int sz;
    
    for(auto i: a){
        result.push_back(i);
        sz = (int)(result.size());

        if(sz >= 2 && abs(result[sz - 1]) == abs(result[sz - 2]) && result[sz - 1] != result[sz - 2]){
            result.pop_back();
            result.pop_back();
        }
    }
    
    a = result;
}

/*
0. r_1 --> r_1 r_0
1. r_0 --> r_0 r_1^{-1} -> r_0 -> r_0^{-1}
2. r_1 --> r_1 r_0^{-1} -> r_1 -> r_1^{-1}
3. r_0 --> r_0 r_1
4: r_1 --> x_0^{-1} r_1 x_0
5: r_0 ---> x_1^{-1} r_0 x_1
6: r_1 --> x_1^{-1} r_1 x_1
7: r_0 ---> x_0 r_0 x_0^{-1}
8: r_1 --> x_0 r_1 x_0^{-1}
9: r_0 --> x_1 r_0 x_1^{-1}
10: r_1 --> x_1 r_1 x_1^{-1}
11: r_0 --> x_0^{-1} r_0 x_0
 
 CHANGE:
 1. r_0 --> r_0 r_1^{-1}
 2. r_1 --> r_1 r_0^{-1}
 
 */

// returns a node when move {move} is performed on {a}
node ACMove(node a, int move){
    if(move == 0){
        a.second = concat(a.second, a.first);
    }
    else if(move == 1){
        deque<int> b = a.second;
        reverse(b.begin(), b.end());
        for(auto &i: b)
            i = -i;
        
        a.first = concat(a.first, b);
    }
    else if(move == 2){
        deque<int> b = a.first;
        reverse(b.begin(), b.end());
        for(auto &i: b)
            i = -i;
        
        a.second = concat(a.second, b);
    }
    else if(move == 3){
        a.first = concat(a.first, a.second);
    }
    else if(move == 4){
        a.second.push_front(-1);
        a.second.push_back(1);
        
        normalise(a.second);
    }
    else if(move == 5){
        a.first.push_front(-2);
        a.first.push_back(2);
        
        normalise(a.first);
    }
    else if(move == 6){
        a.second.push_front(-2);
        a.second.push_back(2);
        
        normalise(a.second);
    }
    else if(move == 7){
        a.first.push_front(1);
        a.first.push_back(-1);
        
        normalise(a.first);
    }
    else if(move == 8){
        a.second.push_front(1);
        a.second.push_back(-1);
        
        normalise(a.second);
    }
    else if(move == 9){
        a.first.push_front(2);
        a.first.push_back(-2);
        
        normalise(a.first);
    }
    else if(move == 10){
        a.second.push_front(2);
        a.second.push_back(-2);
        
        normalise(a.second);
    }
    else if(move == 11){
        a.first.push_front(-1);
        a.first.push_back(1);
        
        normalise(a.first);
    }
    
    return a;
}

// prints nodes along the path from start
void show_path(node start, vector<int> path){
    // apply moves in {path} to node {start} sequentially and print all visited nodes
    print(cout, start);
    
    for(auto i: path){
        start = ACMove(start, i);
        print(cout, start);
    }
}
