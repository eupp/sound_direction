#include "argumentParser.h"

#include <unordered_map>

#include <QCoreApplication>

using namespace std;
using namespace trikSound;

std::unordered_map<const char*, ArgumentParser::Parameter> ArgumentParser::paramsMap =
{
    {"filename", ArgumentParser::Parameter("-f", "--filename", "Filename is missing.")}
  , {"outfile", ArgumentParser::Parameter("-o", "--output-filename", "Output filename is missing.")}
  , {"duration", ArgumentParser::Parameter("-d", "--duration", "Duration is missing")}

  , {"channelCount", ArgumentParser::Parameter("", "--channels", "Channel count is missing.")}

  , {"angleDetection", ArgumentParser::Parameter("-A", "--angle", "")}
  , {"filtering", ArgumentParser::Parameter("-F", "--filtering", "")}
  , {"micrDist", ArgumentParser::Parameter("-D", "--micr-dist", "Microphone distance is missing.")}
  , {"historyDepth", ArgumentParser::Parameter("", "--history-depth", "History depth is missing.")}
  , {"windowSize", ArgumentParser::Parameter("", "--window-size", "Window size is missing.")}

  , {"volume", ArgumentParser::Parameter("-v", "--volume", "Volume is missing.")}
};

template <typename T>
T convertArgument(const QString& arg, bool& ok);

template <>
int convertArgument(const QString& arg, bool& ok)
{
    return arg.toInt(&ok);
}

template <>
double convertArgument(const QString& arg, bool& ok)
{
    return arg.toDouble(&ok);
}

template <typename T>
T parseArgument(const char* paramName, const QString& param, const string& errorString)
{
    bool ok = true;
    T res = convertArgument<T>(param, ok);
    if (!ok) {
        throw ArgumentParser::ParseException(errorString);
    }
    return res;
}

Settings ArgumentParser::parse()
{
    return parseArgumentList(QCoreApplication::arguments());
}

Settings ArgumentParser::parseArgumentList(const QStringList& args)
{
    Settings settings;

    for (auto it = args.begin(); it != args.end(); ++it) {

        if (*it == paramsMap["channelCount"]) {
            int count = parseArgument<int>("channelCount", *(++it), paramsMap["channelCount"].errorString());
            settings.setSingleChannelFlag(count == 1);
        }
        else if (*it == paramsMap["micrDist"]) {
            double dist = parseArgument<double>("micrDist", *(++it), paramsMap["micrDist"].errorString());
            settings.setMicrDist(dist);
        }
        else if (*it == paramsMap["historyDepth"]) {
            int depth = parseArgument<int>("historyDepth", *(++it), paramsMap["historyDepth"].errorString());
            settings.setAngleDetectionHistoryDepth(depth);
        }
        else if (*it == paramsMap["windowSize"]) {
            int wsize = parseArgument<int>("windowSize", *(++it), paramsMap["windowSize"].errorString());
            settings.setWindowSize(wsize);
        }
        else if (*it == paramsMap["angleDetection"]) {
            settings.setAngleDetectionFlag(true);
        }
        else if (*it == paramsMap["duration"]) {
            int duration = parseArgument<int>("duration", *(++it), paramsMap["duration"].errorString());
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
    }

    checkSettings(settings);

    return settings;
}

void ArgumentParser::checkSettings(const Settings& settings)
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


