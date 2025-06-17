#include "000-Components/Button.h"
#include <Wt/WApplication.h>


Button::Button(int WidgetElementRole)
    : Wt::WPushButton()
{
    wApp->theme()->apply(this, , WidgetElementRole);
}