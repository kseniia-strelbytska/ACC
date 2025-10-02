#include <iostream>
#include <vector>
#include <string>
#include <math.h>
#include <cmath>
#include <iomanip>
#include <cstdio>
#include <algorithm>
#include <numeric>
#include <map>
#include <set>
#include <queue>
#include <stack>
#include <deque>
#include <bitset>
#include <cstring>
#include <unordered_map>
#include <filesystem>
#include <fstream>
#include<cstdlib>
#include<ctime>

using namespace std;
typedef long long ll;
typedef unsigned long long ull;
typedef long double ld;

/*
 Representation format:
 
 Each relator is represented as a deque of -2/-1/1/2
 Each node is represented as a pair of two deques
 
 Example:
 deque<int>d = {-1, -2, 1};
 node a = {{-1, -2, 1}, {2, -1}};
 */

typedef pair<deque<int>, deque<int>> node;
typedef pair<pair<int, int>, node> node_info;

using GreedyResult = std::variant<
    std::pair<bool, std::vector<int>>,
    std::pair<bool, std::vector<std::vector<int>>>>;

using GreedyFn = GreedyResult(*)(node, int, int);

// ----------support.cpp----------

// prints a node into a given stream
void print(ostream &stream, node a);

// prints a relator  into a given stream
void print(ostream &stream, deque<int> a);

// returns an inverse of the relator {a}
deque<int> inverse(deque<int> a);

// returns product of {a} and {b}, simplified
deque<int> concat(deque<int> a, deque<int> b);

// returns simplified {a} (with identity terms canceled out)
void normalise(deque<int> &a);

// returns a node when move {move} is performed on {a}
node ACMove(node a, int move);

// prints nodes along the path from start
void show_path(node start, vector<int> path);

// ----------generate_datasets.cpp----------

// a recursive function to generate presentations
void generate_p(ll pos, ll val, ll max_len);

// generates all presentations of length {max_len} (in simplified form)
// stores result in a global set {presentations}
void initialise_presentations(ll max_len);

// generates and saves to separate files all presentations of lengths 2 to 9
void generate_all(string dir_path);

// ----------greedy.cpp----------
// returns a pair {True/False whether trivialisation was found, path if True else {}}
GreedyResult greedy_search(node start, int max_nodes, int max_relator_length);

// ----------load_datasets.cpp----------

// converts a string of numbers (separated by spaces) into a presentation (deque<int>)
deque<int> convert(string line);

// reads generated presentations
vector<node> load_presentations(string file_path);

// reads presentations of Miller-Schupp series
vector<node> load_presentations_MS(string file_path);

// ----------evaluate.cpp----------

// runs greedy search on every presentation from {presentations}
// saves trivialised presentations and their paths
// prints the number of solved out of total
// takes a pointer to a greedy function as first parameter
void evaluate(GreedyFn greedy_search_variant, vector<node> presentations, string output_presentation, string output_path, int max_nodes, int max_relator_length);

// ----------insert-moves-vairant.cpp----------

// returns the resulting relator when performing
// an insert move on relator {a}, with parameters idx and tag
node insertmove(node a, int idx, int tag);

// updates the vector {ans} with insert moves of type {tag}
void add_insertmoves(vector<pair<int, pair<int, int>>> &ans, int tag, node a);

// Functions returns a list of pairs {# of cancelations, {idx, tag}}
vector<pair<int, pair<int, int>>> rank_insertmoves(node a);

// ground truth (relies on concat() function).
// Functions returns a list of pairs {# of cancelations, {idx, tag}}
vector<pair<int, pair<int, int>>> rank_insertmoves_truth(node a);

GreedyResult greedy_search_insertmoves(node start, int max_nodes, int max_relator_length);

// ----------insertmoverotate.cpp----------

void show_path_insertmovesrotate(node start, vector<vector<int>> path);

node insertmoverotate(node a, int idx, int tag, int rotation);

void add_insertmovesrotate(vector<pair<int, vector<int>>> &ans, node a, int tag, int rotation);

vector<pair<int, vector<int>>>  rank_insertmovesrotate(node a);

GreedyResult greedy_search_insertmovesrotate(node start, int max_nodes, int max_relator_length);

int get_distance(node a, node b);
GreedyResult distance_greedy_search_insertmovesrotate(node start, int max_nodes, int max_relator_length);

// ----------analyse.cpp----------

deque<int> get_smallest_rotation(deque<int> a);

void analyse(vector<node> presentations, string sol_path);
