import math
import os
import sys

import json
import numpy
from matplotlib import pyplot as plt

path = sys.argv[1]
criterion = sys.argv[2]
filename = os.path.basename(path)
*rest, result_size = filename.split('_')
file = open(path)
jsonStr = file.read()
file.close()

benches = json.loads(jsonStr)

benchesBySize = {}

for bench in benches:
    size = bench["size"]    
    if not size in benchesBySize: benchesBySize[size] = []

    benchesBySize[size].append(bench)


for size, benchesThisSize in benchesBySize.items():
    benchesByDistribution = {}

    for bench in benchesThisSize:
        distribution = bench["distribution"]
        if not distribution in benchesByDistribution: benchesByDistribution[distribution] = []

        benchesByDistribution[distribution].append(bench)
   
    benchesByAlgo = {}

    for bench in benchesThisSize:
        algorithm = bench["algorithm"]    
        if not algorithm in benchesByAlgo: benchesByAlgo[algorithm] = []

        benchesByAlgo[algorithm].append(bench)

    groupnames = benchesByDistribution.keys()
    firstGroup = next(iter(benchesByDistribution.values()))
    groupsize = len(firstGroup)
    barwidth = 0.6
    spacing = 1
    groupwidth = groupsize * barwidth + spacing
    colors = ["#1f77b4", "#aec7e8", "#484c66", "#ff7f0e", "#800080", "#e60000"]

    for i, (name, benches) in enumerate(benchesByAlgo.items()):
        heights = list(map(lambda b: b[criterion]["median"] / (b["size"] * math.log2(b["size"])), benches))
        errors = list(map(lambda b: b[criterion]["iqr"] / (b["size"] * math.log2(b["size"])), benches))
        plt.bar([barwidth*i + groupwidth*n for n in range(groupsize)],
                heights, 0.6, color = colors[i], label = name, yerr=errors, ecolor="#6f8282")

    plt.xticks([barwidth * groupsize/2 + groupwidth*n for n in range(len(groupnames))], groupnames)
    plt.ylabel("{} / n log n".format(criterion.capitalize()))
    
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
    plt.legend(loc="upper right", fontsize=10)

    figure = plt.gcf()
    figure.set_size_inches(12*.75, 6*.75)
    plt.savefig(os.path.join("plots", criterion + "_" + result_size + "_" + str(size) + ".jpg"),
                dpi = 150, bbox_inches="tight")

    plt.clf()
