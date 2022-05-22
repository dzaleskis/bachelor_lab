from importlib.metadata import distribution
import math
import os
import sys

import json
import numpy
from matplotlib import pyplot as plt

path = sys.argv[1]
distribution = sys.argv[2]
size = sys.argv[3]
filename = os.path.basename(path)
*rest, result_size = filename.split('_')
file = open(path)
jsonStr = file.read()
file.close()

benches = json.loads(jsonStr)

filteredBenches = list(filter(lambda x: (x["distribution"] == distribution and x["size"] == int(size)), benches))
algNames = list(map(lambda x: x["algorithm"], filteredBenches))
# what we have is a collection of benchmarks for same data size and distribution (so only many algos)
# what we need is a plot of algorithms, where bars are cycles, comparisons and assignments for each algorithm 

criteria = ["comparisons", "assignments", "cycles"]
groupnames = list(map(lambda x: x["algorithm"], filteredBenches))
groupsize = len(criteria) # cmp, assign, cycles
barwidth = 0.6
spacing = 1
groupwidth = groupsize * barwidth + spacing
colors = ["#1f77b4", "#aec7e8", "#484c66", "#ff7f0e", "#800080", "#e60000"]

for i, criterion in enumerate(criteria):
    heights = list(map(lambda b: b[criterion]["median"] / (b["size"] * math.log2(b["size"])), filteredBenches))
    errors = list(map(lambda b: b[criterion]["iqr"] / (b["size"] * math.log2(b["size"])), filteredBenches))
    plt.bar([barwidth*i + groupwidth*n for n in range(len(groupnames))],
        heights, 0.6, color = colors[i], label = criterion, yerr=errors, ecolor="#6f8282")

# Set axes limits and labels.
plt.xticks([barwidth * groupsize/2 + groupwidth*n for n in range(len(groupnames))], groupnames)

# Turn off ticks for y-axis.
plt.tick_params(
    axis="x",
    which="both",
    left="off",
    right="off",
    labelleft="on"
)

ax = plt.gca()
ax.relim()
ax.autoscale_view()
plt.legend(loc="upper right", fontsize=10, bbox_to_anchor=(1.3, 1))

figure = plt.gcf()
figure.set_size_inches(8*.75, 6*.75)
plt.savefig(os.path.join("plots", distribution + "_" + size + "_" + result_size + ".jpg"),
            dpi = 150, bbox_inches="tight")

plt.clf()
