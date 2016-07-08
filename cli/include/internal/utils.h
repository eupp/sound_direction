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

#include <string>

#include <QString>

#include "trikSound/trikSoundException.h"

class ConvertationException : public trikSound::TrikSoundException
{
public:
    ConvertationException(const std::string& msg):
        TrikSoundException(msg.c_str())
    {}
};

template <typename T>
inline T convertParam(const QString& param, bool& ok);

template <>
inline int convertParam(const QString& param, bool& ok)
{
    return param.toInt(&ok);
}

template <>
inline quint64 convertParam(const QString &param, bool &ok)
{
    return param.toULongLong(&ok);
}

template <>
inline long long convertParam(const QString& param, bool& ok)
{
    return param.toLongLong(&ok);
}

template <>
inline double convertParam(const QString& param, bool& ok)
{
    return param.toDouble(&ok);
}

template <typename T>
T convertParam(const char* paramName, const QString& param, const std::string& errorString)
{
    bool ok = true;
    T res = convertParam<T>(param, ok);
    if (!ok) {
        throw ConvertationException(errorString);
    }
    return res;
}
