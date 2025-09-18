from evaluate import evaluate, load_presentations, load_presentations_MS
from greedy import greedy_search
import time

if __name__ == '__main__':
    presentations = load_presentations_MS("/Users/kseniia/Desktop/programming/Projects/acc/all_presentations.txt")
    
    t1 = time.perf_counter()
    
    evaluate(presentations, "py_solved_MS_presentations.txt", "py_solved_MS_paths.txt",  int(1e5), 18)

    t2 = time.perf_counter()
    
    print("Time taken:", t2 - t1)
