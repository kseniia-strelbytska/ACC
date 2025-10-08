#include "header.h"

pair<int, int> compare(vector<pair<node, node>> presentations, vector<int> lens){
    // presentations {a, b}: pairs of nodes a and b, trying to get distance to correlate with len(path(a, b))
    int n = (int)(presentations.size());
    
    int total = n * (n - 1) / 2;
    int qscore_correct = 0, editdistance_correct = 0;

    for(int i = 0; i < n; i++){
        for(int j = i + 1; j < n; j++){
            long double qscorei = qgram_distance(presentations[i].first, presentations[i].second);
            long double qscorej = qgram_distance(presentations[j].first, presentations[j].second);
            
            int edit_distancei = get_distance(presentations[i].first, presentations[i].second);
            int edit_distancej = get_distance(presentations[j].first, presentations[j].second);

            int sign0 = (lens[i] < lens[j] ? -1 : (lens[i] == lens[j] ? 0 : 1));
            int sign1 = (qscorei < qscorej ? -1 : (qscorei == qscorej ? 0 : 1));
            int sign2 = (edit_distancei > edit_distancej ? -1 : (edit_distancei == edit_distancej ? 0 : 1));
            
            if(sign0 == sign1)
                qscore_correct++;
            if(sign0 == sign2)
                editdistance_correct++;
        }
    }
    
    return {qscore_correct, editdistance_correct};
}
