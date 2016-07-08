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

#pragma once

#include <QStringList>

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

    static void checkArgsEnd(const QStringList::const_iterator& it,
                             const QStringList::const_iterator& end,
                             std::string errMsg);

    static void checkSettings(const ControllerSettings& settings);

    static std::unordered_map<const char*, Parameter> paramsMap;
    static std::unordered_map<const char*, char>      showFormatting;
};
