from matplotlib.pyplot import *
from sys import argv
from os.path import basename, splitext

if __name__ == "__main__":

    src = argv[1]
    dst = argv[2]
    col = int(argv[3])

    if dst is None:
        dst = splitext(basename(argv[1]))[0]

    fsrc = open(src)

    data = []        
    if col is not None:
        for row in fsrc:
            vals = row.split(" ")            
            if col >= len(vals) - 1:
                continue
            val = float(vals[col])
            data.append(val)
    else:
        data = [float(x) for x in fsrc]
    
    #axis([0, 100, -100, 100])
    plot(data)
    savefig(dst)

