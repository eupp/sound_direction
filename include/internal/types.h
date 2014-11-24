#ifndef TYPES_H
#define TYPES_H

#include <cstdint>

#include "include/fpml/fixed_point.h"

typedef std::int16_t sample_t;
typedef fpml::fixed_point<std::int32_t, 16> real_t;
//typedef float real_t;

#endif // TYPES_H
