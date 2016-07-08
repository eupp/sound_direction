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

#include "trikSound/settings.h"


typedef trikSound::Settings ControllerSettings;

class ViewSettings
{
public:

    ViewSettings();

    bool showAngle() const;
    void setShowAngle(bool showAngle);

    bool showVadCoef() const;
    void setShowVadCoef(bool showVadCoef);

    int diffTime() const;
    void setDiffTime(int diffTime);

private:

    bool mShowAngle;
    bool mShowVadCoef;

    int mDiffTime;
};


class Settings
{
public:

    Settings(const ControllerSettings& cSettings, const ViewSettings& vSettings);

    ControllerSettings controllerSettings() const;
    void setControllerSettings(const ControllerSettings& controllerSettings);

    ViewSettings viewSettings() const;
    void setViewSettings(const ViewSettings& viewSettings);

private:

    ControllerSettings  mControllerSettings;
    ViewSettings        mViewSettings;
};
