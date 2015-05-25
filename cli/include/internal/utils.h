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
