#include "010-TestWidgets/Navigation.h"
#include <Wt/WText.h>
#include <Wt/WApplication.h>
#include <Wt/WPushButton.h>
#include <Wt/WApplication.h>
#include <Wt/WAnchor.h>
#include <Wt/WLink.h>
#include "000-Components/Button.h"

// : Wt::WTemplate(Wt::WString::tr("test-alpine-nav"))
Navigation::Navigation(Session& session)
    : Wt::WTemplate(Wt::WString::tr("penguin-ui-application-shell")), 
    session_(session)
{   
    stacked_widget_ = bindWidget("content", std::make_unique<Wt::WStackedWidget>());
    menu_ = bindWidget("menu", std::make_unique<Wt::WMenu>(stacked_widget_));
    menu_->setInternalPathEnabled("/");
    menu_->setStyleClass("sidebar-nav-menu");
    
    setCondition("user-logged-in", false);
    setCondition("user-logged-out", true);
    auto login_btn = bindWidget("login-link", std::make_unique<Button>("Login", "text-sm w-full", PenguinUiWidgetTheme::BtnPrimary));
    bindString("user-name", "user name");
    
    auth_dialog_ = wApp->root()->addNew<Wt::WDialog>(Wt::WString::tr("Wt.Auth.login-form-title"));
    auth_dialog_->setClosable(false);
    auth_dialog_->setModal(true);
    auth_dialog_->escapePressed().connect([=]() {
        auth_dialog_->hide();
    });
    // auth_dialog_->titleBar()->removeFromParent();
    login_btn->clicked().connect([=]() {
        auth_dialog_->show();
    });

    auth_widget_ = auth_dialog_->contents()->addWidget(std::make_unique<AuthWidget>(session_));

    session_.login().changed().connect([=]() {
        std::cout << "\n\n\n\n ------------------Login changed: " << session_.login().loggedIn() << "\n\n";
        if (session_.login().loggedIn()) {
            setCondition("user-logged-in", true);
            setCondition("user-logged-out", false);
            bindString("user-name", session_.login().user().identity(Wt::Auth::Identity::LoginName));
            bindString("user-image-url", "static/stylus/empty-user.svg");
            if(auth_dialog_->isVisible()) {
                auth_dialog_->hide();
            }
            std::cout << "\n\n\n\n ------------------User logged in: " << session_.login().user().identity(Wt::Auth::Identity::LoginName) << "\n\n";
        }else {
            setCondition("user-logged-in", false);
            setCondition("user-logged-out", true);
            
            std::cout << "\n\n\n\n ------------------User logged out\n\n";
        }
    });

    wApp->internalPathChanged().connect([=](const std::string &path) {
        std::cout << "\n\nInternal path changed to: " << path << "\n\n";
        // if (path.empty() || path == "/") {
        //     menu_->select(0); // Select the first item by default
        // } else if (path == "/penguin-ui") {
        //     menu_->select(1); // Select the second item

        // }
    });
}


void Navigation::addPage(const std::string &name, std::unique_ptr<Wt::WContainerWidget> page_widget, const std::string &icon_xml_id)
{
    auto menu_item = menu_->addItem(name, std::move(page_widget));
    auto svg_temp = menu_item->anchor()->insertNew<Wt::WTemplate>(0, Wt::WString::tr(icon_xml_id));
    svg_temp->setStyleClass("");
    menu_item->anchor()->addStyleClass("block flex items-center space-x-2");
}

Wt::WWidget* Navigation::getPageWrapper(std::string name)
{
    for (auto item : menu_->items()) {
        if (item->text() == name) {
            return item->contents();
        }
    }
    return nullptr;
};

