import os

from runner import Runner
from plot_builder import PlotBuilder

class Worker(object):

    __plots_dir = ''
    __plotter = None
    __args = []
    __tests = []

    def __init__(self, args_filename, wav_dir, destdir):

        self.__plots_dir = os.path.join(destdir, 'plots')
        if not os.path.isdir(self.__plots_dir):
            os.mkdir(self.__plots_dir)
        self.__plotter = PlotBuilder()

        fd = open(args_filename)
        for line in fd:
            line = line.replace('\n', '')
            if line:
                self.__args.append(line.split(' '))

        for d, dirs, files in os.walk(wav_dir):
            for file in files:
                pfile = os.path.join(d, file)
                ext = os.path.splitext(pfile)[1]
                if ext == '.wav':
                    self.__tests.append(pfile)


    def do_angle_tests(self):

        i = 0
        for args in self.__args:

            destdir = 'args_set{}'.format(str(i))
            destdir = os.path.join(self.__plots_dir, destdir)
            if not os.path.isdir(destdir):
                os.mkdir(destdir)
            i += 1

            for test in self.__tests:

                ext_args = list(args)
                ext_args.extend([Runner.PARAM_INPUT_FN[0], test])
                runner = Runner(ext_args)
                ret = runner.run()

                if ret != 0:
                    print 'Program finished with bad return code'
                    print 'Test name: {tn}'.format(tn=test)
                    print "Args: {args}".format(args=args)
                    continue

                self.__plotter.plot_sensor_out(runner.get_stdout(), runner.get_args(), destdir)

                print 'args_set{i} : {tn} finished'.format(i=i, tn=test)




