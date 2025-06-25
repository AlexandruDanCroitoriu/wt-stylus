#pragma once
#include <Wt/WContainerWidget.h>
#include <Wt/WTemplate.h>
#include <Wt/WStackedWidget.h>
#include <Wt/WPopupMenu.h>
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
        
    private:
        // void setNavigation();
        void showUserPopupMenu();
        std::unique_ptr<Wt::WPopupMenu> popup_menu_;

        Wt::WTemplate *user_menu_temp_;
        Wt::WStackedWidget *stacked_widget_;
        Wt::WMenu *menu_;
        Session& session_;
        // std::string menu_item_styles_ = "flex items-center gap-2 text-base font-medium text-on-surface underline-offset-2 hover:bg-primary/5 hover:text-on-surface-strong focus-visible:underline focus:outline-hidden dark:text-on-surface-dark dark:hover:bg-primary-dark/5 dark:hover:text-on-surface-dark-strong";
        std::string menu_item_styles_ = "text-base font-medium text-on-surface-strong dark:text-on-surface-dark-strong";
        std::string menu_item_anchor_styles_ = "flex items-center flex-nowrap px-2 py-1.5 space-x-2 hover:bg-primary/10";

};
