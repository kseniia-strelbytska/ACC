import matplotlib.pyplot as plt
import numpy as np
from scipy.stats import spearmanr, pearsonr

def extract_paths():
    file_path = '/Users/kseniia/Desktop/programming/Projects/ACC/results/solved_paths_rotate_MS_explicit_paths_smallestrotation.txt'

    paths = []

    with open(file_path, 'r') as f:
        current = []

        for line in f:
            line = line.strip('\n')

            if len(line) <= 3:
                if current:
                    paths.append(current)
                
                current = []
            else:
                p = line.split(' ')
                r1, r2 = [], []

                for gen in p:
                    if gen == '':
                        continue
                    
                    if gen == '|':
                        r1, r2 = r2, r1
                    else:
                        r2.append(int(gen))
                
                current.append([r1, r2])
        
        paths.append(current)

    return paths

def extract_moves():
    file_path = '/Users/kseniia/Desktop/programming/Projects/ACC/results/solved_paths_rotate_MS'

    moves = []

    with open(file_path, 'r') as f:
        current = []

        for line in f:
            line = line.strip('\n')

            if len(line) <= 3:
                if current:
                    moves.append(current)
                
                current = []
            else:
                p = []
                for number in line.split(' '):
                    if number != '':
                        p.append(int(number))

                current.append(p)
        
        moves.append(current)

    return moves

def move_sequence(paths, moves):
    flow = [[0] * 4 for i in range(4)]

    for path in moves:
        for move in range(len(path) - 1):
            flow[path[move][1]][path[move + 1][1]] += 1

    mx = max([max(row) for row in flow])
    s = sum([sum(row) for row in flow])

    for row in flow:
        row = [round(i / s, 2) for i in row]
        print(row)

    parity = [[0] * 2 for i in range(2)]

    for m1 in range(4):
        for m2 in range(4):
            parity[m1 % 2][m2 % 2] += flow[m1][m2]

    for row in parity:
        print(row)
        
def alternation_ind(presentation):
    ans = 0

    for relator in presentation:
        for idx in range(len(relator)):
            if abs(relator[idx]) != abs(relator[(idx + 1) % len(relator)]):
                ans += 1

    return ans / (len(presentation[0]) + len(presentation[1]))

def get_subsequences(relator, l):
    subsequences = []
    current = []

    for i in range(len(relator)):
        current.append(relator[i])

        if len(current) > l:
            current = current[1:]

        if(len(current) == l):
            subsequences.append(current)
    
    return subsequences
    
def get_frequency(case):
    seq = {}

    for l in range(5, 10):
        for path in case:
            for node in path:
                subsequences = get_subsequences(node[0], l) + get_subsequences(node[1], l)

                for sequence in subsequences:
                    sequence = tuple(sequence)
                    
                    if sequence not in seq:
                        seq[sequence] = 0

                    seq[sequence] += 1

    all = []
    for sequence in seq.keys():
        all.append([seq[sequence], sequence])

    all.sort(reverse = True)
    
    for i in range(10):
        print(all[i])

    return seq, all

