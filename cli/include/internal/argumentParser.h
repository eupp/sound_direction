#pragma once

#include "trikSound/settings.h"
#include "trikSound/trikSoundController.h"

#include <unordered_map>
#include <string>

class ArgumentParser
{
private:

    class Parameter;

public:

    class ParseException : public trikSound::TrikSoundException
    {
    public:
        ParseException(const std::string& msg):
            TrikSoundException(msg.c_str())
        {}
    };

    typedef trikSound::Settings Settings;

    static Settings parse();

    friend bool operator==(const QString& paramName, const Parameter& param);

private:

    class Parameter
    {
    public:

        Parameter();
        Parameter(std::string alias, std::string name, std::string errorString);

        std::string name() const;
        std::string alias() const;
        std::string errorString() const;

    private:

        std::string mName;
        std::string mAlias;
        std::string mErrorString;
    };

    static Settings parseArgumentList(const QStringList& args);
    static void checkSettings(const Settings& settings);

    static void parseDuration(QStringList::ConstIterator& it, Settings& settings);
    static void parseChannelCount(QStringList::ConstIterator& it, Settings& settings);
    static void parseMicrDistance(QStringList::ConstIterator& it, Settings& settings);
    static void parseHistoryDepth(QStringList::ConstIterator& it, Settings& settings);
    static void parseWindowSize(QStringList::ConstIterator& it, Settings& settings);

    static std::unordered_map<const char*, Parameter> paramsMap;
};
