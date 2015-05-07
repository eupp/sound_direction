import os
import sys

from worker import Worker

if __name__ == "__main__":

    worker = Worker('wav/args_file', 'wav/mdist_7_6', 'analysis_results')

    worker.do_angle_tests()


