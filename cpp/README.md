Compile & run: 

__Directory Path__ = path to the folder
__Input File Name__ = name of the input file (located in the ACC directory)
$ 
g++ -O2 -std=c++17 main.cpp support.cpp gready.cpp generate_datasets.cpp load_datasets.cpp evaluate.cpp insert-moves-variant.cpp insertmovesrotate.cpp analyse.cpp similarity.cpp -o program && ./program __Directory Path__ __Input File Name__

$ 
g++ -O2 -std=c++17 main.cpp support.cpp gready.cpp generate_datasets.cpp load_datasets.cpp evaluate.cpp insert-moves-variant.cpp insertmovesrotate.cpp analyse.cpp similarity.cpp distance_comparison.cpp -o program && ./program /Users/kseniia/Desktop/programming/Projects/ACC all_presentations.txt

$
g++ -O2 -std=c++17 *.cpp -o program && ./program /Users/kseniia/Desktop/programming/Projects/ACC all_presentations.txt

Generate data for statistical analysis:
1) Run timed_evaluation(argv, greedy_search_insertmovesrotate, "/smallest_rotation_solved_presentations.txt", "smallest_rotation_solved_moves.txt") -- writes paths (2 files: solved presentations and paths in terms of trivialisation moves) for all presentations 
2) Run global_writing(load_presentations_MS(dir_path + "/all_presentations.txt"), dir_path + "/results/smallest_rotation_solved_moves.txt") -- generates a file with paths (in terms of all nodes on the path) for all presentations
3) Run write_multi_paths(dir_path + "/all_presentations.txt", dir_path) -- to write explicit paths to individual files in results/files directory 
4) Run write_pairwise_metric_results(extract_paths(dir_path + "/results/smallest_rotation_solved_moves_explicit_paths.txt"), dir_path, "/pairwise_metrics.txt");
, dir_path) -- to create a file with pairwise metrics (qgram, edit, lcs) 

timed_evaluation(argv, greedy_search_insertmovesrotate, "/smallest_rotation_solved_presentations.txt", "smallest_rotation_solved_moves.txt");
global_writing(load_presentations_MS(dir_path + "/all_presentations.txt"), dir_path + "/results/smallest_rotation_solved_moves.txt");
write_multi_paths(dir_path + "/all_presentations.txt", dir_path);
write_pairwise_metric_results(extract_paths(dir_path + "/results/smallest_rotation_solved_moves_explicit_paths.txt"), dir_path, "/pairwise_metrics.txt");
