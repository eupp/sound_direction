#pragma once

#include "triksound_global.h"
#include "audioEvent.h"

namespace trikSound
{

class TRIKSOUNDSHARED_EXPORT AudioEventListener
{
public:

    ~AudioEventListener() {}

    virtual void recieve(const AudioEvent& event) = 0;
};

}
