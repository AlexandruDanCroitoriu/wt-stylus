#include "App.h"
#include "010-TestWidgets/Test.h"
#include "010-TestWidgets/Navigation.h"

#include "004-Theme/DarkModeToggle.h"
#include "004-Theme/ThemeSwitcher.h"

#include "005-WidgetsDisplay/WidgetsDisplay.h"
#include "006-AboutMe/AboutMe.h"

#include <Wt/WStackedWidget.h>
#include <Wt/WPushButton.h>
#include <Wt/WMenu.h>
#include <Wt/WLabel.h>
#include <Wt/WTheme.h>
#include "101-Stylus/000-Utils/StylusState.h"

App::App(const Wt::WEnvironment &env)
    : WApplication(env),
    session_(appRoot() + "../dbo.db")
{

    std::cout << "\n\n ------------------------ App::App() - Application started ------------------------ \n\n";
    // Title
    setTitle("Alexandru Dan CV");
    setHtmlClass("dark");
    
    require("https://cdn.jsdelivr.net/npm/@tailwindcss/browser@4");
    // require("https://unpkg.com/vue@3/dist/vue.global.prod.js");
    require("https://cdn.jsdelivr.net/npm/alpinejs@3.x.x/dist/cdn.min.js");
    
    root()->addStyleClass("max-w-screen max-h-screen overflow-none");


    {// message resource bundle
        wApp->messageResourceBundle().use("../../static/stylus-resources/xml/PenguinUi/svg");
        
        // Auth 
        wApp->messageResourceBundle().use("../../static/stylus-resources/xml/003-Auth/ovrwt-auth");
        wApp->messageResourceBundle().use("../../static/stylus-resources/xml/003-Auth/ovrwt-auth-login");
        wApp->messageResourceBundle().use("../../static/stylus-resources/xml/003-Auth/ovrwt-auth-strings");

        // override the default Wt auth templates
        wApp->messageResourceBundle().use("../../static/stylus-resources/xml/003-Auth/ovrwt-registration-view");

        // override the default Wt templates
        wApp->messageResourceBundle().use("../../static/stylus-resources/xml/001-App/main");
        wApp->messageResourceBundle().use("../../static/stylus-resources/xml/001-App/ovrwt");
        wApp->messageResourceBundle().use("../../static/stylus-resources/xml/001-App/svg");

        wApp->messageResourceBundle().use("../../static/stylus-resources/xml/000-examples/test");
        wApp->messageResourceBundle().use("../../static/stylus-resources/xml/000-examples/override-wt");
    }
    

    auto theme = std::make_shared<Theme>(session_, ThemeConfig::Arctic);
    theme->setPenguinUiConfig();
    setTheme(theme);

    // stylus_ = root()->addChild(std::make_unique<Stylus::Stylus>(session_));
    
    auth_dialog_ = wApp->root()->addNew<Wt::WDialog>("");
    auth_dialog_->titleBar()->removeFromParent();
    auth_dialog_->setClosable(false);
    auth_dialog_->setModal(true);
    auth_dialog_->escapePressed().connect([=]() { auth_dialog_->hide(); });
    auth_dialog_->setMinimumSize(Wt::WLength(100, Wt::LengthUnit::ViewportWidth), Wt::WLength(100, Wt::LengthUnit::ViewportHeight));
    auth_dialog_->contents()->setStyleClass("flex items-center justify-center");
    auth_widget_ = auth_dialog_->contents()->addWidget(std::make_unique<AuthWidget>(session_));
    
    app_root_ = root()->addNew<Wt::WContainerWidget>();
    
    session_.login().changed().connect(this, &App::authEvent);
    auth_widget_->processEnvironment();
    if(!session_.login().loggedIn()) {
        session_.login().changed().emit();
    }

    std::cout << "\n\n ------------------------ App::App() - Application instantiated ------------------------ \n\n";
}

void App::authEvent() {
    if (session_.login().loggedIn()) {
        const Wt::Auth::User& u = session_.login().user();
        log("notice")
        << "User " << u.id()
        << " (" << u.identity(Wt::Auth::Identity::LoginName) << ")"
        << " logged in.";
        if(auth_dialog_->isVisible()) {
            auth_dialog_->hide();
        }
        createApp();
    } else {
        log("notice") << "User logged out.";
        createApp();
    }
}

void App::createApp()
{
    app_root_->clear();
    if(stylus_) stylus_->removeFromParent();

    if(session_.login().loggedIn()) {
        Wt::Dbo::Transaction transaction(session_);
        
        // Query for STYLUS permission, taking first result if multiple exist
        Wt::Dbo::ptr<Permission> stylus_permission;
        auto query = session_.find<Permission>().where("name = ?").bind("STYLUS");
        auto results = query.resultList();
        
        if(results.empty()) {
            Stylus::StylusState::logMessage("Permission 'STYLUS' not found (creating it now) | in App::createApp()", Stylus::LogMessageType::Error);
            stylus_permission = session_.add(std::make_unique<Permission>("STYLUS"));
        } else {
            stylus_permission = results.front();
            if(results.size() > 1) {
                Stylus::StylusState::logMessage("Multiple 'STYLUS' permissions found, using first one | in App::createApp()", Stylus::LogMessageType::Warning);
            }
        }
        
        if(session_.user()->hasPermission(stylus_permission)){
            stylus_ = app_root_->addChild(std::make_unique<Stylus::Stylus>(session_));
        }
        transaction.commit();
    }

    auto theme_switcher = app_root_->addNew<ThemeSwitcher>(session_);
    theme_switcher->addStyleClass("fixed bottom-16 right-3");
    auto dark_mode_toggle = app_root_->addNew<DarkModeToggle>(session_);
    dark_mode_toggle->addStyleClass("fixed bottom-3 right-3");

    auto navbar = app_root_->addNew<Navigation>(session_);
    
    auto penguin_ui_page = std::make_unique<Wt::WContainerWidget>();
    auto widgetsDisplay = penguin_ui_page->addNew<WidgetsDisplay>();
    widgetsDisplay->createButtons();
    
    navbar->addPage("Portofolio", std::make_unique<AboutMe>());
    navbar->addPage("UI Penguin", std::move(penguin_ui_page));




}
