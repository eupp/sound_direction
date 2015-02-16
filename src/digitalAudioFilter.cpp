#include "include/internal/digitalAudioFilter.h"

AudioBuffer DigitalAudioFilter::input(const AudioBuffer& buf)
{
    // to do: check is buffer in mono/stereo format

    QByteArray bytes(buf.size(), '\0');
    const sample_t* isignal = reinterpret_cast<const sample_t*>(buf.data());
    sample_t* osignal = reinterpret_cast<sample_t*>(bytes.data());
    filtfilt(isignal, isignal + buf.sampleCount(), osignal);
    AudioBuffer outBuf(bytes, buf.audioFormat());
    emit output(outBuf);
    return outBuf;
}

