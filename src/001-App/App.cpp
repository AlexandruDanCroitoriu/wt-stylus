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

#include "003-Auth/AuthWidget.h"
#include <Wt/Auth/PasswordService.h>

App::App(const Wt::WEnvironment &env)
    : WApplication(env),
    session_(appRoot() + "../dbo.db")
{
    session_.configureAuth();
    session_.login().changed().connect(this, &App::authEvent);

    // Title
    setTitle("Alexandru Dan CV");
    
    root()->addStyleClass("container");
    // setTheme(std::make_shared<Wt::WBootstrap2Theme>());

    // override the default Wt auth templates
    messageResourceBundle().use("../static/stylus-resources/xml/003-Auth/ovrwt-registration-view");
    messageResourceBundle().use("../static/stylus-resources/xml/003-Auth/ovrwt-auth");
    messageResourceBundle().use("../static/stylus-resources/xml/003-Auth/ovrwt-auth-strings");
    // override the default Wt templates
    messageResourceBundle().use("../static/stylus-resources/xml/001-App/ovrwt");

    messageResourceBundle().use("../static/stylus-resources/xml/000-examples/test");

    auto authWidget = root()->addWidget(std::make_unique<AuthWidget>(session_));

    
    authWidget->model()->addPasswordAuth(&Session::passwordAuth());
    authWidget->model()->addOAuth(Session::oAuth());
    authWidget->setRegistrationEnabled(true);

    stylus_ = root()->addChild(std::make_unique<Stylus::Stylus>(session_));
    
    authWidget->processEnvironment();
 
    // messageResourceBundle().use("../static/stylus-resources/xml/stylus/stylus");

    // require("https://cdn.jsdelivr.net/npm/@tailwindcss/browser@4");
    // useStyleSheet("static/css/questionmark.css");
    
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