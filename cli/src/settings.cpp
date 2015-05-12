#include "settings.h"

ViewSettings::ViewSettings():
    mShowAngle(true)
  , mShowVadCoef(false)
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

