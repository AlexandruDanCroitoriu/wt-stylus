#include "App.h"
#include "010-TestWidgets/Test.h"
#include "010-TestWidgets/Navigation.h"

#include "004-Theme/DarkModeToggle.h"
#include "004-Theme/ThemeSwitcher.h"
#include "004-Theme/ThemeSwitcherV2.h"
#include "004-Theme/Theme.h"


#include "005-WidgetsDisplay/WidgetsDisplay.h"

#include <Wt/WStackedWidget.h>
#include <Wt/WPushButton.h>
#include <Wt/WMenu.h>
#include <Wt/WLabel.h>

App::App(const Wt::WEnvironment &env)
    : WApplication(env),
    session_(appRoot() + "../dbo.db")
{
    // session_.configureAuth();
    session_.login().changed().connect(this, &App::authEvent);

    // Title
    setTitle("Alexandru Dan CV");
    setHtmlClass("dark");
    
    require("https://cdn.jsdelivr.net/npm/@tailwindcss/browser@4");
    // require("https://unpkg.com/vue@3/dist/vue.global.prod.js");
    require("https://cdn.jsdelivr.net/npm/alpinejs@3.x.x/dist/cdn.min.js");
    
    root()->addStyleClass("max-w-screen max-h-screen overflow-none");

    wApp->messageResourceBundle().use("../static/stylus-resources/xml/PenguinUi/svg");
    
    // Auth 
    wApp->messageResourceBundle().use("../static/stylus-resources/xml/003-Auth/ovrwt-auth");
    wApp->messageResourceBundle().use("../static/stylus-resources/xml/003-Auth/ovrwt-auth-login");
    wApp->messageResourceBundle().use("../static/stylus-resources/xml/003-Auth/ovrwt-auth-strings");

    // override the default Wt auth templates
    wApp->messageResourceBundle().use("../static/stylus-resources/xml/003-Auth/ovrwt-registration-view");

    // override the default Wt templates
    wApp->messageResourceBundle().use("../static/stylus-resources/xml/001-App/ovrwt");
    wApp->messageResourceBundle().use("../static/stylus-resources/xml/000-examples/test");
    wApp->messageResourceBundle().use("../static/stylus-resources/xml/000-examples/override-wt");
    
    stylus_ = root()->addChild(std::make_unique<Stylus::Stylus>(session_));
    
    auto theme = std::make_shared<Theme>();
    theme->setPenguinUiConfig();
    setTheme(theme);
    
    auto theme_switcher = root()->addNew<ThemeSwitcherV2>(session_);
    theme_switcher->addStyleClass("fixed bottom-16 right-3");
    auto dark_mode_toggle = root()->addNew<DarkModeToggle>(session_);
    dark_mode_toggle->addStyleClass("fixed bottom-3 right-3");
    
    auto penguin_ui_page = std::make_unique<Wt::WContainerWidget>();
    auto themeSwitcher = penguin_ui_page->addWidget(std::make_unique<ThemeSwitcher>());
    auto widgetsDisplay = penguin_ui_page->addNew<WidgetsDisplay>();
    widgetsDisplay->createButtons();
    auto navbar = root()->addWidget(std::make_unique<Navigation>(session_));
    
    navbar->addPage("penguin ui", std::move(penguin_ui_page));
    
    navbar->auth_widget_->processEnvironment();
}

void App::authEvent() {
    if (session_.login().loggedIn()) {
        const Wt::Auth::User& u = session_.login().user();
        log("notice")
        << "User " << u.id()
        << " (" << u.identity(Wt::Auth::Identity::LoginName) << ")"
        << " logged in.";
    } else
        log("notice") << "User logged out.";
}
