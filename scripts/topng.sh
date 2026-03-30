
#!/bin/sh

if [ $# -ne 5 ]; then
    echo "Usage: $0 <file> <width> <height> <bpp> <out>"
    exit 1
fi

FILE=$1
WIDTH=$2
HEIGHT=$3
BPP=$4
OUT=$5

magick -size ${WIDTH}x${HEIGHT} -depth ${BPP} gray:${FILE} ${OUT}

