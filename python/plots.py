import matplotlib.pyplot as plt
import numpy as np

last = -1 

d = []

with open('/Users/kseniia/Desktop/programming/Projects/ACC/results/solved_paths_rotate_MS_pathlenhistogram.txt', 'r') as f:
    for line in f:
        d.append([int(i) for i in line.split()])

pathlen = {}
for path in d:
    if path[0] not in pathlen:
        pathlen[path[0]] = []

    pathlen[path[0]].append(path[1]) 

# fig, axs = plt.subplots(1, len(pathlen), sharey = True, tight_layout=True)

ind = -1

for i in pathlen:
    ind += 1

    counts, bins, patches = plt.hist(pathlen[i], bins = range(min(pathlen[i]), max(pathlen[i]) + 2))

    plt.ylim(0, 20)

    bin_centres = [0.5 + i for i in pathlen[i]]
    plt.xticks(bin_centres, [int(i - 0.5) for i in bin_centres])
    plt.yticks(np.arange(0, 20, 1))

    plt.tick_params(labelsize=2)

    plt.savefig(f'/Users/kseniia/Desktop/programming/Projects/ACC/results/len{i}', dpi=400)
    plt.clf()

    print(counts)

