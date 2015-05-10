#pragma once

#include "settings.h"
#include "trikSound/trikSoundException.h"

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
    static ControllerSettings parseControllerSettings(const QStringList& args);
    static ViewSettings parseViewSettings(const QStringList& args);

    static void checkSettings(const ControllerSettings& settings);

    static std::unordered_map<const char*, Parameter> paramsMap;
    static std::unordered_map<const char*, char>      showFormatting;
};
