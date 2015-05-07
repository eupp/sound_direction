import os
import matplotlib.pyplot as plt

from runner import Runner

class PlotBuilder(object):

    __angle_plt_name = 'angle_'
    __plt_ext = '.png'

    def __init__(self):

        pass

    def plot_sensor_out(self, data, params, destdir):

        test_name = os.path.splitext(os.path.basename(params['input_filename']))[0]
        window_size = params['window_size']
        history_depth = params['history_depth']

        plt.axis([0, len(data), -140, 100])
        plt.plot(data, color='blue')

        text = ('Test name: {tn} {sep}'
                'Window size = {ws} samples {sep}'
                'Hisory depth = {hd} windows {sep}'.format(tn=test_name,
                                                           ws=window_size,
                                                           hd=history_depth,
                                                           sep=os.linesep))

        plt.text(1, -120, text, bbox=dict(boxstyle='square,pad=0.3', ec='black', fc='white'))

        figname = self.__angle_plt_name + test_name + self.__plt_ext
        figname = os.path.join(destdir, figname)
        plt.savefig(figname)
        plt.clf()
