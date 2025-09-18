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
    
    
    high_resolution_clock::time_point t1 = high_resolution_clock::now();
    
    auto presentations = load_presentations_MS("/Users/kseniia/Desktop/programming/Projects/acc/all_presentations.txt");
    evaluate(presentations, "cpp_solved_MS_presentations.txt", "cpp_solved_MS_paths.txt", (ll)(1e5), 20);
    
    high_resolution_clock::time_point t2 = high_resolution_clock::now();

    duration<double> time_span = duration_cast<duration<double>>(t2 - t1);
    
    cout << time_span.count() << endl;
    
    return 0;
}
