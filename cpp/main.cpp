#include "header.h"

using namespace chrono;

void timed_evaluation(char **argv, GreedyFn greedy_search_variant){
    // get current system time
    high_resolution_clock::time_point t1 = high_resolution_clock::now();
    
    string dir_path = argv[1];
    string input_file = argv[2];
    
    auto presentations = load_presentations_MS(dir_path + "/all_presentations.txt");
//    auto presentations = load_presentations(dir_path + "/cpp/datasets/dataset_len7.txt");
    
    string name = "rotate_MS";
    
    evaluate(greedy_search_variant, presentations, dir_path + "/results/solved_presentations_" + name, dir_path + "/results/solved_paths_" + name, (ll)(1e6), 18);
    
//    // get current system time
    high_resolution_clock::time_point t2 = high_resolution_clock::now();

    // calculate the time the program took to execute
    duration<double> time_span = duration_cast<duration<double>>(t2 - t1);
    cout << time_span.count() << endl;
}

int main(int argc, char **argv){    
    string dir_path = argv[1];
    
    auto presentations = load_presentations_MS(dir_path + "/all_presentations.txt");
    
    analyse(presentations, "/Users/kseniia/Desktop/programming/Projects/ACC/results/solved_paths_rotate_MS");
    
//    auto presentations = load_presentations(dir_path + "/cpp/datasets/dataset_len9.txt");
//    
//    for(auto i: presentations){
//        auto res = rank_insertmoves(i);
//        auto res_truth = rank_insertmoves_truth(i);
//        
//        if(res != res_truth){
//            print(cout, i);
//            
//            return 0;
//        }
//    }

//    timed_evaluation(argv, greedy_search);
    
//    timed_evaluation(argv, greedy_search_insertmovesrotate);
//    timed_evaluation(argv, greedy_search_insertmovesrotate);
    
//    node a = {{-2, -2, -1, -1, -1, -1, 2, 1}, {-1, -2, 1, 2, -1}};
//    node b = {{-2, -2, -2, -1, -1, -1, -1, 2, 1}, {-1, -2, 1, 2, -1}};
//    node c = {{-2, -2, -2, -2, -1, -1, -1, -1, 2, 1}, {-1, -2, 1, 2, -1}};
//    
////    node d = {{-1, -2, -2, -2, 1, -2, -2, -2, -2}, {1, 1, -2, -1, 2}};
//    
//    auto res = distance_greedy_search_insertmovesrotate(b, int(1e7), 18);
//
//    auto result = get<pair<bool, vector<vector<int>>>> (res);
//    
//    cout << result.first << ' ' << result.second.size() << endl;
//    
//    show_path_insertmovesrotate(b, result.second);
    
    return 0;
}

//    auto res = load_presentations_MS("/Users/kseniia/Desktop/programming/Projects/acc/all_presentations.txt");
//
//    ll cnt = 10;
//    for(auto i: res){
//        print(i);
//        cnt--;
//
//        if(!cnt)
//            break;
//    }
    // [, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, , 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
    
//    auto *f = freopen("./output_cpp.txt", "w", stdout);
//    cout << "";
//    fclose(f);
    
//    node start = {{-1, 2, 2, 1, -2, -2, -2}, {-1, 2, 1, 2, -1}};
//
//    auto res = greedy_search(start, (ll)(1e5), 20);
//
//    cout << res.first << endl;
