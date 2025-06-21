#include "App.h"
#include "010-TestWidgets/Test.h"
#include "010-TestWidgets/Navigation.h"

#include "004-Theme/DarkModeToggle.h"
#include "004-Theme/DarkModeToggleV2.h"
#include "004-Theme/ThemeSwitcher.h"
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

    // setTheme(std::make_shared<Wt::WCssTheme>("default"));
    auto theme = std::make_shared<Theme>();
    wApp->messageResourceBundle().use("../static/stylus-resources/xml/PenguinUi/svg");
    
    theme->setPenguinUiConfig();
    setTheme(theme);


    // Auth 
    wApp->messageResourceBundle().use("../static/stylus-resources/xml/003-Auth/ovrwt-auth");
    wApp->messageResourceBundle().use("../static/stylus-resources/xml/003-Auth/ovrwt-auth-login");
    wApp->messageResourceBundle().use("../static/stylus-resources/xml/003-Auth/ovrwt-auth-strings");

    // override the default Wt auth templates
    messageResourceBundle().use("../static/stylus-resources/xml/003-Auth/ovrwt-registration-view");

    // override the default Wt templates
    messageResourceBundle().use("../static/stylus-resources/xml/001-App/ovrwt");
    messageResourceBundle().use("../static/stylus-resources/xml/000-examples/test");
    wApp->messageResourceBundle().use("../static/stylus-resources/xml/000-examples/override-wt");
    
    // root()->addWidget(std::make_unique<Test>());
    // auto stack = navbar->bindWidget("content", std::make_unique<Wt::WStackedWidget>());
    
    // std::unique_ptr<Wt::WStackedWidget> stack_ptr = std::make_unique<Wt::WStackedWidget>();

    // auto wrapper_menu = root()->addNew<Wt::WContainerWidget>();
    // auto anchor_1 = wrapper_menu->addNew<Wt::WAnchor>(Wt::WLink(Wt::LinkType::InternalPath, "/auth"), "auth");
    // auto anchor_2 = wrapper_menu->addNew<Wt::WAnchor>(Wt::WLink(Wt::LinkType::InternalPath, "/penguin-ui"), "penguin-ui");
    
    
    // auto stack = root()->addWidget(std::move(stack_ptr));
    // stack->setStyleClass("p-1");
    
    stylus_ = root()->addChild(std::make_unique<Stylus::Stylus>(session_));
    
    
    auto penguin_ui_page = std::make_unique<Wt::WContainerWidget>();
    auto themeSwitcher = penguin_ui_page->addWidget(std::make_unique<ThemeSwitcher>());
    auto darkModeToggle = penguin_ui_page->addWidget(std::make_unique<DarkModeToggle>());
    auto widgetsDisplay = penguin_ui_page->addNew<WidgetsDisplay>();
    widgetsDisplay->createButtons();
    
    // auto auth_page = std::make_unique<Wt::WContainerWidget>();
    // auto authWidget = auth_page->addWidget(std::make_unique<AuthWidget>(session_));
    // authWidget->processEnvironment();
    
    auto navbar = root()->addWidget(std::make_unique<Navigation>(session_));

    // navbar->addPage("auth", std::move(auth_page));
    navbar->addPage("penguin ui", std::move(penguin_ui_page));
  
    auto dark_mode_toggle_v2 = root()->addNew<DarkModeToggleV2>();
    dark_mode_toggle_v2->addStyleClass("fixed size-10 bottom-3 right-3 flex items-center justify-center z-20 rounded-full bg-primary p-2 text-on-primary dark:bg-primary-dark dark:text-on-primary-dark");
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
