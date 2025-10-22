#include "header.h"

using namespace chrono;

void timed_evaluation(char **argv, GreedyFn greedy_search_variant, string presentations_path, string moves_path){
    // get current system time
    high_resolution_clock::time_point t1 = high_resolution_clock::now();
    
    string dir_path = argv[1];
    string input_file = argv[2];
    
    auto presentations = load_presentations_MS(dir_path + "/all_presentations.txt");
//    auto presentations = load_presentations(dir_path + "/cpp/datasets/dataset_len7.txt");
        
    evaluate(greedy_search_variant, presentations, dir_path + "/results" + presentations_path, dir_path + "/results" + moves_path, (ll)(1e6), 18);
    
//    // get current system time
    high_resolution_clock::time_point t2 = high_resolution_clock::now();

    // calculate the time the program took to execute
    duration<double> time_span = duration_cast<duration<double>>(t2 - t1);
    cout << time_span.count() << endl;
}

void get_average_len(string file_path, bool new_format){
    ifstream in(file_path.c_str());
    
    int n;
    vector<int> lens;
    
    while(in >> n){
        lens.push_back(n);
        
        string a;
        getline(in, a);
        
        n += (!new_format);
        
        while(n--){
            getline(in, a);
            
//            cout << a << endl;
        }
    }
    
    int sum = 0;
    for(auto i: lens)
        sum += i;
    
    sort(lens.begin(), lens.end());
    
    cout << "Average path length is " << fixed << setprecision(10) << (long double)(sum) / (long double)(lens.size()) << endl;
    
    cout << "Median path length is " << lens[lens.size() / 2] << endl;
}

int main(int argc, char **argv){
    string dir_path = argv[1];
    
    write_pairwise_metric_results(extract_paths(dir_path + "/results/smallest_rotation_solved_moves_explicit_paths.txt"), dir_path, "/pairwise_metrics.txt");
//
//    string file_path_new = dir_path + "/results/smallest_rotation_solved_moves.txt";
//    string file_path_old = dir_path + "/results/pre-smallest_rotation-reserve/533_explicit_solved_paths_insert_rotate.txt";
//
//    
//    get_average_len(file_path_new, true);
//    get_average_len(file_path_old, false);
//    
    return 0;
    

    node p1 = {{-2, -2, -2, -1, 2, 2, 1}, {-2, -1, 2, 2, 2, 1, 2, -1}};
    node p2 = {{1}, {-2}};
    
    auto result = guided_exploration(p1, p2, 1);
    
    cout << "RESULT " << result.first << endl;
    
    return 0;
    
    // 2 -1 2 -1 2 1 -2 1 -2 -2 -2 | -1 2 1 -2 -1 2 2
    
//    node source = {{2, -1, 2, -1, 2, 1, -2, 1, -2, -2, -2}, {-1, 2, 1, -2, -1, 2, 2}};
//    
//    auto s = get_distance(source, {{1}, {2}});
//    
//    cout << s << endl;
//    
//    return 0;
    
//
//    timed_evaluation(argv, greedy_search_insertmovesrotate, "/large_MS_presentations.txt", "/large_MS_moves.txt");

//    timed_evaluation(argv, greedy_search_insertmovesrotate, "/smallest_rotation_solved_presentations.txt", "/smallest_rotation_solved_moves.txt");
//    global_writing(load_presentations_MS(dir_path + "/all_presentations.txt"), dir_path + "/results/smallest_rotation_solved_moves.txt");
//    write_multi_paths(dir_path + "/all_presentations.txt", dir_path);
//    write_pairwise_metric_results(extract_paths(dir_path + "/results/smallest_rotation_solved_moves_explicit_paths.txt"), dir_path, "/pairwise_metrics.txt");
    
    return 0;
    
//    global_writing(load_presentations_MS(dir_path + "/all_presentations.txt"), dir_path + "/results/smallest_rotation_solved_moves.txt");
    
//    write_multi_paths(dir_path + "/all_presentations.txt", dir_path);
    
//    analysis(extract_paths(dir_path + "/results/smallest_rotation_solved_moves_explicit_paths.txt"), dir_path);

    return 0;
//    node p4 = {{-2, -2, 1, 2, 2, 2, -1}, {2, 2, 2, 1, 1, 2, 2, 2, 2, 1}};
//    node w = {{2, 1, 1, 2, 2, 2, 1, 1}, {-2, -2, 1, 2, 2, 2, -1}};
//    
//    cout << get_lcs(w, p4) << endl;
//    
//    return 0;
    
    /*
     -1 -1 -2 -2 1 1 2 -1 | 2 -1 2 -1 -2 1 2 2 -1 -2 -2 1 1
     -1 -1 -2 -2 1 1 2 -1 | -1 2 -1 -2 1 2 2 -1 -2 -2 1 1 2
     
     */

    
    compare_by_order(extract_paths(dir_path + "/results/533_explicit_solved_paths_insert_rotate.txt"));

    return 0;

    
    return 0;

    write_multi_paths(dir_path + "/all_presentations.txt", dir_path);
    
    return 0;

    return 0;
    
//    node p1 = {{-2, -2, 1, 2, -1, -2, 1}, {-2, -2, -2, -2, 1, -2, -2, -2, 1, 1}};
//    node p2 = {{-2, -2, 1, 2, -1, -2, 1}, {-2, -2, -2, 1, -2, -1, -2, 1, 1}};
//    node p3 = {{-2, -2, 1, 2, -1, -2, 1}, {-2, -2, -1, -2, 1, 1}};
//    node p4 = {{-2, -2, 1, 2, 2, 2, -1}, {-2, -2, -2, 1, 2, -1, -2, 1}};
    
//    node a = {{-2, -2, -1, -1, -1, -1, 2, 1}, {-1, -2, 1, 2, -1}};
//   node b = {{-2, -2, -2, -1, -1, -1, -1, 2, 1}, {-1, -2, 1, 2, -1}};
//   node c = {{-2, -2, -2, -2, -1, -1, -1, -1, 2, 1}, {-1, -2, 1, 2, -1}};
    
//    auto result = solver(b, a, (int)(1e8), 18);
    
//    auto result = greedy_search_insertmovesrotate(b, int(1e12), 18);
//    
//    return 0;
//    
//    string dir_path = argv[1];
//    
//    auto presentations = load_presentations_MS(dir_path + "/all_presentations.txt");
    
//    analyse(presentations, "/Users/kseniia/Desktop/programming/Projects/ACC/results/solved_paths_rotate_MS");
    
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
//    
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
