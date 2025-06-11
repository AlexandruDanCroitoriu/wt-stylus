#pragma once
#include <Wt/WApplication.h>
#include "002-Dbo/Session.h"

#include "101-Stylus/Stylus.h"

class App : public Wt::WApplication
{
public:
    App(const Wt::WEnvironment &env);

    Wt::Signal<bool> dark_mode_changed_;
private:
    Session session_;
    Stylus::Stylus* stylus_;
    void authEvent();

};