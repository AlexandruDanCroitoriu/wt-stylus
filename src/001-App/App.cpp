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
 
    // messageResourceBundle().use("../static/stylus-resources/xml/stylus/override-wt");
    // messageResourceBundle().use("../static/stylus-resources/xml/stylus/stylus");

    // require("https://cdn.jsdelivr.net/npm/@tailwindcss/browser@4");
    // useStyleSheet("static/css/questionmark.css");

    // Title
    setTitle("Starter App");
    // root()->setStyleClass("block min-h-[1000vh] bg-red-200");
  
    auto dark_mode_toggle = root()->addWidget(std::make_unique<DarkModeToggle>());


    
auto container = root()->addWidget(std::make_unique<Wt::WContainerWidget>());
Wt::WTree *tree = container->addWidget(std::make_unique<Wt::WTree>());

tree->setSelectionMode(Wt::SelectionMode::Extended);

auto folderIcon
    = std::make_unique<Wt::WIconPair>("static/stylus/yellow-folder-closed.png",
                        "static/stylus/yellow-folder-open.png", false);

auto node =
    std::make_unique<Wt::WTreeNode>("Furniture",std::move(folderIcon));
auto furnitureNode = node.get();
tree->setTreeRoot(std::move(node));


tree->treeRoot()->label()->setTextFormat(Wt::TextFormat::Plain);
tree->treeRoot()->setLoadPolicy(Wt::ContentLoading::NextLevel);
tree->treeRoot()->addChildNode(std::make_unique<Wt::WTreeNode>("Table"));
tree->treeRoot()->addChildNode(std::make_unique<Wt::WTreeNode>("Cupboard"));

auto subtree = std::make_unique<Wt::WTreeNode>("Chair");
auto subtree_ = tree->treeRoot()->addChildNode(std::move(subtree));
tree->treeRoot()->addChildNode(std::make_unique<Wt::WTreeNode>("Coach"));
tree->treeRoot()->expand();

subtree_->addChildNode(std::make_unique<Wt::WTreeNode>("Doc"));
subtree_->addChildNode(std::make_unique<Wt::WTreeNode>("Grumpy"));
subtree_->addChildNode(std::make_unique<Wt::WTreeNode>("Happy"));
subtree_->addChildNode(std::make_unique<Wt::WTreeNode>("Sneezy"));
subtree_->addChildNode(std::make_unique<Wt::WTreeNode>("Dopey"));
subtree_->addChildNode(std::make_unique<Wt::WTreeNode>("Bashful"));
subtree_->addChildNode(std::make_unique<Wt::WTreeNode>("Sleepy"));

auto imageButton = container->addWidget(std::make_unique<Wt::WPushButton>("Use Image Icons"));
imageButton->clicked().connect([=](){
    auto icon
    = std::make_unique<Wt::WIconPair>("icons/yellow-folder-closed.png",
                        "icons/yellow-folder-open.png", false);
    furnitureNode->setLabelIcon(std::move(icon));
});

auto FAButton = container->addWidget(std::make_unique<Wt::WPushButton>("Use Font-Awesome Icons"));
FAButton->clicked().connect([=](){
    auto icon
    = std::make_unique<Wt::WIconPair>("folder",
                        "folder-open", false);
    icon->setIconsType(Wt::WIconPair::IconType::IconName);
    furnitureNode->setLabelIcon(std::move(icon));
});

}
