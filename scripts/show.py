#!/usr/bin/env python3

import numpy as np
import matplotlib.pyplot as plt
import sys

if len(sys.argv) != 5:
    print("Usage: python view.py <file> <width> <height> <bpp>")
    sys.exit(1)

filename = sys.argv[1]
width    = int(sys.argv[2])
height   = int(sys.argv[3])
bpp      = int(sys.argv[4])

dtype = {8: np.uint8, 16: np.uint16, 32: np.uint32}.get(bpp)
if dtype is None:
    print(f"Unsupported bpp: {bpp}")
    sys.exit(1)

data = np.fromfile(filename, dtype=dtype)
img  = data.reshape((height, width))

plt.imshow(img, cmap="gray", vmin=0, vmax=(1 << bpp) - 1)
plt.title(f"{filename} {width}x{height} {bpp}bpp")
plt.show()
