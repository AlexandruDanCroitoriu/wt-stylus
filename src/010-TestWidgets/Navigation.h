#pragma once
#include <Wt/WContainerWidget.h>
#include <Wt/WTemplate.h>
#include <Wt/WStackedWidget.h>
#include <Wt/WMenu.h>
#include <Wt/WMenuItem.h>
#include <vector>

#include "002-Dbo/Session.h"
#include "003-Auth/AuthWidget.h"

class Navigation : public Wt::WTemplate
{
    public:
        Navigation(Session& session);

        void addPage(const std::string &name, std::unique_ptr<Wt::WContainerWidget> page_widget, const std::string &icon_xml_id = "");
        Wt::WWidget* getPageWrapper(std::string name);
        
        AuthWidget* auth_widget_;
        private:
        
        Wt::WStackedWidget *stacked_widget_;
        Wt::WMenu *menu_;
        Session& session_;
        Wt::WDialog* auth_dialog_;
};
