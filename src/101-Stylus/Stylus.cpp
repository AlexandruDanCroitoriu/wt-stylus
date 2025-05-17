#include "101-Stylus/Stylus.h"
#include <Wt/WStackedWidget.h>
#include <Wt/WPushButton.h>
#include <Wt/WTemplate.h>
#include <Wt/WApplication.h>
#include <Wt/WStackedWidget.h>
#include <fstream>
#include <Wt/WRandom.h>
#include "010-TestWidgets/DarkModeToggle.h"
#include "001-App/App.h"

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
    
    Wt::WApplication::instance()->doJavaScript(R"(
        document.addEventListener('keydown', function(event) {
            if (event.altKey && (event.key === 'ArrowLeft' || event.key === 'ArrowRight')) {
                event.preventDefault();
                // Your custom logic here if needed
            }
        });
    )");


    // Wt::WApplication::instance()->useStyleSheet(Wt::WApplication::instance()->docRoot() + "/static/stylus/stylus.css?v=" + Wt::WRandom::generateId());
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
    auto dark_mode_toggle = navbar->addWidget(std::make_unique<DarkModeToggle>());

    std::string nav_btns_styles = "w-[40px] p-[4px] m-[4px] cursor-pointer rounded-md flex items-center stylus-menu";

    templates_menu_item->setStyleClass(nav_btns_styles);
    tailwind_menu_item->setStyleClass(nav_btns_styles);
    css_menu_item->setStyleClass(nav_btns_styles);
    javascript_menu_item->setStyleClass(nav_btns_styles);
    
    xml_files_manager_ = content_wrapper->addWidget(std::make_unique<XmlFilesManager>(state_));    
    css_files_manager_ = content_wrapper->addWidget(std::make_unique<CssFilesManager>(state_));
    js_files_manager_ = content_wrapper->addWidget(std::make_unique<JsFilesManager>(state_));
    tailwind_config_ = content_wrapper->addWidget(std::make_unique<TailwindConfigManager>(state_));


    templates_menu_item->clicked().connect(this, [=]() {
        templates_menu_item->toggleStyleClass("stylus-menu-selected", true);
        tailwind_menu_item->toggleStyleClass("stylus-menu-selected", false);
        css_menu_item->toggleStyleClass("stylus-menu-selected", false);
        javascript_menu_item->toggleStyleClass("stylus-menu-selected", false);
        content_wrapper->setCurrentWidget(xml_files_manager_);
        state_->stylus_node_->SetAttribute("selected-menu", "templates");
        state_->doc_->SaveFile(state_->state_file_path_.c_str());
    });

    tailwind_menu_item->clicked().connect(this, [=]() {
        templates_menu_item->toggleStyleClass("stylus-menu-selected", false);
        tailwind_menu_item->toggleStyleClass("stylus-menu-selected", true);
        css_menu_item->toggleStyleClass("stylus-menu-selected", false);
        javascript_menu_item->toggleStyleClass("stylus-menu-selected", false);
        content_wrapper->setCurrentWidget(tailwind_config_);
        state_->stylus_node_->SetAttribute("selected-menu", "tailwind");
        state_->doc_->SaveFile(state_->state_file_path_.c_str());
    });

    css_menu_item->clicked().connect(this, [=]() {
        templates_menu_item->toggleStyleClass("stylus-menu-selected", false);
        tailwind_menu_item->toggleStyleClass("stylus-menu-selected", false);
        css_menu_item->toggleStyleClass("stylus-menu-selected", true);
        javascript_menu_item->toggleStyleClass("stylus-menu-selected", false);
        content_wrapper->setCurrentWidget(css_files_manager_);
        state_->stylus_node_->SetAttribute("selected-menu", "css");
        state_->doc_->SaveFile(state_->state_file_path_.c_str());
    });

    javascript_menu_item->clicked().connect(this, [=]() {
        templates_menu_item->toggleStyleClass("stylus-menu-selected", false);
        tailwind_menu_item->toggleStyleClass("stylus-menu-selected", false);
        css_menu_item->toggleStyleClass("stylus-menu-selected", false);
        javascript_menu_item->toggleStyleClass("stylus-menu-selected", true);
        content_wrapper->setCurrentWidget(js_files_manager_);
        state_->stylus_node_->SetAttribute("selected-menu", "javascript");
        state_->doc_->SaveFile(state_->state_file_path_.c_str());
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

    dark_mode_toggle->dark_mode_changed_.connect(this, [=](bool dark)
    {
        xml_files_manager_->editor_->setDarkTheme(dark);
        state_->stylus_node_->SetAttribute("dark-mode", dark ? "true" : "false");
        state_->doc_->SaveFile(state_->state_file_path_.c_str());
    });

    if(state_->stylus_node_->BoolAttribute("dark-mode")){
        dynamic_cast<App*>(Wt::WApplication::instance())->dark_mode_changed_.emit(true);
        xml_files_manager_->editor_->setDarkTheme(true);
    }

    Wt::WApplication::instance()->globalKeyWentDown().connect([=](Wt::WKeyEvent e)
    { 
        if (e.modifiers().test(Wt::KeyboardModifier::Alt)){
            if(e.modifiers().test(Wt::KeyboardModifier::Shift)){
                if(e.key() == Wt::Key::Q){

                    if(navbar->isHidden())
                    {
                        state_->stylus_node_->SetAttribute("navigation-bar-hidden", "false");
                        navbar->animateShow(Wt::WAnimation(Wt::AnimationEffect::SlideInFromLeft, Wt::TimingFunction::EaseInOut, 500));
                    }else
                    {
                        state_->stylus_node_->SetAttribute("navigation-bar-hidden", "true");
                        navbar->animateHide(Wt::WAnimation(Wt::AnimationEffect::SlideInFromLeft, Wt::TimingFunction::EaseInOut, 500));
                    }
                    state_->doc_->SaveFile(state_->state_file_path_.c_str());
                }else if(e.key() == Wt::Key::Up){
                    auto selected_node = xml_files_manager_->selected_file_brain_->selected_node_;
                    if(selected_node)
                    {
                        auto parent_node = selected_node->Parent();
                        auto prev_node = selected_node->PreviousSiblingElement();
                        if(prev_node && parent_node)
                        {
                            if(prev_node->PreviousSiblingElement())
                            {

                                parent_node->InsertAfterChild(prev_node->PreviousSiblingElement(), selected_node);
                            }else {
                                parent_node->InsertFirstChild(selected_node);
                            }
                            xml_files_manager_->selected_file_brain_->doc_->SaveFile(xml_files_manager_->selected_file_brain_->file_path_.c_str());
                            xml_files_manager_->selected_file_brain_->file_saved_.emit();
                        }
                    }
                }else if(e.key() == Wt::Key::Down){
                    auto selected_node = xml_files_manager_->selected_file_brain_->selected_node_;
                    if(selected_node)
                    {
                        auto parent_node = selected_node->Parent();
                        auto next_node = selected_node->NextSiblingElement();
                        if(next_node && parent_node)
                        {
                            parent_node->InsertAfterChild(next_node, selected_node);
                            xml_files_manager_->selected_file_brain_->doc_->SaveFile(xml_files_manager_->selected_file_brain_->file_path_.c_str());
                            xml_files_manager_->selected_file_brain_->file_saved_.emit();
                        }
                    }
                }else if(e.key() == Wt::Key::Left){
                    auto selected_node = xml_files_manager_->selected_file_brain_->selected_node_;
                    if(selected_node && 
                        selected_node != xml_files_manager_->selected_file_brain_->doc_->RootElement() &&
                        selected_node->Parent() != xml_files_manager_->selected_file_brain_->doc_->RootElement())
                    {
                        auto parent_node = selected_node->Parent();
                        auto grand_parent_node = parent_node->Parent();
                        auto parent_prev_node = parent_node->PreviousSiblingElement();
                        if(parent_prev_node){
                            grand_parent_node->InsertAfterChild(parent_prev_node, selected_node);
                        }else {
                            grand_parent_node->InsertFirstChild(selected_node);
                        }
                        xml_files_manager_->selected_file_brain_->doc_->SaveFile(xml_files_manager_->selected_file_brain_->file_path_.c_str());
                        xml_files_manager_->selected_file_brain_->file_saved_.emit();
                    }
                }else if(e.key() == Wt::Key::Right){
                    auto selected_node = xml_files_manager_->selected_file_brain_->selected_node_;
                    if(selected_node && selected_node->NextSiblingElement() &&
                            selected_node != xml_files_manager_->selected_file_brain_->doc_->RootElement() &&
                            !(selected_node->NextSiblingElement()->FirstChild() && selected_node->NextSiblingElement()->FirstChild()->ToText())){
                                
                        auto next_node = selected_node->NextSiblingElement();
                        next_node->InsertFirstChild(selected_node);
                        xml_files_manager_->selected_file_brain_->doc_->SaveFile(xml_files_manager_->selected_file_brain_->file_path_.c_str());
                        xml_files_manager_->selected_file_brain_->file_saved_.emit();                            
                    }
                }
            }else if (e.key() == Wt::Key::Q){
                if(isHidden()){
                    animateShow(Wt::WAnimation(Wt::AnimationEffect::Fade, Wt::TimingFunction::EaseInOut, 500));
                    content_wrapper->currentWidget()->refresh();
                    state_->stylus_node_->SetAttribute("open", "true");
                }else{
                    animateHide(Wt::WAnimation(Wt::AnimationEffect::Fade, Wt::TimingFunction::EaseInOut, 500));
                    state_->stylus_node_->SetAttribute("open", "false");
                }
                state_->doc_->SaveFile(state_->state_file_path_.c_str());
            }else if (e.key() == Wt::Key::Key_1){
                templates_menu_item->clicked().emit(Wt::WMouseEvent());
            }else if (e.key() == Wt::Key::Key_2){
                css_menu_item->clicked().emit(Wt::WMouseEvent());
            }else if (e.key() == Wt::Key::Key_3){
                javascript_menu_item->clicked().emit(Wt::WMouseEvent());
            }else if (e.key() == Wt::Key::Key_4){
                tailwind_menu_item->clicked().emit(Wt::WMouseEvent());
            }else if(e.key() == Wt::Key::Key_5){
                dynamic_cast<App*>(Wt::WApplication::instance())->dark_mode_changed_.emit(!state_->stylus_node_->BoolAttribute("dark-mode"));
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
                    state_->doc_->SaveFile(state_->state_file_path_.c_str());
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
                    state_->doc_->SaveFile(state_->state_file_path_.c_str());
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
                    state_->doc_->SaveFile(state_->state_file_path_.c_str());
                }
            }else if (e.key() == Wt::Key::Up){
                auto selected_node = xml_files_manager_->selected_file_brain_->selected_node_;
                if(!selected_node) return;
                auto prev_node = selected_node->PreviousSiblingElement();
                if(prev_node != nullptr)
                {
                    xml_files_manager_->selected_file_brain_->xml_node_selected_.emit(prev_node, true);
                    return;
                }
                auto parent_node = selected_node->Parent();
                if(parent_node != nullptr)
                {
                    auto last_child = parent_node->LastChildElement();
                    if(last_child != nullptr)
                    {
                        xml_files_manager_->selected_file_brain_->xml_node_selected_.emit(last_child, true);
                        return;
                    }
                }
            }else if (e.key() == Wt::Key::Down){
                auto selected_node = xml_files_manager_->selected_file_brain_->selected_node_;
                if(!selected_node) return;
                auto next_node = selected_node->NextSiblingElement();
                if(next_node != nullptr)
                {
                    xml_files_manager_->selected_file_brain_->xml_node_selected_.emit(next_node, true);
                    return;
                }
                auto parent_node = selected_node->Parent();
                if(parent_node != nullptr)
                {
                    auto first_child = parent_node->FirstChildElement();
                    if(first_child != nullptr)
                    {
                        xml_files_manager_->selected_file_brain_->xml_node_selected_.emit(first_child, true);
                        return;
                    }
                }
            }else if (e.key() == Wt::Key::Left){
                auto selected_node = xml_files_manager_->selected_file_brain_->selected_node_;
                if(selected_node != nullptr && selected_node->ToElement() != xml_files_manager_->selected_file_brain_->doc_->RootElement())
                {
                    auto parent_node = selected_node->Parent();
                    if(parent_node != nullptr)
                    {
                        xml_files_manager_->selected_file_brain_->xml_node_selected_.emit(parent_node->ToElement(), true);
                        return;
                    }
                }
            }else if (e.key() == Wt::Key::Right){
                auto selected_node = xml_files_manager_->selected_file_brain_->selected_node_;
                if(!selected_node) return;
                {
                    auto first_child = selected_node->FirstChildElement();
                    if(first_child != nullptr)
                    {
                        xml_files_manager_->selected_file_brain_->xml_node_selected_.emit(first_child, true);
                        return;
                    }
                }
            }else if (e.key() == Wt::Key::Enter){
                auto selected_node = xml_files_manager_->selected_file_brain_->selected_node_;
                if(!selected_node) return;
                auto new_node = xml_files_manager_->selected_file_brain_->doc_->NewElement("div");

                auto first_child = selected_node->FirstChild();
                if(first_child && first_child->ToText()){
                    auto parent_node = selected_node->Parent();
                    parent_node->InsertAfterChild(selected_node, new_node);
                }else if ((selected_node->FirstChild() && selected_node->FirstChild()->ToElement()) ||
                    !selected_node->FirstChild()){
                    selected_node->InsertFirstChild(new_node);
                }
                xml_files_manager_->selected_file_brain_->doc_->SaveFile(xml_files_manager_->selected_file_brain_->file_path_.c_str());
                xml_files_manager_->selected_file_brain_->selected_node_ = new_node;
                xml_files_manager_->selected_file_brain_->file_saved_.emit();
            }else if( e.key() == Wt::Key::Delete){
                auto selected_node = xml_files_manager_->selected_file_brain_->selected_node_;
                if(!selected_node || selected_node == xml_files_manager_->selected_file_brain_->doc_->RootElement()) return;
                auto parent_node = selected_node->Parent();
                auto prev_node = selected_node->PreviousSiblingElement();
                auto next_node = selected_node->NextSiblingElement();
                if(parent_node)
                {
                    parent_node->DeleteChild(selected_node);
                    xml_files_manager_->selected_file_brain_->doc_->SaveFile(xml_files_manager_->selected_file_brain_->file_path_.c_str());
                    if(prev_node)
                    {
                        xml_files_manager_->selected_file_brain_->selected_node_ = prev_node;
                    }else if(next_node)
                    {
                        xml_files_manager_->selected_file_brain_->selected_node_ = next_node;
                    }else{
                        xml_files_manager_->selected_file_brain_->selected_node_ = parent_node;
                    }
                    xml_files_manager_->selected_file_brain_->file_saved_.emit();
                }
            }else if(e.key() == Wt::Key::C){
                state_->setCopyNode(xml_files_manager_->selected_file_brain_->selected_node_->ToElement());
            }else if(e.key() == Wt::Key::V){
                auto selected_node = xml_files_manager_->selected_file_brain_->selected_node_;
                if(!selected_node) return;
                auto copied_node = state_->copy_node_;
                if(!copied_node) return;
                auto new_node = copied_node->FirstChild()->DeepClone(xml_files_manager_->selected_file_brain_->doc_.get());
                if(selected_node->FirstChild() && selected_node->FirstChild()->ToText())
                {
                    auto parent_node = selected_node->Parent();
                    parent_node->InsertAfterChild(selected_node, new_node);
                }
                else if ((selected_node->FirstChild() && selected_node->FirstChild()->ToElement()) ||
                    !selected_node->FirstChild()){
                    selected_node->InsertFirstChild(new_node);
                }
                xml_files_manager_->selected_file_brain_->doc_->SaveFile(xml_files_manager_->selected_file_brain_->file_path_.c_str());
                xml_files_manager_->selected_file_brain_->selected_node_ = new_node;
                xml_files_manager_->selected_file_brain_->file_saved_.emit();
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
