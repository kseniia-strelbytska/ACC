
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

paths = extract_paths()
moves = extract_moves()

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

    return seq, all 

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