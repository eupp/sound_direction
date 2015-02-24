from matplotlib.pyplot import *
from sys import argv
from os.path import basename, splitext

if __name__ == "__main__":

    try:
        src = argv[1]
    except IndexError:
        print "Filename is missing"
        exit()

    try:
        dst = argv[2]
    except IndexError:
        dst = splitext(basename(argv[1]))[0]

    try:
        col = int(argv[3])
    except IndexError:
        col = None

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