def easy_hard_analysis():
    paths = extract_paths()
    moves = extract_moves()

    # for each presentation length, stores all paths from nodes of that length
    presentation_len = {}

    for path in paths:
        p_len = len(path[0][0]) + len(path[0][1])

        if p_len not in presentation_len:
            presentation_len[p_len] = []
        
        presentation_len[p_len].append(path)

    divide = {}
    # for each MS presentation length stores easy and hard instances (< median path len > median path len)

    for p_len in presentation_len.keys():
        path_lens = []

        for path in presentation_len[p_len]:
            path_lens.append(len(path) - 1)

        path_lens.sort()
        median = path_lens[len(path_lens) // 2]

        divide[p_len] = [[], []]

        for path in presentation_len[p_len]:
            if len(path) - 1 < median:
                divide[p_len][0].append(path) # easy instance
            else:
                divide[p_len][1].append(path) # hard instance


    tot_easy, tot_hard = 0, 0
    easy_altind, hard_altind = 0, 0

    all_easy, all_hard = [], []

    for i in range(7, 18):
        tot_easy += len(divide[i][0])
        tot_hard += len(divide[i][1])

        for easy in divide[i][0]:
            all_easy.append(easy)

            presentation = easy[0]
            easy_altind += alternation_ind(presentation)

        for hard in divide[i][1]:
            all_hard.append(hard)

            presentation = hard[0]
            hard_altind += alternation_ind(presentation)

    easy_altind /= tot_easy
    hard_altind /= tot_hard

    easy_freq = get_frequency(all_easy)[0]
    hard_freq = get_frequency(all_hard)[0]

    combined = {}
    for subseq in easy_freq.keys():
        if subseq not in combined:
            combined[subseq] = 0
        combined[subseq] += easy_freq[subseq]

    for subseq in hard_freq.keys():
        if subseq not in combined:
            combined[subseq] = 0
        combined[subseq] += hard_freq[subseq]

    all = []

    mx = max(combined.values())

    for sequence in combined.keys():
        mx = max(mx, combined[sequence])
        all.append([combined[sequence], sequence])

    all.sort(reverse = True)

    print(all[0:10])

def process_all_pairs(filepath):
    # each pair is stored in format distance, qgram, edit, path, ind1, ind2
    # path, ind1, ind2 can be used to locate the pair in the file
    all_pairs = []
    
    with open(filepath, 'r') as f:
        for line in f:
            data = [float(i) for i in line.strip().split(' ')]
            
            for idx in range(len(data)):
                if idx != 1: # not qgram
                    data[idx] = int(data[idx])
                   
            all_pairs.append(data)
            
    distances, qgram, edit, lcs = [], [], [], []
    
    for pair in all_pairs:
        distances.append(pair[0])
        qgram.append(pair[1])
        edit.append(pair[2])
        lcs.append(pair[3])
        
#    plt.scatter(distances, edit, s=0.1)
# plt.savefig('/Users/kseniia/Desktop/programming/Projects/ACC/results/edit_vs_distance_scatter.png', dpi=1000)

    d_q = {}
    
    for l in range(300):
        d_q[l] = []
    
    for pair in all_pairs:
        d_q[pair[0]].append((pair[1], pair))
        
#    for l in range(1, 2):
#        d_q[l].sort()
#
#        for example in d_q[l][0:20]:
#            print(example)
#        for example in d_q[l][-21:-1]:
#            print(example)
#
#        print('-' * 30)
    
    print('S and P correlations very from -1 to 1. Higher absolute value means higher correlation')
    correlation = spearmanr(distances, qgram)
    print(f'S Correlation between ground truth distance and qgram is {correlation[0]}')
    correlation = spearmanr(distances, edit)
    print(f'S Correlation between ground truth distance and edit is {correlation[0]}')
    correlation = spearmanr(distances, lcs)
    print(f'S Correlation between ground truth distance and lcs is {correlation[0]}')
    
    correlation = pearsonr(distances, qgram)
    print(f'P Correlation between ground truth distance and qgram is {correlation[0]}')
    correlation = pearsonr(distances, edit)
    print(f'P Correlation between ground truth distance and edit is {correlation[0]}')
    correlation = pearsonr(distances, lcs)
    print(f'P Correlation between ground truth distance and lcs is {correlation[0]}')
        
    return all_pairs

# metric_idx = 1 for qgram, 2 for edit, 3 for LCS
def roc_curve(all_pairs, ndistance, metric_idx, metric_name):
    qgram_scores = []
    idx = -1
    
    for pair in all_pairs:
        idx += 1
        
        qgram_scores.append((pair[metric_idx], idx))
    
    qgram_scores.sort(reverse = (True if metric_idx == 1 else False))
    
    with open(f'/Users/kseniia/Desktop/programming/Projects/ACC/results/all_pairs_{metric_name}sorted.txt', 'w') as f:
        for pair in qgram_scores:
            s = ''
            for i in all_pairs[pair[1]]:
                s += str(i) + ' '
            f.write(f'{s:<40}1' if all_pairs[pair[1]][0] <= ndistance else f'{s:<40}0')
            f.write('\n')
    
    total_neighbouring = sum([(1 if pair[0] <= ndistance else 0) for pair in all_pairs])
    
    neighbouring_retrieved = 0
    
    precision, recall = [], []
    
    for idx in range(len(qgram_scores)):
        if all_pairs[qgram_scores[idx][1]][0] <= ndistance:
            neighbouring_retrieved += 1
        
        precision.append(neighbouring_retrieved / (idx + 1))
        recall.append(neighbouring_retrieved / total_neighbouring)
    
    fig, ax = plt.subplots()
    ax.set(ylim=(0, 1.1))
    ax.plot(recall, precision)
    fig.savefig(f'/Users/kseniia/Desktop/programming/Projects/ACC/results/roc_curve_' + metric_name + f'_nd={ndistance}.png')
    
    fig.clf()
    
def edit_lcs_correlation(all_pairs):
    data = [[], []]
    
    for pair in all_pairs:
        data[0].append(pair[2]) # edit
        data[1].append(pair[3]) # lcs
    
    print('S and P correlations vary from -1 to 1. Higher absolute value means higher correlation')
    correlation = spearmanr(data[0], data[1])
    print(f'S Correlation between edit distance and lcs is {correlation[0]}')
    correlation = pearsonr(data[0], data[1])
    print(f'P Correlation between edit distance and lcs is {correlation[0]}')
    
    data.sort()
    
    plt.scatter(data[0], data[1]) 
    plt.xlabel('edit distance')
    plt.ylabel('lcs distance')
    
    fig = plt.gcf()
    fig.savefig(f'/Users/kseniia/Desktop/programming/Projects/ACC/results/plot_lcs_vs_edit.png')
    plt.close(fig)
    
def show_averages(all_pairs, dmax):
    distances = {}
    
    for pair in all_pairs:
        if pair[0] not in distances:
            distances[pair[0]] = []
        
        distances[pair[0]].append(pair)
        
    fig, axs = plt.subplots()
    
    y_edit, y_qgram, y_lcs = [], [], []
    
    for d in range(1, dmax):
        edit, qgram, lcs = 0, 0, 0
        
        for pair in distances[d]:
            qgram += pair[1]
            edit += pair[2]
            lcs += pair[3]
        
        l = len(distances[d])
        
        print(f'Averages at distance {d}: edit = {round(edit / l, 3)}, qgram = {round(qgram / l, 3)}, lcs = {round(lcs / l, 3)}')
        
        y_edit.append(edit / l)
        y_qgram.append(10 * qgram / l)
        y_lcs.append(lcs / l)
    
    x = [i for i in range(1, dmax)]
    
    axs.plot(x, y_qgram, label = 'Qgram distance')
    axs.plot(x, y_edit, label = 'Edit distance')
    axs.plot(x, y_lcs, label = 'LCS distance')
    axs.legend()
    axs.set_xlabel('Distance')
    
    fig.savefig(f'/Users/kseniia/Desktop/programming/Projects/ACC/results/avg_metric_change_vs_distance.png')

if __name__ == "__main__":
    all_pairs = process_all_pairs('/Users/kseniia/Desktop/programming/Projects/ACC/results/pairwise_metrics.txt')
    
#    roc_curve(all_pairs, 4, 1, 'qgram')
#    
#    exit(0)
    
    show_averages(all_pairs, 20)
    
    names = ['qgram', 'edit', 'lcs']
    
    for idx in range(len(names)):
        roc_curve(all_pairs, 4, idx + 1, names[idx])
        
    edit_lcs_correlation(all_pairs)
