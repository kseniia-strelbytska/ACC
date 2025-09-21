#include "header.h"

// converts a string of numbers (separated by spaces) into a presentation (deque<int>)
deque<int> convert(string line){
    deque<int> a;
    
    int idx = 0;
    while(idx < (ll)(line.size())){
        if(line[idx] == ' '){
            idx++;
            continue;
        }
        
        int number = 1;
        
        if(line[idx] == '-'){
            number = -1;
            idx++;
        }
        
        number *= (line[idx] - '0');
        idx++;
        
        a.push_back(number);
    }
    
    return a;
}

// reads presentations in format "relator1 \n relator2" (relators are separated by spaces)
vector<node> load_presentations(string file_path){
    ifstream stream(file_path.c_str());
    
    vector<node> presentations;
        
    if(stream.is_open()){
        string line1, line2;
        
        while(getline(stream, line1)){
            getline(stream, line2);
            
            node presentation;
            
            presentation.first = convert(line1);
            presentation.second = convert(line2);
            
            presentations.push_back(presentation);
        }
        
        stream.close();
    }
    else{
        cout << "Error when opening " << file_path << endl;
    }
    
    return presentations;
}

// reads presentations of Miller-Schupp series in format "[relator1, 0s padding, relator2, 0s padding]" (relators are separated by commas)
vector<node> load_presentations_MS(string file_path){
    ifstream stream(file_path.c_str());
    
    vector<node> presentations;
        
    if(stream.is_open()){
        string line;
        
        while(getline(stream, line)){
            node presentation;
            
            bool f = false;
            
            for(int i0 = 0; i0 < (ll)(line.size()); i0++){
                char i = line[i0];
                
                if(i == '0')
                    f = true;
                
                if(i == '[' or i == ',' or i == ' ' or i == ']' or i == '0')
                    continue;
                
                ll val = 1;
                if(i == '-'){
                    val = -1;
                    i = line[++i0];
                }
                
                val *= (i - '0');
                
                if(!f){
                    presentation.first.push_back(val);
                }
                else{
                    presentation.second.push_back(val);
                }
            }
            
            presentations.push_back(presentation);
        }
        
        stream.close();
    }
    else{
        cout << "Error when opening " << file_path << endl;
    }
    
    return presentations;
}

//int main(){
//    for(int max_len = 2; max_len <= 2; max_len++){
//        string file_path = get_file_path(max_len);
//        
//        cout << file_path << endl;
//        
//        auto res = load_presentations(file_path);
//        
//        for(auto i: res)
//            print(i);
//    }
//     
//    return 0;
//}
//
