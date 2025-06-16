#pragma once
#include <Wt/WApplication.h>
#include "002-Dbo/Session.h"

#include "101-Stylus/Stylus.h"
#include "004-Theme/Theme.h"

class App : public Wt::WApplication
{
public:
    App(const Wt::WEnvironment &env);

    Wt::Signal<bool> dark_mode_changed_;
    Wt::Signal<> theme_changed_;
private:
    std::shared_ptr<Theme> theme_;
    Session session_;
    Stylus::Stylus* stylus_;
    void authEvent();

};