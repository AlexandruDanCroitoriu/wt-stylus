#pragma once
#include <Wt/WApplication.h>
#include "002-Dbo/Session.h"

// #include "005-TailwindConfigCenter/TailwindConfigCenter.h"
#include "101-Stylus/Stylus.h"

class App : public Wt::WApplication
{
public:
    App(const Wt::WEnvironment &env);

    // Session session_;
    Wt::Signal<bool> dark_mode_changed_;
    private:
    // TailwindConfigCenter* tailwind_config_center_;
    Stylus::Stylus* stylus_;

};