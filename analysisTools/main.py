import os
import sys

import input_parser as inpars
import varals

def walk_dir(curr_dir, test_data):
    test_filename = "testWindow.test"
    for d, dirs, files in os.walk(curr_dir):
        for file in files:
            if os.path.basename(file) != test_filename:
                continue
            file_path = os.path.join(d, file)
            # print file_path
            fdata = inpars.read_data_hd(file_path, 10)
            test_data.append(fdata)

if __name__ == "__main__":

    # basedir = sys.argv[1]
    basedir = "../tests/test_results/"
    test_data = []
    walk_dir(basedir, test_data)

    vars = varals.calc_variance(test_data)


    print "Test count {count} {sep}".format(count = len(test_data), sep = os.linesep)
    # print "Variance: {sep}".format(sep = os.linesep)
    # print vars
    print "{sep}Variance mean: {sep}".format(sep = os.linesep)
    print varals.calc_var_mean(vars)
    print "{sep}Variance min: {sep}".format(sep = os.linesep)
    print varals.calc_topmin(vars)[0:3]


