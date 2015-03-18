#include "argumentParser.h"

#include <unordered_map>

#include <QCoreApplication>

using namespace std;
using namespace trikSound;

std::unordered_map<const char*, ArgumentParser::Parameter> ArgumentParser::paramsMap =
{
    {"filename", ArgumentParser::Parameter("-f", "--filename", "Filename is missing.")}
  , {"outfile", ArgumentParser::Parameter("-o", "--output-filename", "Output filename is missing.")}

  , {"channelCount", ArgumentParser::Parameter("", "--channels", "Channel count is missing.")}

  , {"angleDetection", ArgumentParser::Parameter("-a", "--angle", "")}
  , {"micrDist", ArgumentParser::Parameter("-d", "--micr_dist", "Microphone distance is missing.")}
  , {"historyDepth", ArgumentParser::Parameter("", "--history-depth", "History depth is missing.")}
  , {"windowSize", ArgumentParser::Parameter("", "--window-size", "Window size is missing.")}

  , {"volume", ArgumentParser::Parameter("-v", "--volume", "Volume is missing.")}
};

trikSound::TrikSoundController::Settings ArgumentParser::parse()
{
    return parseArgumentList(QCoreApplication::arguments());
}

TrikSoundController::Settings ArgumentParser::parseArgumentList(const QStringList& args)
{
    trikSound::TrikSoundController::Settings settings;

    for (auto it = args.begin(); it != args.end(); ++it) {

        if (*it == paramsMap["channelCount"]) {
            parseChannelCount(it, settings);
            continue;
        }
        if (*it == paramsMap["micrDist"]) {
            parseMicrDistance(it, settings);
            continue;
        }
        if (*it == paramsMap["historyDepth"]) {
            parseHistoryDepth(it, settings);
            continue;
        }
        if (*it == paramsMap["windowSize"]) {
            parseWindowSize(it, settings);
            continue;
        }
        if (*it == paramsMap["angleDetection"]) {
            settings.setAngleDetectionFlag(true);
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

void ArgumentParser::parseChannelCount(QStringList::ConstIterator& it, Settings& settings)
{
    ++it;
    bool ok = true;
    int count = it->toInt(&ok);
    if (!ok) {
        throw ParseException(paramsMap["channelCount"].errorString());
    }
    settings.setSingleChannelFlag(count == 1);
}

void ArgumentParser::parseMicrDistance(QStringList::ConstIterator& it, ArgumentParser::Settings& settings)
{
    ++it;
    bool ok = true;
    double micrDist = it->toDouble(&ok);
    if (!ok) {
        throw ParseException(paramsMap["micrDist"].errorString());
    }
    settings.setMicrDist(micrDist);
}

void ArgumentParser::parseHistoryDepth(QStringList::ConstIterator& it, ArgumentParser::Settings& settings)
{
    ++it;
    bool ok = true;
    int histDepth = it->toInt(&ok);
    if (!ok) {
        throw ParseException(paramsMap["historyDepth"].errorString());
    }
    settings.setAngleDetectionHistoryDepth(histDepth);
}

void ArgumentParser::parseWindowSize(QStringList::ConstIterator& it, ArgumentParser::Settings& settings)
{
    ++it;
    bool ok = true;
    int windowSize = it->toInt(&ok);
    if (!ok) {
        throw ParseException(paramsMap["windowSize"].errorString());
    }
    settings.setWindowSize(windowSize);
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


