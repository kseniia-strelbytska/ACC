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

using namespace std;
typedef long long ll;
typedef long double ld;
typedef pair<deque<int>, deque<int>> node;
typedef pair<pair<int, int>, node> node_info;

// support.cpp
void print(node a);
void print(deque<int> a);
deque<int> concat(deque<int> a, deque<int> b);
void normalise(deque<int> &a);
node ACMove(node a, int move);
void show_path(node start, vector<int> path);

// generate_datasets.cpp
void generate_p(ll pos, ll val, ll max_len);
void initialise_presentations(ll max_len);
void generate_all();

// greedy.cpp
pair<bool, vector<int>> greedy_search(node start, int max_nodes, int max_relator_length);

// load_datasets.cpp
deque<int> convert(string line);
vector<node> load_presentations(string file_path);
vector<node> load_presentations_MS(string file_path);
string get_file_path(int max_len);

// evaluate.cpp
void evaluate(vector<node> presentations, string output_pr, string output_pa, int max_nodes, int max_relator_length);
