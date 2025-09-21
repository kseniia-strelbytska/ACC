Compile & run: 

__Directory Path__ = path to the folder
__Input File Name__ = name of the input file (located in the ACC directory)
$ 
g++ -O2 -std=c++17 main.cpp support.cpp gready.cpp generate_datasets.cpp load_datasets.cpp evaluate.cpp -o program && ./program __Directory Path__ __Input File Name__

$ 
g++ -O2 -std=c++17 main.cpp support.cpp gready.cpp generate_datasets.cpp load_datasets.cpp evaluate.cpp -o program && ./program /Users/kseniia/Desktop/programming/Projects/ACC all_presentations.txt
