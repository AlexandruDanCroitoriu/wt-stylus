#include "App.h"
#include "010-TestWidgets/DarkModeToggle.h"
#include "010-TestWidgets/Test.h"


#include <Wt/WContainerWidget.h>
#include <Wt/WIconPair.h>
#include <Wt/WPushButton.h>
#include <Wt/WText.h>
#include <Wt/WTree.h>
#include <Wt/WTreeNode.h>


// #include <Wt/WBootstrap2Theme.h>
#include <Wt/WContainerWidget.h>
#include <Wt/WServer.h>

// #include <Wt/Auth/AuthWidget.h>
#include "003-Auth/AuthWidget.h"
#include <Wt/Auth/PasswordService.h>

App::App(const Wt::WEnvironment &env)
    : WApplication(env),
    session_(appRoot() + "../dbo.db")
{
    session_.configureAuth();
    session_.login().changed().connect(this, &App::authEvent);

    root()->addStyleClass("container");
    // setTheme(std::make_shared<Wt::WBootstrap2Theme>());


    auto authWidget = std::make_unique<AuthWidget>(session_);

    authWidget->model()->addPasswordAuth(&Session::passwordAuth());
    authWidget->model()->addOAuth(Session::oAuth());
    authWidget->setRegistrationEnabled(true);

    authWidget->processEnvironment();

    root()->addWidget(std::move(authWidget));

    stylus_ = root()->addChild(std::make_unique<Stylus::Stylus>());
 
    messageResourceBundle().use("../static/stylus-resources/xml/000-examples/test");
    messageResourceBundle().use("../static/stylus-resources/xml/003-Auth/templates");
    messageResourceBundle().use("../static/stylus-resources/xml/003-Auth/strings");
    // messageResourceBundle().use("../static/stylus-resources/xml/stylus/stylus");

    // require("https://cdn.jsdelivr.net/npm/@tailwindcss/browser@4");
    // useStyleSheet("static/css/questionmark.css");

    // Title
    setTitle("Alexandru Dan CV");
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