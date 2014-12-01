#ifndef TYPES_H
#define TYPES_H

#include <QtGlobal>

#include "include/fpml/fixed_point.h"

typedef qint16 sample_t;
typedef fpml::fixed_point<std::int32_t, 16> fixed_point;
typedef fixed_point real_t;
//typedef float real_t;

#endif // TYPES_H
