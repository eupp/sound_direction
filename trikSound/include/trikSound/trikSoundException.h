#pragma once

#include <exception>

#include <QString>

#include "triksound_global.h"

namespace trikSound {

class TRIKSOUNDSHARED_EXPORT TrikSoundException: public std::exception
{
public:
    TrikSoundException(const char* msg):
        mMsg(msg)
    {}

    virtual ~TrikSoundException() throw()
    {}

    const char* what() const throw()
    {
        return mMsg.toAscii().data();
    }

private:
    QString mMsg;
};

}
