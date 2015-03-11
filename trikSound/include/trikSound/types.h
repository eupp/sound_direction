#pragma once

#include <QtGlobal>

#include "../fpml/fixed_point.h"

namespace trikSound {

typedef std::int16_t sample_type;
typedef fpml::fixed_point<std::int32_t, 16> fixed_point;
typedef fixed_point real_t;
//typedef float real_t;

}
