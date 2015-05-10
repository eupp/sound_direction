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

private:

    bool mShowAngle;
    bool mShowVadCoef;
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
