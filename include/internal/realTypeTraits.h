#ifndef REALTYPETRAITS_H
#define REALTYPETRAITS_H

#include <QtGlobal>

#include "include/fpml/fixed_point.h"
#include "include/internal/types.h"

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

#endif // REALTYPETRAITS_H
