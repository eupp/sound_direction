
import numpy as np

# calculate vector of variances for each test
# (i.e for each signal calculate vector of variances of values with different history depth)
def calc_variance(tests):
    vars = []
    for test in tests:
        var = [np.var(vector) for vector in test]
        vars.append(var)
    return np.array(vars)

# sort values of history depth such as values with minimal variance across tests
# placed in the begin
def calc_topmin(var_matrix):
    counts = np.zeros(var_matrix.shape[1])
    for test in var_matrix:
        ind = test.argmin()
        counts[ind] += 1
    counts_ind = [(i, count) for (i, count) in zip(range(0, counts.size), counts)]
    counts_ind.sort(key = lambda pair: pair[1])
    counts_ind = counts_ind [::-1]
    return counts_ind


# calculate mean of variances for each history depth value
def calc_var_mean(var_matrix):
    return var_matrix.mean(0)

if __name__ == "__main__":

    test1 = [[1, 1, 1],         # var = 0
             [2, 1, 2, 1],      # var = 0.25
             [4, 1, 4, 1, 0]]   # var = 2.8

    test2 = [[0, 3, 0],         # var = 2
             [3, 3, 2, 4],      # var = 0.5
             [3, 6, 1, 8, 2]]   # var = 6.8

    tests = [test1, test2]

    vars = calc_variance(tests)

    print "\n Variaces: \n"
    print vars
    print "\n Mean of variaces: \n"
    print calc_var_mean(vars)
    print "\n Count of index is min: \n"
    print calc_topmin(vars)