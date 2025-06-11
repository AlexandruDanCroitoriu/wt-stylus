#pragma once
#include <Wt/WContainerWidget.h>
#include <Wt/WCheckBox.h>
#include <Wt/WSignal.h>


class DarkModeToggle : public Wt::WContainerWidget
{
    public:
        DarkModeToggle(bool dark = true);
        void setDarkMode(bool dark);
        Wt::Signal<bool> dark_mode_changed_;
    private:
        Wt::WCheckBox *checkBox_;
};