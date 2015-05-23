import os
import math

from runner import Runner
from plot_builder import PlotBuilder

class Worker(object):

    __plots_dir = ''
    __plotter = None
    __args = []
    __tests = []
    __destdir = ''

    def __init__(self, args_filename, wav_dir, destdir):

        self.__destdir = destdir
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

        resfn = os.path.join(self.__destdir, 'res.txt')
        resfd = open(resfn, 'w')

        for args in self.__args:

            destdir = 'args_set{}'.format(str(i))
            destdir = os.path.join(self.__plots_dir, destdir)
            if not os.path.isdir(destdir):
                os.mkdir(destdir)
            i += 1

            sumdev = 0
            maxdev = -1

            devs = []
            for test in self.__tests:

                fname = os.path.splitext(os.path.basename(test))[0]
                angle = int(fname.split('_')[1])
                angle -= 90

                ext_args = list(args)
                ext_args.extend([Runner.PARAM_INPUT_FN[0], test])
                runner = Runner(ext_args)
                ret = runner.run()

                if ret != 0:
                    print 'Program finished with bad return code'
                    print 'Test name: {tn}'.format(tn=test)
                    print "Args: {args}".format(args=args)
                    continue

                data = {}
                data['vad'] = runner.get_vad_out()
                data['angle'] = runner.get_angle_out()
                # self.__plotter.plot_sensor_out(data, runner.get_args(), destdir)

                dev = self.__calc_dev(angle, data['angle'])
                devs.append(dev)
                sumdev += dev
                if dev > maxdev:
                    maxdev = dev

                print 'args_set{i} : {tn} finished; dev={dev}'.format(i=i, tn=test, dev=dev)

            avrdev = float(sumdev) / len(self.__tests)
            ddev = 0
            for dev in devs:
                ddev += (dev - avrdev) ** 2
            ddev = math.sqrt(float(ddev) / len(devs))

            hd = runner.get_args()['history_depth']
            resfd.write('{:^4}, {:+12.6f}, {:+12.6f}, {:+12.6f} {}'.format(hd, avrdev, ddev, maxdev, os.linesep))


    def __calc_dev(self, angle, data):

        dev = 0
        for x in data:
            xd = x - angle
            xd *= xd
            dev += xd
        dev = math.sqrt(float(dev) / len(data))
        return dev



