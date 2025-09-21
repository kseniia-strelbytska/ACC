#include "header.h"

using namespace chrono;

int main(int argc, char **argv){
    // get current system time
    high_resolution_clock::time_point t1 = high_resolution_clock::now();
    
    string dir_path = argv[1];
    string input_file = argv[2];
    
    auto presentations = load_presentations_MS(dir_path + "/all_presentations.txt");
    
    evaluate(presentations, dir_path + "/results/cpp_solved_presentations" + input_file, dir_path + "/results/cpp_solved_paths" + input_file, (ll)(1e6), 18);
    
    // get current system time
    high_resolution_clock::time_point t2 = high_resolution_clock::now();

    // calculate the time the program took to execute
    duration<double> time_span = duration_cast<duration<double>>(t2 - t1);
    cout << time_span.count() << endl;
    
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
