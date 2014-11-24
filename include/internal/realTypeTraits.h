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
};

#endif // REALTYPETRAITS_H
