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
    auth_dialog_->setStyleClass("absolute top-0 left-0 right-0 bottom-0 w-screen h-screen");
    auth_dialog_->setMargin(WLength("-21em"), Side::Left); // .Wt-form width
    auth_dialog_->setMargin(WLength("-200px"), Side::Top); // ???
    auth_dialog_->contents()->setStyleClass("min-h-screen min-w-screen m-1 p-1 flex items-center justify-center");
    auth_widget_ = auth_dialog_->contents()->addWidget(std::make_unique<AuthWidget>(session_));
    // auto auth_widget = auth_dialog_->contents()->addWidget(std::make_unique<Wt::Auth::AuthWidget>(Session::auth(), session_.users(), session_.login()));
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
    if(app_root_ && app_root_->children().size() > 0) app_root_->clear();

    if(session_.login().loggedIn()) {
        Wt::Dbo::Transaction transaction(session_);
        
        // Query for STYLUS permission, taking first result if multiple exist
        Wt::Dbo::ptr<Permission> stylus_permission = session_.find<Permission>().where("name = ?").bind("STYLUS").resultValue();
        if(stylus_permission && session_.user()->hasPermission(stylus_permission)){
            wApp->log("info") << "Permission STYLUS found, Stylus will be available.";
            stylus_ = app_root_->addChild(std::make_unique<Stylus::Stylus>(session_));
        }else {
            wApp->log("error") << "Permission STYLUS not found, Stylus will not be available.";
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
