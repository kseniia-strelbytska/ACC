from greedy import greedy_search
import time

def get_name(file_path):
    s = []
    
    for i in range(len(file_path) - 1, -1, -1):
        if file_path[i] == '/':
            break
        
        s.append(file_path[i])
        
    s.reverse()
    s = ''.join(s)
    
    return s

def load_presentations(file_path):
    presentations = []
    
    with open(file_path, 'r') as f:
        vec = []
        
        prev = ''
        
        for l in f:
            if prev != '':
                prev = [int(i) for i in prev.strip().split()]
                l = [int(i) for i in l.strip().split()]
                
#                mx = max(len(prev), len(l))
                while len(prev) < max_relator_length:
                    prev.append(0)
                    
                while len(l) < max_relator_length:
                    l.append(0)
                    
                presentations.append(prev + l)
                prev = ''
            else:
                prev = l
        f.close()
        
    return presentations

def load_presentations_MS(file_path):
    presentations = []
    
    with open(file_path, 'r') as f:
        vec = []
        
        for l in f:
            p = list([int(i.strip()) for i in l.strip()[1:-1].split(',')])
            presentations.append(p)
            
        f.close()
                
    return presentations

def evaluate(presentations, output_pr, output_pa, max_nodes, max_relator_length):
    output_presentations = "/Users/kseniia/Desktop/programming/Projects/acc/results/" + output_pr;
    output_paths = "/Users/kseniia/Desktop/programming/Projects/acc/results/" + output_pa;
    
    with open(output_presentations, 'w') as f:
        f.write('')
    with open(output_paths, 'w') as f:
        f.write('')
    
    solved, total = 0, 0
    for p in presentations:
        ans, path = greedy_search(p, max_nodes)
                
        total += 1
        
        if ans == True:
            solved += 1
        
            with open(output_presentations, 'a') as f:
                f.write(''.join([str(p[i]) + ' ' if p[i] != 0 else '' + ('| ' if p[i] == 0 and i + 1 < len(p) and p[i + 1] != 0 else '')for i in range(len(p))]))
                f.write('\n')
                
            with open(output_paths, 'a') as f:
                f.write(' '.join([str(i) for i in path]))
                f.write('\n')
                
        if total % 1 == 0:
            print(f'Solved: {solved}/{total}')
                
    print(solved, len(presentations), solved / len(presentations))


def get_file_path(max_len):
    return "/Users/kseniia/Desktop/programming/Projects/acc/cpp/datasets/dataset_len" + str(max_len) + ".txt";
    
#    print(greedy_search([-2, 0, 0, 0, 0, 0, -1, -1, 0, 0, 0, 0], int(1e5)))
#    print(greedy_search([-2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0], int(1e5)))
