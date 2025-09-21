#include "header.h"

using namespace chrono;

int main(){
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
    
    high_resolution_clock::time_point t1 = high_resolution_clock::now();
    
    auto presentations = load_presentations_MS("/Users/kseniia/Desktop/programming/Projects/acc/all_presentations.txt");
    evaluate(presentations, "cpp_solved_MS_presentations.txt", "cpp_solved_MS_paths.txt", (ll)(1e6), 18);
    
    high_resolution_clock::time_point t2 = high_resolution_clock::now();

    duration<double> time_span = duration_cast<duration<double>>(t2 - t1);
    
    cout << time_span.count() << endl;
    
    return 0;
}
