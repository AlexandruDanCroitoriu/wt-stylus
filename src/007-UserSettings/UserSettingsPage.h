#pragma once
#include <Wt/WContainerWidget.h>
#include "002-Dbo/Session.h"


class UserSettingsPage : public Wt::WContainerWidget
{
public:
    UserSettingsPage(Session& session);
private:
    Session& session_;
};