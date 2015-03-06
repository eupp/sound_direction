__author__ = 'evgeniy'

# read data with sensor values with different history depth from file
def read_data_hd(filename, max_history_depth):
    dfile = open(filename)
    data = [[] for i in range(0, max_history_depth)]
    j = 0
    for row in dfile:
        vals = row.split(" ")
        for i, val in zip(range(0, max_history_depth), vals):
            if val == '\n':
                continue
            fval = float(val)
            data[i].append(fval)
        j += 1
    return data


if __name__ == "__main__":
    data = read_data_hd("data.test", 10)

    for row in data:
        print row