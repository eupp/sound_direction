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

#include "argumentParser.h"

#include <unordered_map>

#include <QCoreApplication>
#include <QStringList>

#include "trikSound/types.h"

#include "utils.h"

using namespace std;
using trikSound::threshold_type;

std::unordered_map<const char*, ArgumentParser::Parameter> ArgumentParser::paramsMap =
{
    {"filename", ArgumentParser::Parameter("-f", "--filename", "Filename is missing.")}
  , {"outfile", ArgumentParser::Parameter("-o", "--output-filename", "Output filename is missing.")}
  , {"duration", ArgumentParser::Parameter("-d", "--duration", "Duration is missing")}

  , {"channelCount", ArgumentParser::Parameter("", "--channels", "Channel count is missing.")}

  , {"angleDetection", ArgumentParser::Parameter("-A", "--angle", "")}
  , {"vad", ArgumentParser::Parameter("-V", "--vad", "")}
  , {"vadThreshold", ArgumentParser::Parameter("-T", "--threshold", "Threshold is missing")}
  , {"filtering", ArgumentParser::Parameter("-F", "--filtering", "")}
  , {"micrDist", ArgumentParser::Parameter("-D", "--micr-dist", "Microphone distance is missing.")}
  , {"historyDepth", ArgumentParser::Parameter("", "--history-depth", "History depth is missing.")}
  , {"windowSize", ArgumentParser::Parameter("", "--window-size", "Window size is missing.")}

  , {"audioDeviceInit",  ArgumentParser::Parameter("-I", "--force-init", "")}
  , {"volume", ArgumentParser::Parameter("-v", "--volume", "Volume is missing.")}

  , {"show", ArgumentParser::Parameter("-s", "--show", "Output formatting string is missing")}

  , {"diffTime", ArgumentParser::Parameter("", "--diff-time", "Diff time is missing")}
};

std::unordered_map<const char*, char> ArgumentParser::showFormatting =
{
    {"angle", 'a'}
  , {"vad", 'v'}
};

Settings ArgumentParser::parse()
{
    return parseArgumentList(QCoreApplication::arguments());
}

Settings ArgumentParser::parseArgumentList(const QStringList& args)
{
    return Settings(parseControllerSettings(args), parseViewSettings(args));
}

ControllerSettings ArgumentParser::parseControllerSettings(const QStringList& args)
{
    ControllerSettings settings;

    for (auto it = args.begin(); it != args.end(); ++it) {

        if (*it == paramsMap["channelCount"]) {
            checkArgsEnd(++it, args.end(), paramsMap["channelCount"].errorString());
            int count = convertParam<int>("channelCount", *it, paramsMap["channelCount"].errorString());
            settings.setSingleChannelFlag(count == 1);
        }
        else if (*it == paramsMap["micrDist"]) {
            checkArgsEnd(++it, args.end(), paramsMap["micrDist"].errorString());
            double dist = convertParam<double>("micrDist", *it, paramsMap["micrDist"].errorString());
            settings.setMicrDist(dist);
        }
        else if (*it == paramsMap["vadThreshold"]) {
            checkArgsEnd(++it, args.end(), paramsMap["vadThreshold"].errorString());
            threshold_type thrsd =
                    convertParam<threshold_type>("vadThreshold", *it, paramsMap["vadThreshold"].errorString());
            settings.setVadThreshold(thrsd);
        }
        else if (*it == paramsMap["historyDepth"]) {
            checkArgsEnd(++it, args.end(), paramsMap["historyDepth"].errorString());
            int depth = convertParam<int>("historyDepth", *it, paramsMap["historyDepth"].errorString());
            settings.setAngleDetectionHistoryDepth(depth);
        }
        else if (*it == paramsMap["windowSize"]) {
            checkArgsEnd(++it, args.end(), paramsMap["windowSize"].errorString());
            int wsize = convertParam<int>("windowSize", *it, paramsMap["windowSize"].errorString());
            settings.setWindowSize(wsize);
        }
        else if (*it == paramsMap["duration"]) {
            checkArgsEnd(++it, args.end(), paramsMap["duration"].errorString());
            int duration = convertParam<int>("duration", *it, paramsMap["duration"].errorString());
            settings.setDurationFlag(true);
            settings.setDuration(duration);
        }
        else if (*it == paramsMap["filename"]) {
            settings.setFileInputFlag(true);
            if (++it != args.end()) {
                settings.setInputWavFilename(*it);
            }
            else {
                throw ParseException(paramsMap["filename"].errorString());
            }
        }
        else if (*it == paramsMap["outfile"]) {
            settings.setRecordStreamFlag(true);
            if (++it != args.end()) {
                settings.setOutputWavFilename(*it);
            }
            else {
                throw ParseException(paramsMap["outfile"].errorString());
            }
        }
        else if (*it == paramsMap["angleDetection"]) {
            settings.setAngleDetectionFlag(true);
        }
        else if (*it == paramsMap["filtering"]) {
            settings.setFilteringFlag(true);
        }
        else if (*it == paramsMap["vad"]) {
            settings.setVadFlag(true);
        }
        else if (*it == paramsMap["audioDeviceInit"]) {
            settings.setAudioDeviceInitFlag(true);
        }
    }

    checkSettings(settings);

    return settings;
}

ViewSettings ArgumentParser::parseViewSettings(const QStringList& args)
{
    ViewSettings settings;

    for (auto it = args.begin(); it != args.end(); ++it) {
        if (*it == paramsMap["show"]) {
            if (++it != args.end()) {
                QString show = *it;
                if (show.contains(showFormatting["angle"])) {
                    settings.setShowAngle(true);
                }
                if (show.contains(showFormatting["vad"])) {
                    settings.setShowVadCoef(true);
                }
            }
        }
        else if (*it == paramsMap["diffTime"]) {
            checkArgsEnd(++it, args.end(), paramsMap["duration"].errorString());
            int diffTime = convertParam<int>("diffTime", *it, paramsMap["diffTime"].errorString());
            settings.setDiffTime(diffTime);
        }
    }

    return settings;
}

void ArgumentParser::checkArgsEnd(const QStringList::const_iterator& it,
                                  const QStringList::const_iterator& end,
                                  string errMsg)
{
    if (it == end) {
        throw ParseException(errMsg);
    }
}

void ArgumentParser::checkSettings(const ControllerSettings& settings)
{
    if (settings.angleDetectionFlag() && settings.singleChannelFlag()) {
        throw ParseException("Angle detection enabled but single channel capturing specified");
    }
}

ArgumentParser::Parameter::Parameter():
    mName("")
  , mAlias("")
  , mErrorString("")
{}

ArgumentParser::Parameter::Parameter(string alias, string name, string errorString):
    mName(name)
  , mAlias(alias)
  , mErrorString(errorString)
{}

std::string ArgumentParser::Parameter::name() const
{
    return mName;
}

std::string ArgumentParser::Parameter::alias() const
{
    return mAlias;
}

std::string ArgumentParser::Parameter::errorString() const
{
    return mErrorString;
}

bool operator==(const QString& paramName,
                const ArgumentParser::Parameter& param)
{
    if (paramName == "") {
        return false;
    }
    return paramName == param.name().c_str() || paramName == param.alias().c_str();
}


