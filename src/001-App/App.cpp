#include "App.h"
#include "010-TestWidgets/DarkModeToggle.h"
#include "010-TestWidgets/Test.h"


#include <Wt/WContainerWidget.h>
#include <Wt/WIconPair.h>
#include <Wt/WPushButton.h>
#include <Wt/WText.h>
#include <Wt/WTree.h>
#include <Wt/WTreeNode.h>

App::App(const Wt::WEnvironment &env)
    : WApplication(env)
//   session_(appRoot() + "../dbo.db"),
{
    stylus_ = root()->addChild(std::make_unique<Stylus::Stylus>());
 
    messageResourceBundle().use("../static/stylus-resources/xml/000-examples/test");
    // messageResourceBundle().use("../static/stylus-resources/xml/stylus/stylus");

    // require("https://cdn.jsdelivr.net/npm/@tailwindcss/browser@4");
    // useStyleSheet("static/css/questionmark.css");

    // Title
    setTitle("Starter App");
    // root()->setStyleClass("block min-h-[1000vh] bg-red-200");
  
    auto dark_mode_toggle = root()->addWidget(std::make_unique<DarkModeToggle>());


    auto temp = root()->addWidget(std::make_unique<Wt::WTemplate>(Wt::WString::tr("examples:test-1")));
    temp->setCondition("if:cond", true);

}
