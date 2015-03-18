#pragma once

#include "triksound_global.h"
#include "audioEvent.h"

namespace trikSound
{

class TRIKSOUNDSHARED_EXPORT IAudioEventListener
{
public:

    virtual ~IAudioEventListener() {}

    virtual void recieve(const AudioEvent& event) = 0;
};

}
