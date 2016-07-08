/* Copyright 2014 - 2016 Evgenii Moiseenko.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#include "settings.h"

namespace trikSound {



Settings::Settings():

    mSingleChannelFlag(false)
  , mFilteringFlag(false)
  , mAngleDetectionFlag(false)
  , mVadFlag(false)
  , mRecordStreamFlag(false)
  , mFileInputFlag(false)
  , mAudioDeviceInitFlag(false)

  , mSampleRate(44100)
  , mSampleSize(16)
  , mSampleType(QAudioFormat::SignedInt)

  , mAngleDetectionHistoryDepth(5)
  , mWindowSize(1024)
  , mVolume(1.0)

  , mMicrDist(10.0)
  , mVadThreshold(0)

  , mDurationFlag(false)
  , mDuration(0)
{}


int Settings::angleDetectionHistoryDepth() const
{
    return mAngleDetectionHistoryDepth;
}

void Settings::setAngleDetectionHistoryDepth(int angleDetectionHistoryDepth)
{
    mAngleDetectionHistoryDepth = angleDetectionHistoryDepth;
}

size_t Settings::windowSize() const
{
    return mWindowSize;
}

void Settings::setWindowSize(const size_t& windowSize)
{
    mWindowSize = windowSize;
}

double Settings::volume() const
{
    return mVolume;
}

void Settings::setVolume(double volume)
{
    mVolume = volume;
}

QAudioFormat::SampleType Settings::sampleType() const
{
    return mSampleType;
}

void Settings::setSampleType(const QAudioFormat::SampleType& sampleType)
{
    mSampleType = sampleType;
}

int Settings::sampleSize() const
{
    return mSampleSize;
}

void Settings::setSampleSize(int sampleSize)
{
    mSampleSize = sampleSize;
}

int Settings::sampleRate() const
{
    return mSampleRate;
}

void Settings::setSampleRate(int sampleRate)
{
    mSampleRate = sampleRate;
}

bool Settings::angleDetectionFlag() const
{
    return mAngleDetectionFlag;
}

void Settings::setAngleDetectionFlag(bool angleDetectionFlag)
{
    mAngleDetectionFlag = angleDetectionFlag;
}

bool Settings::filteringFlag() const
{
    return mFilteringFlag;
}

void Settings::setFilteringFlag(bool filteringFlag)
{
    mFilteringFlag = filteringFlag;
}

bool Settings::singleChannelFlag() const
{
    return mSingleChannelFlag;
}

void Settings::setSingleChannelFlag(bool singleChannelFlag)
{
    mSingleChannelFlag = singleChannelFlag;
}

double Settings::micrDist() const
{
    return mMicrDist;
}

void Settings::setMicrDist(double micrDist)
{
    mMicrDist = micrDist;
}

bool Settings::recordStreamFlag() const
{
    return mRecordStreamFlag;
}

void Settings::setRecordStreamFlag(bool recordStreamFlag)
{
    mRecordStreamFlag = recordStreamFlag;
}

QString Settings::outputWavFilename() const
{
    return mOutputWavFilename;
}

void Settings::setOutputWavFilename(const QString& outputWavFilename)
{
    mOutputWavFilename = outputWavFilename;
}

int Settings::duration() const
{
    return mDuration;
}

void Settings::setDuration(int duration)
{
    mDuration = duration;
}

bool Settings::durationFlag() const
{
    return mDurationFlag;
}

void Settings::setDurationFlag(bool durationSetFlag)
{
    mDurationFlag = durationSetFlag;
}

QString Settings::inputWavFilename() const
{
    return mInputWavFilename;
}

void Settings::setInputWavFilename(const QString& inputWavFilename)
{
    mInputWavFilename = inputWavFilename;
}

bool Settings::audioDeviceInitFlag() const
{
    return mAudioDeviceInitFlag;
}

void Settings::setAudioDeviceInitFlag(bool audioDeviceInitFlag)
{
    mAudioDeviceInitFlag = audioDeviceInitFlag;
}

bool Settings::vadFlag() const
{
    return mVadFlag;
}

void Settings::setVadFlag(bool vadFlag)
{
    mVadFlag = vadFlag;
}

bool Settings::fileInputFlag() const
{
    return mFileInputFlag;
}

void Settings::setFileInputFlag(bool fileInputFlag)
{
    mFileInputFlag = fileInputFlag;
}

double Settings::vadThreshold() const
{
    return mVadThreshold;
}

void Settings::setVadThreshold(double vadThreshold)
{
    mVadThreshold = vadThreshold;
}

}
