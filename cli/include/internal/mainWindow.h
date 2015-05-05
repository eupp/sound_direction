#pragma once

#include <memory>
#include <vector>

#include <QMainWindow>

#include "trikSound/trikSoundController.h"
#include "trikSound/iSettingsProvider.h"
#include "trikSound/iAudioEventListener.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
                 , public trikSound::IAudioEventListener
                 , public trikSound::ISettingsProvider
{
    Q_OBJECT

public:

    explicit MainWindow(std::shared_ptr<trikSound::TrikSoundController> controller
                      , QWidget *parent = 0);
    ~MainWindow();

    void recieve(const trikSound::AudioEvent& event);

signals:

    void updateAngleDetectionHistoryDepth(int historyDepth);
    void updateWindowSize(size_t size);
    void updateVolume(double vol);

protected:

    void keyPressEvent(QKeyEvent * event);

private:
    Ui::MainWindow *ui;

    static const int historyDiff;
    static const double volDiff;

    std::weak_ptr<trikSound::TrikSoundController> mController;

    static const int SIZE = 60;
};
