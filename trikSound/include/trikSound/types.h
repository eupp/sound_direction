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

namespace trikSound {

typedef std::int16_t sample_type;
typedef fpml::fixed_point<std::int32_t, 16> fixed_point;
typedef fixed_point real_t;
//typedef float real_t;

typedef long long threshold_type;

}
