#include "010-TestWidgets/Navigation.h"
#include <Wt/WText.h>
#include <Wt/WApplication.h>
#include <Wt/WPushButton.h>
#include <Wt/WApplication.h>
#include <Wt/WAnchor.h>
#include <Wt/WLink.h>

#include "001-App/App.h"
#include "000-Components/Button.h"
#include "007-UserSettings/UserSettingsPage.h"

#include <regex>
// #include <Wt/Http/Cookie.h>

Navigation::Navigation(Session& session)
    : Wt::WTemplate(Wt::WString::tr("app-shell-v1")), 
    session_(session)
{   
    stacked_widget_ = bindWidget("content", std::make_unique<Wt::WStackedWidget>());
    stacked_widget_->setStyleClass("container mx-auto");
    menu_ = bindWidget("menu", std::make_unique<Wt::WMenu>(stacked_widget_));
    menu_->setInternalPathEnabled("/");
    menu_->setInternalBasePath("/");
    menu_->setStyleClass("sidebar-nav-menu");


      if (session_.login().loggedIn()) {
        user_menu_temp_ = bindWidget("user-menu", std::make_unique<Wt::WTemplate>(Wt::WString::tr("app-shell-sidebar-user-v1")));
        user_menu_temp_->bindString("user-name", session_.login().user().identity(Wt::Auth::Identity::LoginName));
        user_menu_temp_->bindString("user-image-url", "static/stylus/empty-user.svg");
        // if(auth_dialog_->isVisible()) {
        //     auth_dialog_->hide();
        // }
        user_menu_temp_->clicked().connect([=]() {
            showUserPopupMenu();
        });
        if (!popup_menu_) {
            popup_menu_ = std::make_unique<Wt::WPopupMenu>(stacked_widget_);
            popup_menu_->setHideOnSelect(true);
            popup_menu_->setInternalBasePath("/user");
            popup_menu_->setInternalPathEnabled("/user");
            // popup_menu_->setStyleClass("border divide-y divide-outline border-outline bg-surface dark:divide-outline-dark dark:border-outline-dark dark:bg-surface-dark rounded-radius shadow-2xl");
            popup_menu_->setStyleClass("stylus-background border divide-y divide-outline border-outline dark:divide-outline-dark dark:border-outline-dark rounded-radius shadow-2xl");
            auto user_profile_menu_item = popup_menu_->addItem("Settings", std::make_unique<UserSettingsPage>(session_));
            user_profile_menu_item->anchor()->insertNew<Wt::WTemplate>(0, Wt::WString::tr("app:settings-svg"));
            user_profile_menu_item->addStyleClass(menu_item_styles_);
            user_profile_menu_item->anchor()->addStyleClass(menu_item_anchor_styles_ + " px-4 py-2");
            
            popup_menu_->addSeparator();
            auto logout_menu_item = popup_menu_->addItem("Logout");
            logout_menu_item->setInternalPathEnabled(false);
            logout_menu_item->anchor()->insertNew<Wt::WTemplate>(0, Wt::WString::tr("app:logout-svg"));
            logout_menu_item->addStyleClass(menu_item_styles_);
            logout_menu_item->anchor()->addStyleClass(menu_item_anchor_styles_ + " px-4 py-2");

            logout_menu_item->clicked().connect([=]() {
                session_.login().logout();
            });
        }
    }else {
        auto login_btn = bindWidget("user-menu", std::make_unique<Button>("Login", "text-sm w-full", PenguinUiWidgetTheme::BtnPrimary));
        login_btn->clicked().connect([=]() {
            dynamic_cast<App*>(wApp)->auth_dialog_->show();
        });
    }
}

void Navigation::addPage(const std::string &name, std::unique_ptr<Wt::WContainerWidget> page_widget, const std::string &icon_xml_id)
{
    auto menu_item = menu_->addItem(name, std::move(page_widget));
    auto svg_temp = menu_item->anchor()->insertNew<Wt::WTemplate>(0, Wt::WString::tr(icon_xml_id));
    svg_temp->setStyleClass("");
    menu_item->addStyleClass(menu_item_styles_);
    menu_item->anchor()->addStyleClass(menu_item_anchor_styles_);
}

void Navigation::showUserPopupMenu()
{
    // std::cout << "\n\n\n\n ------------------Navigation::showUserPopupMenu called\n\n";
    if(!session_.login().loggedIn())
        return;

    if (popup_menu_->isHidden()) {
        popup_menu_->popup(user_menu_temp_, Wt::Orientation::Vertical);
    } else {
        popup_menu_->hide();
    }
}
