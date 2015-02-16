
from matplotlib.pyplot import *
from sys import argv
from os.path import basename, splitext

if __name__ == "__main__":
    ps = open(argv[1]) 
    xs = [float(x) for x in ps]
    plot(xs)
        
    fn = splitext(basename(argv[1]))[0]
    savefig(fn)

