#!/usr/bin/env python3
import pandas as pd
import matplotlib.pyplot as plt
import sys

if len(sys.argv) != 2:
    print("Usage: python view_hist.py <file>")
    sys.exit(1)

filename = sys.argv[1]

# find where bins start
with open(filename) as f:
    lines = f.readlines()

stats_lines = []
bins_start = 0
for i, line in enumerate(lines):
    if line.startswith("bin,"):
        bins_start = i
        break
    stats_lines.append(line)

# parse stats
import io
stats_df = pd.read_csv(io.StringIO("".join(stats_lines)), index_col="channel")
ch_names = ["R", "Gr", "Gb", "B"]
stats_df.index = ch_names

# parse bins
bins_df = pd.read_csv(io.StringIO("".join(lines[bins_start:])), index_col="bin")

# plot bins
fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(12, 8))
fig.suptitle(filename)

bins_df.plot(ax=ax1, color={"R": "red", "Gr": "green", "Gb": "lime", "B": "blue"})
ax1.set_xlabel("bin")
ax1.set_ylabel("count")

# plot stats as table
ax2.axis("off")
table = ax2.table(
    cellText=stats_df.values.round(2),
    colLabels=stats_df.columns,
    rowLabels=stats_df.index,
    loc="center"
)
table.scale(1, 1.5)

plt.tight_layout()
plt.show()
