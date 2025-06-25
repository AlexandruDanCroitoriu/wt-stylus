#include "007-UserSettings/UserSettingsPage.h"
#include <Wt/WText.h>

UserSettingsPage::UserSettingsPage(Session& session)
    : Wt::WContainerWidget(), 
    session_(session)
{
    addNew<Wt::WText>("user settings");
}