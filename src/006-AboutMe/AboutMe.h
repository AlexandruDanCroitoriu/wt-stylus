#pragma once
#include <Wt/WContainerWidget.h>
#include "002-Dbo/Session.h"
#include <Wt/WStackedWidget.h>

class AboutMe : public Wt::WContainerWidget
{
public:
    AboutMe();
private:
    Wt::WStackedWidget* stack_;
    std::vector<std::string> data_already_added_to_stack_;
    std::string base_url_ = "https://swapi.info/api/";

    void setView(std::string url);
};