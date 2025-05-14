#include "App.h"
#include "010-TestWidgets/DarkModeToggle.h"
#include "010-TestWidgets/Test.h"

App::App(const Wt::WEnvironment &env)
    : WApplication(env)
//   session_(appRoot() + "../dbo.db"),
{
    stylus_ = root()->addChild(std::make_unique<Stylus::Stylus>());
 
    messageResourceBundle().use("../static/stylus-resources/xml/stylus/override-wt");

    // require("https://cdn.jsdelivr.net/npm/@tailwindcss/browser@4");
    useStyleSheet("static/css/questionmark.css");

    // Title
    setTitle("Starter App");
    root()->setStyleClass("block min-h-[1000vh] bg-red-200");
  
    auto dark_mode_toggle = root()->addWidget(std::make_unique<DarkModeToggle>());

}
