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

#include "settings.h"

ViewSettings::ViewSettings():
    mShowAngle(false)
  , mShowVadCoef(false)
  , mDiffTime(0)
{}

bool ViewSettings::showAngle() const
{
    return mShowAngle;
}

void ViewSettings::setShowAngle(bool showAngle)
{
    mShowAngle = showAngle;
}

bool ViewSettings::showVadCoef() const
{
    return mShowVadCoef;
}

void ViewSettings::setShowVadCoef(bool showVadCoef)
{
    mShowVadCoef = showVadCoef;
}
int ViewSettings::diffTime() const
{
    return mDiffTime;
}

void ViewSettings::setDiffTime(int diffTime)
{
    mDiffTime = diffTime;
}


Settings::Settings(const ControllerSettings& cSettings, const ViewSettings& vSettings):
    mControllerSettings(cSettings)
  , mViewSettings(vSettings)
{}

ControllerSettings Settings::controllerSettings() const
{
    return mControllerSettings;
}

void Settings::setControllerSettings(const ControllerSettings& controllerSettings)
{
    mControllerSettings = controllerSettings;
}

ViewSettings Settings::viewSettings() const
{
    return mViewSettings;
}

void Settings::setViewSettings(const ViewSettings& viewSettings)
{
    mViewSettings = viewSettings;
}

