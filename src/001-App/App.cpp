#include "App.h"
#include "010-TestWidgets/DarkModeToggle.h"
#include "010-TestWidgets/Test.h"
#include "010-TestWidgets/Navigation.h"
#include <Wt/WStackedWidget.h>

// #include <Wt/WBootstrap2Theme.h>
#include <Wt/WContainerWidget.h>

#include "003-Auth/AuthWidget.h"

App::App(const Wt::WEnvironment &env)
    : WApplication(env),
    session_(appRoot() + "../dbo.db")
{
    // session_.configureAuth();
    session_.login().changed().connect(this, &App::authEvent);
    // addMetaHeader("viewport", "width=device-width, initial-scale=1");
    
    // Title
    setTitle("Alexandru Dan CV");
    
    require("https://cdn.jsdelivr.net/npm/@tailwindcss/browser@4");
    // require("https://unpkg.com/vue@3/dist/vue.global.prod.js");
    require("https://cdn.jsdelivr.net/npm/alpinejs@3.x.x/dist/cdn.min.js");
    
    root()->addStyleClass("max-w-screen max-h-screen overflow-scroll");

    // setTheme(std::make_shared<Wt::WBootstrap2Theme>());

    // override the default Wt auth templates
    messageResourceBundle().use("../static/stylus-resources/xml/003-Auth/ovrwt-registration-view");

    // override the default Wt templates
    messageResourceBundle().use("../static/stylus-resources/xml/001-App/ovrwt");
    messageResourceBundle().use("../static/stylus-resources/xml/000-examples/test");
    wApp->messageResourceBundle().use("../static/stylus-resources/xml/000-examples/override-wt");
    
    // root()->addWidget(std::make_unique<Test>());
    auto navbar = root()->addWidget(std::make_unique<Navigation>());
    auto stack = navbar->bindWidget("content", std::make_unique<Wt::WStackedWidget>());
    auto authWidget = stack->addWidget(std::make_unique<AuthWidget>(session_));

    stylus_ = root()->addChild(std::make_unique<Stylus::Stylus>(session_));
    
    authWidget->processEnvironment();
    

    // root()->setStyleClass("block min-h-[1000vh] bg-red-200");
    
    // auto dark_mode_toggle = root()->addWidget(std::make_unique<DarkModeToggle>());
    // auto temp = root()->addWidget(std::make_unique<Wt::WTemplate>(Wt::WString::tr("examples:test-1")));
    // temp->setCondition("if:cond", true);

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