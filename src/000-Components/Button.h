#pragma once
#include <Wt/WPushButton.h>
#include "004-Theme/Theme.h"


class Button : public Wt::WPushButton
{
public:
    Button(const std::string& text = "", const std::string &style_classes = "text-xm",  PenguinUiWidgetTheme widget_theme = PenguinUiWidgetTheme::BtnPrimary);
private:
};