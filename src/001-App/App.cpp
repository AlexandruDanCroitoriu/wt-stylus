#include "App.h"
#include "010-TestWidgets/Test.h"
#include "010-TestWidgets/Navigation.h"

#include "004-Theme/DarkModeToggle.h"
#include "004-Theme/ThemeSwitcher.h"

#include "003-Auth/AuthWidget.h"
#include "004-Theme/Theme.h"

#include <Wt/WStackedWidget.h>

App::App(const Wt::WEnvironment &env)
    : WApplication(env),
    session_(appRoot() + "../dbo.db")
{
    // session_.configureAuth();
    session_.login().changed().connect(this, &App::authEvent);
    
    // Title
    setTitle("Alexandru Dan CV");
    
    require("https://cdn.jsdelivr.net/npm/@tailwindcss/browser@4");
    // require("https://unpkg.com/vue@3/dist/vue.global.prod.js");
    require("https://cdn.jsdelivr.net/npm/alpinejs@3.x.x/dist/cdn.min.js");
    
    root()->addStyleClass("max-w-screen max-h-screen overflow-none");

    // setTheme(std::make_shared<Wt::WCssTheme>("default"));
    setTheme(std::make_shared<Theme>());


    // override the default Wt auth templates
    messageResourceBundle().use("../static/stylus-resources/xml/003-Auth/ovrwt-registration-view");

    // override the default Wt templates
    messageResourceBundle().use("../static/stylus-resources/xml/001-App/ovrwt");
    messageResourceBundle().use("../static/stylus-resources/xml/000-examples/test");
    wApp->messageResourceBundle().use("../static/stylus-resources/xml/000-examples/override-wt");
    
    // root()->addWidget(std::make_unique<Test>());
    auto navbar = root()->addWidget(std::make_unique<Navigation>());
    auto stack = navbar->bindWidget("content", std::make_unique<Wt::WStackedWidget>());
    stack->setStyleClass("p-1");
    
    auto content = stack->addWidget(std::make_unique<Wt::WContainerWidget>());
    auto themeSwitcher = content->addWidget(std::make_unique<ThemeSwitcher>());
    auto darkModeToggle = content->addWidget(std::make_unique<DarkModeToggle>());
    
    stylus_ = root()->addChild(std::make_unique<Stylus::Stylus>(session_));
    auto authWidget = content->addWidget(std::make_unique<AuthWidget>(session_));
    authWidget->processEnvironment();
    
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