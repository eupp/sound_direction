Sound Direction
===============

Calculates angle to the sound source using signals from two microphones.

Build For TRIK
--------------

```bash
cd $PROJECT_PATH
source $TRIK_SDK/environment-setup-armv5te-oe-linux-gnueabi
qmake
make
```

Basic options
--------------

* -D, --micr-dist   -   Half distance between microphones
* -A, --angle       -   Enable angle detection
* -V, --vad         -   Enable voice activity detection
* -T, --threshold   -   Threshold for vad
* -F, --filtering   -   Enable filtering
* -v, --volume      -   Microphone volume
* -s, --show        -   Output format (a - output angle, v - output vad coefficient, av - both)

Advanced options
--------------

* -f, --filename        -   Input filename (instead of microphone input)
* -o, --output-filename -   Output filename (for output audio stream)
* -d, --duration        -   Duration for capturing
* --channels            -   Count of channels
* --history-depth       -   Size of history for angle detector
* --window-size         -   Size of window in samples

