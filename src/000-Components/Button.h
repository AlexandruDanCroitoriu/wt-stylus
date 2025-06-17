#pragma once
#include <Wt/WPushButton.h>
#include "004-Theme/Theme.h"


class Button : public Wt::WPushButton
{
public:
    Button(int WidgetElementRole = WidgetElementRole::ButtonPrimary);
private:
};