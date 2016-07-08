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

#include <QtGlobal>

#include "../fpml/fixed_point.h"
#include "../trikSound/types.h"

namespace trikSound {

template <typename T>
class RealTypeTraits
{
public:
    static qint16 toInt16(T val)
    {
        return (qint16) val;
    }

    static qint32 toInt32(T val)
    {
        return (qint32) val;
    }

    static float toFloat(T val)
    {
        return (float) val;
    }

    static double toDouble(T val)
    {
        return (double) val;
    }
};

template <>
class RealTypeTraits<fixed_point>
{
public:
    static qint16 toInt16(fixed_point val)
    {
        return val.to_short();
    }

    static qint32 toInt32(fixed_point val)
    {
        return val.to_int();
    }

    static float toFloat(fixed_point val)
    {
        return val.to_float();
    }

    static double toDouble(fixed_point val)
    {
        return val.to_double();
    }
};

}
