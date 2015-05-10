import os
import matplotlib.pyplot as plt
import scipy.io.wavfile as wav

from runner import Runner

class PlotBuilder(object):

    __angle_plt_name = 'angle_'
    __plt_ext = '.png'

    def __init__(self):

        pass

    def plot_sensor_out(self, data, params, destdir):

        test_path = params['input_filename']
        test_name = os.path.splitext(os.path.basename(test_path))[0]
        window_size = params['window_size']
        history_depth = params['history_depth']

        rate, wavdata = wav.read(test_path)
        left_chl  = wavdata[:, 0]
        right_chl = wavdata[:, 1]

        fig, plts = plt.subplots(4, 1, sharex=True)
        xlen = len(left_chl)

        # plts[0].axis([0, xlen, -2e15, 2e15])
        plts[0].set_title('Left channel')
        plts[0].plot(left_chl, color='blue', linestyle='-')

        # plts[1].axis([0, xlen, -2e15, 2e15])
        plts[1].set_title('Right channel')
        plts[1].plot(right_chl, color='blue', linestyle='-')

        wdots = range(0, xlen + window_size, window_size)
        wdots = wdots[:len(data['angle'])]

        # plts[2].axis([0, xlen, 0, 1e-1])
        plts[2].set_title('Energy')
        plts[2].plot(wdots, data['vad'], color='red', linestyle='-', marker='o')

        plts[3].axis([0, xlen, -120, 100])
        plts[3].set_title('Angle')
        plts[3].set_xlabel('Sample')
        plts[3].set_ylabel('Degree')
        plts[3].grid(True)
        plts[3].text(0, 90, '+90$^{\circ}$')
        plts[3].text(0, -90, '-90$^{\circ}$')
        plts[3].plot(wdots, data['angle'], color='green', linestyle='-', marker='o')

        text = ('Test name: {tn} {sep}'
                'Window size = {ws} samples {sep}'
                'Hisory depth = {hd} windows {sep}'.format(tn=test_name,
                                                           ws=window_size,
                                                           hd=history_depth,
                                                           sep=os.linesep))

        # plts[3].text(1, -120, text, bbox=dict(boxstyle='square,pad=0.3', ec='black', fc='white'))

        figname = self.__angle_plt_name + test_name + self.__plt_ext
        figname = os.path.join(destdir, figname)
        fig.savefig(figname)
        fig.clf()
