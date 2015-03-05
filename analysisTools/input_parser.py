__author__ = 'evgeniy'

# read data with sensor values with different history depth from file
def read_data_hd(filename, max_history_depth):
    dfile = open(filename)
    data = [[] for i in range(0, max_history_depth)]
    for row in dfile:
        vals = row.split(" ")
        for i, val in zip(range(0, max_history_depth), vals):
            fval = float(val)
            data[i].append(fval)
    return data

