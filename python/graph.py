import matplotlib.pyplot as plt
import numpy as np
import random

data = []
with open("/Users/kseniia/Desktop/programming/Projects/ACC/results/qgramdistance.txt", 'r') as f:
    data = [round(float(i) * 100) for i in f.read().strip().split()]

counts, bins, patches = plt.hist(data, bins = range(min(data), max(data) + 2))

bin_centres = [0.5 + i for i in data]
plt.xticks(bin_centres, [int(i - 0.5) for i in bin_centres])

plt.tick_params(labelsize=2)
plt.savefig('/Users/kseniia/Desktop/programming/Projects/ACC/results/qgramdistance_figure', dpi=400)
plt.clf()

print(counts)
