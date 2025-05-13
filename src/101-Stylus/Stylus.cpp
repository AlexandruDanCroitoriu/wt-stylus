#include "101-Stylus/Stylus.h"
#include <Wt/WStackedWidget.h>
#include <Wt/WPushButton.h>
#include <Wt/WTemplate.h>
#include <Wt/WApplication.h>
#include <Wt/WStackedWidget.h>
#include <fstream>
#include <Wt/WRandom.h>

namespace Stylus {


Stylus::Stylus()
    : state_(std::make_shared<StylusState>())
{
    
    setOffsets(0, Wt::Side::Top | Wt::Side::Bottom | Wt::Side::Left | Wt::Side::Right);
    titleBar()->children()[0]->removeFromParent();
    setStyleClass("!border-0 overflow-auto stylus-background");
    titleBar()->hide();
    titleBar()->setStyleClass("p-0 flex items-center overflow-x-visible h-[40px]");
    contents()->setStyleClass("h-[100vh] overflow-y-auto overflow-x-visible flex");
    setModal(false);
    setResizable(false);
    setMovable(false);
    
    Wt::WApplication::instance()->useStyleSheet(Wt::WApplication::instance()->docRoot() + "/static/stylus/stylus.css?v=" + Wt::WRandom::generateId());
    Wt::WApplication::instance()->require(Wt::WApplication::instance()->docRoot() + "/static/stylus/monaco-edditor.js");

    Wt::WApplication::instance()->messageResourceBundle().use(Wt::WApplication::instance()->docRoot() + "/static/stylus/templates");

    auto navbar = contents()->addWidget(std::make_unique<Wt::WContainerWidget>());
    auto content_wrapper = contents()->addWidget(std::make_unique<Wt::WStackedWidget>());
    // content_wrapper->setTransitionAnimation(Wt::WAnimation(Wt::AnimationEffect::Pop, Wt::TimingFunction::EaseInOut, 500)); // this line represents a bug in Wt probably :P

    navbar->setStyleClass("flex flex-col h-full border-r border-solid dark:border-[#FFF]/50 stylus-background");
    content_wrapper->setStyleClass("w-screen h-screen stylus-background");

    auto templates_menu_item = navbar->addWidget(std::make_unique<Wt::WTemplate>(Wt::WString::tr("stylus-svg-xml-logo")));
    auto css_menu_item = navbar->addWidget(std::make_unique<Wt::WTemplate>(Wt::WString::tr("stylus-svg-css-logo")));
    auto javascript_menu_item = navbar->addWidget(std::make_unique<Wt::WTemplate>(Wt::WString::tr("stylus-svg-javascript-logo")));
    auto tailwind_menu_item = navbar->addWidget(std::make_unique<Wt::WTemplate>(Wt::WString::tr("stylus-svg-tailwind-logo")));
    std::string nav_btns_styles = "w-[40px] p-[4px] m-[4px] cursor-pointer rounded-md flex items-center filesManager-menu";

    templates_menu_item->setStyleClass(nav_btns_styles);
    tailwind_menu_item->setStyleClass(nav_btns_styles);
    css_menu_item->setStyleClass(nav_btns_styles);
    javascript_menu_item->setStyleClass(nav_btns_styles);
    
    xml_files_manager_ = content_wrapper->addWidget(std::make_unique<XmlFilesManager>(state_));    
    css_files_manager_ = content_wrapper->addWidget(std::make_unique<CssFilesManager>(state_));
    js_files_manager_ = content_wrapper->addWidget(std::make_unique<JsFilesManager>(state_));
    tailwind_config_ = content_wrapper->addWidget(std::make_unique<TailwindConfigManager>(state_));


    templates_menu_item->clicked().connect(this, [=]() {
        templates_menu_item->toggleStyleClass("filesManager-menu-selected", true);
        tailwind_menu_item->toggleStyleClass("filesManager-menu-selected", false);
        css_menu_item->toggleStyleClass("filesManager-menu-selected", false);
        javascript_menu_item->toggleStyleClass("filesManager-menu-selected", false);
        content_wrapper->setCurrentWidget(xml_files_manager_);
        state_->stylus_node_->SetAttribute("selected-menu", "templates");
        state_->doc_.SaveFile(state_->state_file_path_.c_str());
    });

    tailwind_menu_item->clicked().connect(this, [=]() {
        templates_menu_item->toggleStyleClass("filesManager-menu-selected", false);
        tailwind_menu_item->toggleStyleClass("filesManager-menu-selected", true);
        css_menu_item->toggleStyleClass("filesManager-menu-selected", false);
        javascript_menu_item->toggleStyleClass("filesManager-menu-selected", false);
        content_wrapper->setCurrentWidget(tailwind_config_);
        state_->stylus_node_->SetAttribute("selected-menu", "tailwind");
        state_->doc_.SaveFile(state_->state_file_path_.c_str());
    });

    css_menu_item->clicked().connect(this, [=]() {
        templates_menu_item->toggleStyleClass("filesManager-menu-selected", false);
        tailwind_menu_item->toggleStyleClass("filesManager-menu-selected", false);
        css_menu_item->toggleStyleClass("filesManager-menu-selected", true);
        javascript_menu_item->toggleStyleClass("filesManager-menu-selected", false);
        content_wrapper->setCurrentWidget(css_files_manager_);
        state_->stylus_node_->SetAttribute("selected-menu", "css");
        state_->doc_.SaveFile(state_->state_file_path_.c_str());
    });

    javascript_menu_item->clicked().connect(this, [=]() {
        templates_menu_item->toggleStyleClass("filesManager-menu-selected", false);
        tailwind_menu_item->toggleStyleClass("filesManager-menu-selected", false);
        css_menu_item->toggleStyleClass("filesManager-menu-selected", false);
        javascript_menu_item->toggleStyleClass("filesManager-menu-selected", true);
        content_wrapper->setCurrentWidget(js_files_manager_);
        state_->stylus_node_->SetAttribute("selected-menu", "javascript");
        state_->doc_.SaveFile(state_->state_file_path_.c_str());
    });

    auto selected_menu = state_->stylus_node_->Attribute("selected-menu");
    if (std::strcmp(selected_menu, "templates") == 0)
        templates_menu_item->clicked().emit(Wt::WMouseEvent());
    else if (std::strcmp(selected_menu, "tailwind") == 0)
        tailwind_menu_item->clicked().emit(Wt::WMouseEvent());
    else if (std::strcmp(selected_menu, "css") == 0)
        css_menu_item->clicked().emit(Wt::WMouseEvent());
    else if (std::strcmp(selected_menu, "javascript") == 0)
        javascript_menu_item->clicked().emit(Wt::WMouseEvent());

    if(state_->stylus_node_->BoolAttribute("open"))
        show();
    else
        hide();

    if(state_->stylus_node_->BoolAttribute("navigation-bar-hidden"))
    {
        navbar->animateHide(Wt::WAnimation(Wt::AnimationEffect::SlideInFromLeft, Wt::TimingFunction::EaseInOut, 500));
    }
    if(state_->xml_node_->BoolAttribute("navigation-bar-hidden"))
    {
        xml_files_manager_->grid_layout_->itemAt(0)->widget()->animateHide(Wt::WAnimation(Wt::AnimationEffect::SlideInFromLeft, Wt::TimingFunction::EaseInOut, 500));
    }
    if(state_->css_node_->BoolAttribute("navigation-bar-hidden"))
    {
        css_files_manager_->grid_layout_->itemAt(0)->widget()->animateHide(Wt::WAnimation(Wt::AnimationEffect::SlideInFromLeft, Wt::TimingFunction::EaseInOut, 500));
    }
    if(state_->js_node_->BoolAttribute("navigation-bar-hidden"))
    {
        js_files_manager_->grid_layout_->itemAt(0)->widget()->animateHide(Wt::WAnimation(Wt::AnimationEffect::SlideInFromLeft, Wt::TimingFunction::EaseInOut, 500));
    }


    Wt::WApplication::instance()->globalKeyWentDown().connect([=](Wt::WKeyEvent e)
    { 
        if (e.modifiers().test(Wt::KeyboardModifier::Alt) && e.modifiers().test(Wt::KeyboardModifier::Shift) && e.key() == Wt::Key::Q){
            if(navbar->isHidden())
            {
                state_->stylus_node_->SetAttribute("navigation-bar-hidden", "false");
                navbar->animateShow(Wt::WAnimation(Wt::AnimationEffect::SlideInFromLeft, Wt::TimingFunction::EaseInOut, 500));
            }else
            {
                state_->stylus_node_->SetAttribute("navigation-bar-hidden", "true");
                navbar->animateHide(Wt::WAnimation(Wt::AnimationEffect::SlideInFromLeft, Wt::TimingFunction::EaseInOut, 500));
            }
            state_->doc_.SaveFile(state_->state_file_path_.c_str());
            return;
        }
        if (e.modifiers().test(Wt::KeyboardModifier::Alt))
        {
            if (e.key() == Wt::Key::Q)
            {
                if(isHidden()){
                    animateShow(Wt::WAnimation(Wt::AnimationEffect::Fade, Wt::TimingFunction::EaseInOut, 500));
                    content_wrapper->currentWidget()->refresh();
                    state_->stylus_node_->SetAttribute("open", "true");
                }else{
                    animateHide(Wt::WAnimation(Wt::AnimationEffect::Fade, Wt::TimingFunction::EaseInOut, 500));
                    state_->stylus_node_->SetAttribute("open", "false");
                }
                state_->doc_.SaveFile(state_->state_file_path_.c_str());
            }else if (e.key() == Wt::Key::Key_1){
                templates_menu_item->clicked().emit(Wt::WMouseEvent());
            }else if (e.key() == Wt::Key::Key_2){
                css_menu_item->clicked().emit(Wt::WMouseEvent());
            }else if (e.key() == Wt::Key::Key_3){
                javascript_menu_item->clicked().emit(Wt::WMouseEvent());
            }else if (e.key() == Wt::Key::Key_4){
                tailwind_menu_item->clicked().emit(Wt::WMouseEvent());
            }else if (e.key() == Wt::Key::A){
                if(content_wrapper->currentWidget() == xml_files_manager_)
                {
                    if(state_->xml_node_->BoolAttribute("navigation-bar-hidden"))
                    {
                        xml_files_manager_->grid_layout_->itemAt(0)->widget()->animateShow(Wt::WAnimation(Wt::AnimationEffect::SlideInFromLeft, Wt::TimingFunction::EaseInOut, 500));
                        state_->xml_node_->SetAttribute("navigation-bar-hidden", "false");
                    }else
                    {
                        xml_files_manager_->grid_layout_->itemAt(0)->widget()->animateHide(Wt::WAnimation(Wt::AnimationEffect::SlideInFromLeft, Wt::TimingFunction::EaseInOut, 500));
                        state_->xml_node_->SetAttribute("navigation-bar-hidden", "true");
                    }
                    state_->doc_.SaveFile(state_->state_file_path_.c_str());
                }else if(content_wrapper->currentWidget() == css_files_manager_)
                {
                    if(state_->css_node_->BoolAttribute("navigation-bar-hidden"))
                    {
                        css_files_manager_->grid_layout_->itemAt(0)->widget()->animateShow(Wt::WAnimation(Wt::AnimationEffect::SlideInFromLeft, Wt::TimingFunction::EaseInOut, 500));
                        state_->css_node_->SetAttribute("navigation-bar-hidden", "false");
                    }else
                    {
                        css_files_manager_->grid_layout_->itemAt(0)->widget()->animateHide(Wt::WAnimation(Wt::AnimationEffect::SlideInFromLeft, Wt::TimingFunction::EaseInOut, 500));
                        state_->css_node_->SetAttribute("navigation-bar-hidden", "true");
                    }
                    state_->doc_.SaveFile(state_->state_file_path_.c_str());
                }else if(content_wrapper->currentWidget() == js_files_manager_)
                {
                    if(state_->js_node_->BoolAttribute("navigation-bar-hidden"))
                    {
                        js_files_manager_->grid_layout_->itemAt(0)->widget()->animateShow(Wt::WAnimation(Wt::AnimationEffect::SlideInFromLeft, Wt::TimingFunction::EaseInOut, 500));
                        state_->js_node_->SetAttribute("navigation-bar-hidden", "false");
                    }else
                    {
                        js_files_manager_->grid_layout_->itemAt(0)->widget()->animateHide(Wt::WAnimation(Wt::AnimationEffect::SlideInFromLeft, Wt::TimingFunction::EaseInOut, 500));
                        state_->js_node_->SetAttribute("navigation-bar-hidden", "true");
                    }
                    state_->doc_.SaveFile(state_->state_file_path_.c_str());
                }

            }
        }
    });

    css_files_manager_->file_saved().connect(this, [=]()
    {
        tailwind_config_->generateCssFile();
    });
    xml_files_manager_->file_saved().connect(this, [=]()
    {
        tailwind_config_->generateCssFile();
    });

}

}
