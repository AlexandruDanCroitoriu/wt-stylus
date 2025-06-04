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

namespace Stylus
{

    Stylus::Stylus(Session& session)
        : session_(session)
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

        setMinimumSize(Wt::WLength(100, Wt::LengthUnit::ViewportWidth), Wt::WLength(100, Wt::LengthUnit::ViewportHeight));
        setLayoutSizeAware(true);
     
        Wt::WApplication::instance()->doJavaScript(WT_CLASS R"(
            .$(')" + id() + R"(').oncontextmenu = function() {
                event.cancelBubble = true;
                event.returnValue = false;
                return false;
            };
            document.addEventListener('keydown', function(event) {
                if (event.altKey && (event.key === 'ArrowLeft' || event.key === 'ArrowRight')) {
                    event.preventDefault();
                    // Your custom logic here if needed
                } else if ((event.ctrlKey || event.metaKey) && event.key === 's') {
                    event.preventDefault();
                }
            });
        )");
        // Wt::WApplication::instance()->useStyleSheet(Wt::WApplication::instance()->docRoot() + "/static/stylus/stylus.css?v=" + Wt::WRandom::generateId());
        // Wt::WApplication::instance()->require(Wt::WApplication::instance()->docRoot() + "/static/stylus/monaco-edditor.js");
        Wt::WApplication::instance()->require("/static/stylus/monaco-edditor.js");

        Wt::WApplication::instance()->messageResourceBundle().use(Wt::WApplication::instance()->docRoot() + "/static/stylus/templates");

        // if (session_.login().loggedIn() && session_.user()->hasPermission("STYLUS_FILES_MANAGER")) {
        session_.login().changed().connect(this, [=]()
        {
            if (session_.login().loggedIn()) {
                std::cout << "\n\nStylus logged in successfully.\n\n";
                Wt::Dbo::Transaction transaction(session_);
                stylus_permission_ = session_.find<Permission>().where("name = ?").bind("STYLUS_FILES_MANAGER").resultValue();
                if(!stylus_permission_) {
                    std::cout << "\n\nStylus permission not found, creating it...\n\n";
                    stylus_permission_ = session_.add(std::make_unique<Permission>("STYLUS_FILES_MANAGER"));
                }
                if(session_.user()->hasPermission(stylus_permission_) && state_ == nullptr) {
                    setupStylus();
                }
                transaction.commit();
            } else {
                std::cout << "\n\nStylus logged out successfully.\n\n";
            }
        });
    }
    
    void Stylus::setupStylus()
    {
        state_ = std::make_shared<StylusState>();
      
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
        std::cout << "\n\nStylus initialized successfully.\n\n";

        xml_files_manager_ = content_wrapper->addWidget(std::make_unique<XmlFilesManager>(state_));
        std::cout << "\n\nStylus initialized successfully.\n\n";
        css_files_manager_ = content_wrapper->addWidget(std::make_unique<CssFilesManager>(state_));
        std::cout << "\n\nStylus initialized successfully.\n\n";
        js_files_manager_ = content_wrapper->addWidget(std::make_unique<JsFilesManager>(state_));
        std::cout << "\n\nStylus initialized successfully.\n\n";
        tailwind_config_ = content_wrapper->addWidget(std::make_unique<TailwindConfigManager>(state_));
        std::cout << "\n\nStylus initialized successfully.\n\n";
        

        templates_menu_item->clicked().connect(this, [=]()
                                               {
        templates_menu_item->toggleStyleClass("stylus-menu-selected", true);
        tailwind_menu_item->toggleStyleClass("stylus-menu-selected", false);
        css_menu_item->toggleStyleClass("stylus-menu-selected", false);
        javascript_menu_item->toggleStyleClass("stylus-menu-selected", false);
        content_wrapper->setCurrentWidget(xml_files_manager_);
        state_->stylus_node_->SetAttribute("selected-menu", "templates");
        state_->doc_->SaveFile(state_->state_file_path_.c_str()); });

        tailwind_menu_item->clicked().connect(this, [=]()
                                              {
        templates_menu_item->toggleStyleClass("stylus-menu-selected", false);
        tailwind_menu_item->toggleStyleClass("stylus-menu-selected", true);
        css_menu_item->toggleStyleClass("stylus-menu-selected", false);
        javascript_menu_item->toggleStyleClass("stylus-menu-selected", false);
        content_wrapper->setCurrentWidget(tailwind_config_);
        state_->stylus_node_->SetAttribute("selected-menu", "tailwind");
        state_->doc_->SaveFile(state_->state_file_path_.c_str()); });

        css_menu_item->clicked().connect(this, [=]()
                                         {
        templates_menu_item->toggleStyleClass("stylus-menu-selected", false);
        tailwind_menu_item->toggleStyleClass("stylus-menu-selected", false);
        css_menu_item->toggleStyleClass("stylus-menu-selected", true);
        javascript_menu_item->toggleStyleClass("stylus-menu-selected", false);
        content_wrapper->setCurrentWidget(css_files_manager_);
        state_->stylus_node_->SetAttribute("selected-menu", "css");
        state_->doc_->SaveFile(state_->state_file_path_.c_str()); });

        javascript_menu_item->clicked().connect(this, [=]()
                                                {
        templates_menu_item->toggleStyleClass("stylus-menu-selected", false);
        tailwind_menu_item->toggleStyleClass("stylus-menu-selected", false);
        css_menu_item->toggleStyleClass("stylus-menu-selected", false);
        javascript_menu_item->toggleStyleClass("stylus-menu-selected", true);
        content_wrapper->setCurrentWidget(js_files_manager_);
        state_->stylus_node_->SetAttribute("selected-menu", "javascript");
        state_->doc_->SaveFile(state_->state_file_path_.c_str()); });

        auto selected_menu = state_->stylus_node_->Attribute("selected-menu");
        if (std::strcmp(selected_menu, "templates") == 0)
            templates_menu_item->clicked().emit(Wt::WMouseEvent());
        else if (std::strcmp(selected_menu, "tailwind") == 0)
            tailwind_menu_item->clicked().emit(Wt::WMouseEvent());
        else if (std::strcmp(selected_menu, "css") == 0)
            css_menu_item->clicked().emit(Wt::WMouseEvent());
        else if (std::strcmp(selected_menu, "javascript") == 0)
            javascript_menu_item->clicked().emit(Wt::WMouseEvent());

        if (state_->stylus_node_->BoolAttribute("open"))
            show();
        else
            hide();

        if (state_->stylus_node_->BoolAttribute("navigation-bar-hidden"))
        {
            navbar->animateHide(Wt::WAnimation(Wt::AnimationEffect::SlideInFromLeft, Wt::TimingFunction::EaseInOut, 500));
        }
        if (state_->xml_node_->BoolAttribute("navigation-bar-hidden"))
        {
            xml_files_manager_->grid_layout_->itemAt(0)->widget()->animateHide(Wt::WAnimation(Wt::AnimationEffect::SlideInFromLeft, Wt::TimingFunction::EaseInOut, 500));
        }
        if (state_->css_node_->BoolAttribute("navigation-bar-hidden"))
        {
            css_files_manager_->grid_layout_->itemAt(0)->widget()->animateHide(Wt::WAnimation(Wt::AnimationEffect::SlideInFromLeft, Wt::TimingFunction::EaseInOut, 500));
        }
        if (state_->js_node_->BoolAttribute("navigation-bar-hidden"))
        {
            js_files_manager_->grid_layout_->itemAt(0)->widget()->animateHide(Wt::WAnimation(Wt::AnimationEffect::SlideInFromLeft, Wt::TimingFunction::EaseInOut, 500));
        }

        dark_mode_toggle->dark_mode_changed_.connect(this, [=](bool dark)
                                                     {
        xml_files_manager_->editor_->setDarkTheme(dark);
        state_->stylus_node_->SetAttribute("dark-mode", dark ? "true" : "false");
        state_->doc_->SaveFile(state_->state_file_path_.c_str()); });

        if (state_->stylus_node_->BoolAttribute("dark-mode"))
        {
            dynamic_cast<App *>(Wt::WApplication::instance())->dark_mode_changed_.emit(true);
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
                    if(!xml_files_manager_->selected_file_brain_) return;
                    auto selected_node = xml_files_manager_->selected_file_brain_->selected_node_;
                    if(!selected_node) return;
                    auto parent_node = selected_node->Parent();
                    if(!selected_node->PreviousSiblingElement()) return;
                    if(selected_node->PreviousSiblingElement()->PreviousSiblingElement())
                    {
                        auto prev_prev_elem_node = selected_node->PreviousSiblingElement()->PreviousSiblingElement();
                        auto prev_node = selected_node->PreviousSibling();
                        auto next_node = selected_node->NextSibling();

                        if(state_->isCondNode(selected_node) && 
                            state_->isCondNode(prev_prev_elem_node)
                        ){
                            // selected node, prev prev sibling elements are condition nodes
                            std::cout << "\n\nselected node, prev prev sibling elements are condition nodes\n";
                            auto end_condition_node = prev_prev_elem_node->NextSibling();
                            parent_node->InsertAfterChild(end_condition_node, prev_node);
                            parent_node->InsertAfterChild(prev_node, selected_node);
                            parent_node->InsertAfterChild(selected_node, next_node);
                        }else if(state_->isCondNode(selected_node)){
                            // selected node is condition node
                            std::cout << "\n\nselected node is condition node\n";
                            parent_node->InsertAfterChild(prev_prev_elem_node, prev_node);
                            parent_node->InsertAfterChild(prev_node, selected_node);
                            parent_node->InsertAfterChild(selected_node, next_node);
                        }else if(state_->isCondNode(prev_prev_elem_node)){
                            // prev prev sibling element is condition node
                            std::cout << "\n\nprev prev sibling element is condition node\n";
                            auto end_condition_node = prev_prev_elem_node->NextSibling();
                            parent_node->InsertAfterChild(end_condition_node, selected_node);
                        }else {
                            // selected node and prev prev sibling element are not condition nodes
                            std::cout << "\n\nselected node and prev prev sibling element are not condition nodes\n";
                            parent_node->InsertAfterChild(prev_prev_elem_node, selected_node);
                        }
                    }else {
                        if(state_->isCondNode(selected_node)){
                            // selected node is condition node
                            auto prev_node = selected_node->PreviousSibling();
                            auto next_node = selected_node->NextSibling();
                            if(state_->isCondNode(selected_node->PreviousSiblingElement()) && 
                                state_->trimAllWitespace(selected_node->PreviousSiblingElement()->NextSibling()->ToText()->Value()).compare("}${") == 0    
                            ){
                                selected_node->PreviousSiblingElement()->NextSibling()->ToText()->SetValue("}");
                                prev_node = xml_files_manager_->selected_file_brain_->doc_->NewText("${");
                            }
                            parent_node->InsertFirstChild(prev_node);
                            parent_node->InsertAfterChild(prev_node, selected_node);
                            parent_node->InsertAfterChild(selected_node, next_node);
                        }else {
                            parent_node->InsertFirstChild(selected_node);
                        }
                    }
                    xml_files_manager_->selected_file_brain_->doc_->SaveFile(xml_files_manager_->selected_file_brain_->file_path_.c_str());
                    xml_files_manager_->selected_file_brain_->file_saved_.emit();
                }else if(e.key() == Wt::Key::Down){
                    if(!xml_files_manager_->selected_file_brain_) return;
                    auto selected_node = xml_files_manager_->selected_file_brain_->selected_node_;
                    if(!selected_node) return;
                    auto parent_node = selected_node->Parent();
                    auto prev_node = selected_node->PreviousSibling();
                    auto next_node = selected_node->NextSibling();
                    if(!selected_node->NextSiblingElement()) return;
                    if(state_->isCondNode(selected_node) && 
                        state_->isCondNode(selected_node->NextSiblingElement())
                    ){
                        // selected node and next sibling element are condition nodes
                        std::cout << "\n\nselected node and next sibling element are condition nodes\n";
                        parent_node->InsertAfterChild(selected_node->NextSiblingElement()->NextSibling(), prev_node);
                        parent_node->InsertAfterChild(prev_node, selected_node);
                        parent_node->InsertAfterChild(selected_node, next_node);
                    }else if(state_->isCondNode(selected_node)){
                        // selected node is condition node
                        std::cout << "\n\nselected node is condition node\n";
                        parent_node->InsertAfterChild(selected_node->NextSiblingElement(), prev_node);
                        parent_node->InsertAfterChild(prev_node, selected_node);
                        parent_node->InsertAfterChild(selected_node, next_node);
                    }else if(state_->isCondNode(selected_node->NextSiblingElement())){
                        // next sibling element is condition node
                        std::cout << "\n\nnext sibling element is condition node\n";
                        auto end_condition_node = selected_node->NextSiblingElement()->NextSibling();
                        parent_node->InsertAfterChild(end_condition_node, selected_node);
                    }else {
                        // selected node and next sibling element are not condition nodes
                        std::cout << "\n\nselected node and next sibling element are not condition nodes\n";
                        parent_node->InsertAfterChild(selected_node->NextSiblingElement(), selected_node);
                    }
                    xml_files_manager_->selected_file_brain_->doc_->SaveFile(xml_files_manager_->selected_file_brain_->file_path_.c_str());
                    xml_files_manager_->selected_file_brain_->file_saved_.emit();
                }else if(e.key() == Wt::Key::Left){
                    if(!xml_files_manager_->selected_file_brain_) return;
                    auto selected_node = xml_files_manager_->selected_file_brain_->selected_node_;
                    if(selected_node && 
                        selected_node != xml_files_manager_->selected_file_brain_->doc_->RootElement() &&
                        selected_node->Parent() != xml_files_manager_->selected_file_brain_->doc_->RootElement())
                    {
                        auto parent_node = selected_node->Parent();
                        auto grand_parent_node = parent_node->Parent();
                        auto parent_prev_node_elem = parent_node->PreviousSiblingElement();
                        auto prev_node = selected_node->PreviousSibling();
                        auto next_node = selected_node->NextSibling();
                        if(parent_prev_node_elem){
                            if(state_->isCondNode(parent_prev_node_elem) && 
                                state_->isCondNode(selected_node)
                            ){
                                // parent prev node and selected node are condition nodes
                                std::cout << "\n\nparent prev node and selected node are condition nodes\n";
                                auto parent_prev_node = parent_prev_node_elem->NextSibling();
                                grand_parent_node->InsertAfterChild(parent_prev_node, prev_node);
                                grand_parent_node->InsertAfterChild(prev_node, selected_node);
                                grand_parent_node->InsertAfterChild(selected_node, next_node);
                            }else if(state_->isCondNode(parent_prev_node_elem)){
                                // parent prev node is condition node
                                std::cout << "\n\nparent prev node is condition node\n";
                                grand_parent_node->InsertAfterChild(parent_prev_node_elem->NextSibling(), selected_node);
                            }else if(state_->isCondNode(selected_node)){
                                // selected node is condition node
                                std::cout << "\n\nselected node is condition node\n";
                                grand_parent_node->InsertAfterChild(parent_prev_node_elem, prev_node);
                                grand_parent_node->InsertAfterChild(prev_node, selected_node); 
                                grand_parent_node->InsertAfterChild(selected_node, next_node);
                            }else {
                                std::cout << "\n\nselected node and parent prev node are not condition nodes\n";
                                grand_parent_node->InsertAfterChild(parent_prev_node_elem, selected_node);
                            }
                        }else {
                            if(state_->isCondNode(selected_node) && 
                                state_->isCondNode(grand_parent_node->ToElement())
                            ){
                                // selected node and grand parent node are condition nodes
                                std::cout << "\n\nselected node and grand parent node are condition nodes"<<grand_parent_node->FirstChild()->ToText()->Value() << "|" <<
                                grand_parent_node->LastChild()->ToText()->Value() << "\n";
                                auto end_condition_node = grand_parent_node->FirstChild();
                                grand_parent_node->InsertAfterChild(end_condition_node, prev_node);
                                grand_parent_node->InsertAfterChild(prev_node, selected_node);
                                grand_parent_node->InsertAfterChild(selected_node, next_node);                                

                            }else if (state_->isCondNode(selected_node)){
                                // selected node is condition node
                                std::cout << "\n\nselected node is condition node\n";
                                grand_parent_node->InsertFirstChild(prev_node);
                                grand_parent_node->InsertAfterChild(prev_node, selected_node);
                                grand_parent_node->InsertAfterChild(selected_node, next_node);
                            }else if(state_->isCondNode(grand_parent_node->ToElement())){
                                // grand parent node is condition node
                                std::cout << "\n\ngrand parent node is condition node\n";
                                auto end_condition_node = grand_parent_node->FirstChild();
                                grand_parent_node->InsertAfterChild(end_condition_node, selected_node);
                            }else {
                                // selected node and grand parent node are not condition nodes
                                std::cout << "\n\nselected node and grand parent node are not condition nodes\n";
                                grand_parent_node->InsertFirstChild(selected_node);
                            }
                        }
                        xml_files_manager_->selected_file_brain_->doc_->SaveFile(xml_files_manager_->selected_file_brain_->file_path_.c_str());
                        xml_files_manager_->selected_file_brain_->file_saved_.emit();
                    }
                }else if(e.key() == Wt::Key::Right){
                    if(!xml_files_manager_->selected_file_brain_) return;
                    auto selected_node = xml_files_manager_->selected_file_brain_->selected_node_;
                    if(selected_node && selected_node != xml_files_manager_->selected_file_brain_->doc_->RootElement() &&
                        selected_node->NextSiblingElement()){
                        auto next_node_elem = selected_node->NextSiblingElement();
                        auto next_node = selected_node->NextSibling();
                        auto prev_node = selected_node->PreviousSibling();

                        if(state_->isCondNode(next_node_elem) &&
                        state_->isCondNode(selected_node)
                        ){
                            // selected node and next node are condition nodes
                            std::cout << "\n\nselected node and next node are condition nodes\n";
                            auto end_condition_node = next_node_elem->FirstChild();
                            next_node_elem->InsertAfterChild(end_condition_node, prev_node);
                            next_node_elem->InsertAfterChild(prev_node, selected_node);
                            next_node_elem->InsertAfterChild(selected_node, next_node);
                        }else if(state_->isCondNode(selected_node) && !(next_node_elem->FirstChild() && next_node_elem->FirstChild()->ToText())){
                            // selected node is condition node
                            std::cout << "\n\nselected node is condition node\n";
                            next_node_elem->InsertFirstChild(prev_node);
                            next_node_elem->InsertAfterChild(prev_node, selected_node);
                            next_node_elem->InsertAfterChild(selected_node, next_node);
                        }else if(state_->isCondNode(next_node_elem)){
                            // next node is condition node
                            std::cout << "\n\nnext node is condition node\n";
                            auto end_condition_node = next_node_elem->FirstChild();
                            next_node_elem->InsertAfterChild(end_condition_node, selected_node);
                        }else if(!(next_node_elem->FirstChild() && next_node_elem->FirstChild()->ToText())){
                            // selected node and next node are not condition nodes
                            std::cout << "\n\nselected node and next node are not condition nodes\n";
                            next_node_elem->InsertFirstChild(selected_node);
                        }        
                    }
                    xml_files_manager_->selected_file_brain_->doc_->SaveFile(xml_files_manager_->selected_file_brain_->file_path_.c_str());
                    xml_files_manager_->selected_file_brain_->file_saved_.emit();
                }else if(e.key() == Wt::Key::Key_1){
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
                }else if(e.key() == Wt::Key::Key_2){
                    if(content_wrapper->currentWidget() == xml_files_manager_)
                    {
                        if(state_->xml_node_->BoolAttribute("editor-hidden"))
                        {
                            xml_files_manager_->editor_->animateShow(Wt::WAnimation(Wt::AnimationEffect::SlideInFromLeft, Wt::TimingFunction::EaseInOut, 500));
                            state_->xml_node_->SetAttribute("editor-hidden", "false");
                        }else
                        {
                            xml_files_manager_->editor_->animateHide(Wt::WAnimation(Wt::AnimationEffect::SlideInFromLeft, Wt::TimingFunction::EaseInOut, 500));
                            state_->xml_node_->SetAttribute("editor-hidden", "true");
                        }
                        state_->doc_->SaveFile(state_->state_file_path_.c_str());
                    }
                }else if(e.key() == Wt::Key::Key_3){
                    if(content_wrapper->currentWidget() == xml_files_manager_)
                    {
                        if(state_->xml_node_->BoolAttribute("preview-tree-hidden"))
                        {
                            xml_files_manager_->tree_wrapper_->animateShow(Wt::WAnimation(Wt::AnimationEffect::SlideInFromLeft, Wt::TimingFunction::EaseInOut, 500));
                            state_->xml_node_->SetAttribute("preview-tree-hidden", "false");
                        }else
                        {
                            xml_files_manager_->tree_wrapper_->animateHide(Wt::WAnimation(Wt::AnimationEffect::SlideInFromLeft, Wt::TimingFunction::EaseInOut, 500));
                            state_->xml_node_->SetAttribute("preview-tree-hidden", "true");
                        }
                        state_->doc_->SaveFile(state_->state_file_path_.c_str());
                    }
                }else if(e.key() == Wt::Key::Key_4){
                    if(content_wrapper->currentWidget() == xml_files_manager_)
                    {
                        if(state_->xml_node_->BoolAttribute("preview-elem-hidden"))
                        {
                            xml_files_manager_->elem_wrapper_->animateShow(Wt::WAnimation(Wt::AnimationEffect::SlideInFromRight, Wt::TimingFunction::EaseInOut, 500));
                            state_->xml_node_->SetAttribute("preview-elem-hidden", "false");
                        }else
                        {
                            xml_files_manager_->elem_wrapper_->animateHide(Wt::WAnimation(Wt::AnimationEffect::SlideInFromRight, Wt::TimingFunction::EaseInOut, 500));
                            state_->xml_node_->SetAttribute("preview-elem-hidden", "true");
                        }
                        state_->doc_->SaveFile(state_->state_file_path_.c_str());
                    }
                }else if(e.key() == Wt::Key::Key_5){
                    if(content_wrapper->currentWidget() == xml_files_manager_)
                    {
                        if(state_->xml_node_->BoolAttribute("control-center-hidden"))
                        {
                            xml_files_manager_->control_center_->animateShow(Wt::WAnimation(Wt::AnimationEffect::SlideInFromRight, Wt::TimingFunction::EaseInOut, 500));
                            state_->xml_node_->SetAttribute("control-center-hidden", "false");
                        }else
                        {
                            xml_files_manager_->control_center_->animateHide(Wt::WAnimation(Wt::AnimationEffect::SlideInFromRight, Wt::TimingFunction::EaseInOut, 500));
                            state_->xml_node_->SetAttribute("control-center-hidden", "true");
                        }
                        state_->doc_->SaveFile(state_->state_file_path_.c_str());
                    }
                }
            }else if (e.key() == Wt::Key::Q){
                if(isHidden()){
                    animateShow(Wt::WAnimation(Wt::AnimationEffect::Pop, Wt::TimingFunction::EaseInOut, 500));
                    content_wrapper->currentWidget()->refresh();
                    state_->stylus_node_->SetAttribute("open", "true");
                }else{
                    animateHide(Wt::WAnimation(Wt::AnimationEffect::Pop, Wt::TimingFunction::EaseInOut, 500));
                    state_->stylus_node_->SetAttribute("open", "false");
                    Wt::WMessageResourceBundle& resource_boundle = Wt::WApplication::instance()->messageResourceBundle();
                    Wt::WApplication::instance()->refresh();
                    
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
            }else if (e.key() == Wt::Key::Up){
                if(!xml_files_manager_->selected_file_brain_) return;
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
                if(!xml_files_manager_->selected_file_brain_) return;
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
                if(!xml_files_manager_->selected_file_brain_) return;
                auto selected_node = xml_files_manager_->selected_file_brain_->selected_node_;
                if(selected_node && selected_node->ToElement() != xml_files_manager_->selected_file_brain_->doc_->RootElement())
                {
                    auto parent_node = selected_node->Parent();
                    if(parent_node != nullptr)
                    {
                        xml_files_manager_->selected_file_brain_->xml_node_selected_.emit(parent_node->ToElement(), true);
                        return;
                    }
                }
            }else if (e.key() == Wt::Key::Right){
                if(!xml_files_manager_->selected_file_brain_) return;
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
                if(!xml_files_manager_->selected_file_brain_) return;
                auto selected_node = xml_files_manager_->selected_file_brain_->selected_node_;
                std::cout << "\n\n enter key pressed\n";
                if(!selected_node) return;
                std::cout << "\n\n enter key pressed\n";

                auto new_node = xml_files_manager_->selected_file_brain_->doc_->NewElement("div");
                std::cout << "\n\n enter key pressed\n";

                if(xml_files_manager_->selected_file_brain_->selected_node_ == xml_files_manager_->selected_file_brain_->doc_->RootElement()) {
                    std::cout << "\n\n selected node is root element\n";
                    if(selected_node->FirstChildElement() && state_->isCondNode(selected_node->FirstChildElement())){
                        std::cout << "\n\nselected node first child is condition node\n";
                        selected_node->InsertFirstChild(new_node);
                    }else if(selected_node->FirstChild() && selected_node->FirstChild()->ToText()){
                        std::cout << "\n\nselected node first child is text node\n";
                        new_node = xml_files_manager_->selected_file_brain_->selected_node_;
                    }else {
                        std::cout << "\n\nselected node root and first child are not condition or text nodes\n";
                        new_node = xml_files_manager_->selected_file_brain_->doc_->NewElement("div");
                        selected_node->InsertFirstChild(new_node);
                    }
                }else {
                    if(state_->isCondNode(selected_node))
                    {
                        std::cout << "\n\nselected node is condition node\n";
                        auto end_condition_node = selected_node->FirstChild();
                        selected_node->InsertAfterChild(end_condition_node, new_node);
                    }else if (selected_node->FirstChildElement() && state_->isCondNode(selected_node->FirstChildElement()))
                    {
                        std::cout << "\n\nselected node first child is condition node\n";
                        selected_node->InsertFirstChild(new_node);
                    }else {
                        std::cout << "\n\nselected node and first child are not condition nodes\n";
                        auto first_child = selected_node->FirstChild();
                        if(first_child && first_child->ToText()){
                            auto parent_node = selected_node->Parent();
                            parent_node->InsertAfterChild(selected_node, new_node);
                        }else if ((selected_node->FirstChild() && selected_node->FirstChild()->ToElement()) || !selected_node->FirstChild()){
                            selected_node->InsertFirstChild(new_node);
                        }
                    }
                }
                xml_files_manager_->selected_file_brain_->doc_->SaveFile(xml_files_manager_->selected_file_brain_->file_path_.c_str());
                xml_files_manager_->selected_file_brain_->selected_node_ = new_node;
                xml_files_manager_->selected_file_brain_->file_saved_.emit();
            }else if( e.key() == Wt::Key::Delete){
                if(!xml_files_manager_->selected_file_brain_) return;
                auto selected_node = xml_files_manager_->selected_file_brain_->selected_node_;
                if(!selected_node || selected_node == xml_files_manager_->selected_file_brain_->doc_->RootElement()) return;
                auto parent_node = selected_node->Parent();
                auto prev_node = selected_node->PreviousSiblingElement();
                auto next_node = selected_node->NextSiblingElement();
                if(parent_node)
                {
                    if(state_->isCondNode(selected_node))
                    {
                        parent_node->DeleteChild(selected_node->NextSibling());
                        parent_node->DeleteChild(selected_node->PreviousSibling());
                    }
                    parent_node->DeleteChild(selected_node);
                    if(prev_node)
                    {
                        xml_files_manager_->selected_file_brain_->selected_node_ = prev_node;
                    }else if(next_node)
                    {
                        xml_files_manager_->selected_file_brain_->selected_node_ = next_node;
                    }else{
                        xml_files_manager_->selected_file_brain_->selected_node_ = parent_node->ToElement();
                    }
                    xml_files_manager_->selected_file_brain_->doc_->SaveFile(xml_files_manager_->selected_file_brain_->file_path_.c_str());
                    xml_files_manager_->selected_file_brain_->file_saved_.emit();
                }
            }else if(e.key() == Wt::Key::C){
                if(!xml_files_manager_->selected_file_brain_) return;
                if(!state_->copy_node_) return;
                state_->copy_node_->DeleteChildren();
                auto selected_node = xml_files_manager_->selected_file_brain_->selected_node_;
                if(!selected_node) return;
                if(state_->isCondNode(selected_node))
                {
                    std::cout << "\n\nselected node is condition node\n";
                    auto prev_node = selected_node->PreviousSibling()->DeepClone(state_->doc_.get());
                    auto next_node = selected_node->NextSibling()->DeepClone(state_->doc_.get());
                    state_->copy_node_->InsertFirstChild(prev_node);
                    auto copy_selected_node = state_->copy_node_->InsertAfterChild(prev_node, selected_node->DeepClone(state_->doc_.get()));
                    state_->copy_node_->InsertAfterChild(copy_selected_node, next_node);
                }else {
                    std::cout << "\n\nselected node and first child are not condition nodes\n";
                    state_->copy_node_->InsertFirstChild(selected_node->DeepClone(state_->doc_.get()));
                }
                state_->organizeXmlNode(state_->copy_node_->ToElement(), state_->state_file_path_);
                state_->doc_->SaveFile(state_->state_file_path_.c_str());
            }else if(e.key() == Wt::Key::V){
                if(!xml_files_manager_->selected_file_brain_) return;
                auto selected_node = xml_files_manager_->selected_file_brain_->selected_node_;
                if(!selected_node) return;
                auto copy_node = state_->copy_node_;
                if(!copy_node) return;
                std::cout << "\n\n Paste node \n";
                bool is_selected_cond = state_->isCondNode(selected_node);
                bool is_copy_cond = state_->isCondNode(copy_node->FirstChildElement());
                if(is_selected_cond && is_copy_cond)
                {
                    // copy and selected are conditions
                    std::cout << "\n\ncopy and selected are conditions\n";
                    auto prev_node = copy_node->FirstChild()->DeepClone(xml_files_manager_->selected_file_brain_->doc_.get());
                    auto cond_node = selected_node->DeepClone(xml_files_manager_->selected_file_brain_->doc_.get());
                    auto next_node = copy_node->LastChild()->DeepClone(xml_files_manager_->selected_file_brain_->doc_.get());
                    selected_node->InsertAfterChild(selected_node->FirstChild(), prev_node);
                    selected_node->InsertAfterChild(prev_node, cond_node);
                    selected_node->InsertAfterChild(cond_node, next_node);
                    xml_files_manager_->selected_file_brain_->selected_node_ = cond_node->ToElement();
                    state_->organizeXmlNode(cond_node->ToElement(), state_->state_file_path_);
                }else if(is_selected_cond && !is_copy_cond)
                {
                    // copy is not condition
                    std::cout << "\n\ncopy is not condition\n";
                    auto copy_node_child = copy_node->FirstChild()->DeepClone(xml_files_manager_->selected_file_brain_->doc_.get());
                    selected_node->InsertAfterChild(selected_node->FirstChild(), copy_node_child);
                    xml_files_manager_->selected_file_brain_->selected_node_ = copy_node_child->ToElement();
                }else if(!is_selected_cond && is_copy_cond)
                {
                    // selected is not condition
                    std::cout << "\n\nselected is not condition copy is condition\n";

                    if(selected_node != xml_files_manager_->selected_file_brain_->doc_->RootElement())
                    {
                        std::cout << "\n\nselected node is not root element\n";
                        if(selected_node->FirstChild() && selected_node->FirstChild()->ToText()){
                            std::cout << "\n\nselected node first child is not text 1\n";
                            auto parent_node = selected_node->Parent();
                            auto prev = parent_node->InsertAfterChild(selected_node, copy_node->FirstChild()->DeepClone(xml_files_manager_->selected_file_brain_->doc_.get()));
                            auto new_selected = parent_node->InsertAfterChild(prev, copy_node->FirstChildElement()->DeepClone(xml_files_manager_->selected_file_brain_->doc_.get()));
                            parent_node->InsertAfterChild(new_selected, copy_node->LastChild()->DeepClone(xml_files_manager_->selected_file_brain_->doc_.get()));
                            xml_files_manager_->selected_file_brain_->selected_node_ = new_selected->ToElement();
                            state_->organizeXmlNode(new_selected->ToElement(), state_->state_file_path_);
                        }else {
                            std::cout << "\n\nselected node has child element and first child is not text\n";
                            auto prev = selected_node->InsertFirstChild(copy_node->FirstChild()->DeepClone(xml_files_manager_->selected_file_brain_->doc_.get()));
                            auto new_selected = selected_node->InsertAfterChild(prev, copy_node->FirstChildElement()->DeepClone(xml_files_manager_->selected_file_brain_->doc_.get()));
                            selected_node->InsertAfterChild(new_selected, copy_node->LastChild()->DeepClone(xml_files_manager_->selected_file_brain_->doc_.get()));
                            xml_files_manager_->selected_file_brain_->selected_node_ = new_selected->ToElement();
                            state_->organizeXmlNode(new_selected->ToElement(), state_->state_file_path_);
                        }
                    }else {
                        std::cout << "\n\nselected node is root element\n";

                        if(selected_node->FirstChild() && selected_node->FirstChild()->ToText() && selected_node->FirstChildElement() && 
                        state_->isCondNode(selected_node->FirstChildElement())
                        ){
                            std::cout << "\n\nselected node first child is not text 2\n";
                            auto prev = selected_node->InsertFirstChild(copy_node->FirstChild()->DeepClone(xml_files_manager_->selected_file_brain_->doc_.get()));
                            auto new_selected = selected_node->InsertAfterChild(prev, copy_node->FirstChildElement()->DeepClone(xml_files_manager_->selected_file_brain_->doc_.get()));
                            selected_node->InsertAfterChild(new_selected, copy_node->LastChild()->DeepClone(xml_files_manager_->selected_file_brain_->doc_.get()));
                            xml_files_manager_->selected_file_brain_->selected_node_ = new_selected->ToElement();
                            state_->organizeXmlNode(new_selected->ToElement(), state_->state_file_path_);
                        }else if((selected_node->FirstChild() && !selected_node->FirstChild()->ToText() || !selected_node->FirstChild())){
                            std::cout << "\n\nselected node first child is not text 3\n";
                            auto prev = selected_node->InsertFirstChild(copy_node->FirstChild()->DeepClone(xml_files_manager_->selected_file_brain_->doc_.get()));
                            auto new_selected = selected_node->InsertAfterChild(prev, copy_node->FirstChildElement()->DeepClone(xml_files_manager_->selected_file_brain_->doc_.get()));
                            selected_node->InsertAfterChild(new_selected, copy_node->LastChild()->DeepClone(xml_files_manager_->selected_file_brain_->doc_.get()));
                            xml_files_manager_->selected_file_brain_->selected_node_ = new_selected->ToElement();
                            state_->organizeXmlNode(new_selected->ToElement(), state_->state_file_path_);
                        }
                    }
                }else {
                    // selected and copy are not conditions
                    std::cout << "\n\nselected and copy are not conditions\n";
                    auto copy_node_child = copy_node->FirstChild()->DeepClone(xml_files_manager_->selected_file_brain_->doc_.get());
                    if(selected_node->FirstChild() && selected_node->FirstChild()->ToText()){
                        if(selected_node != xml_files_manager_->selected_file_brain_->doc_->RootElement())
                        {
                            std::cout << "\n\nselected node is not root element\n";
                            auto parent_node = selected_node->Parent();
                            auto new_selected = parent_node->InsertAfterChild(selected_node, copy_node_child);
                            xml_files_manager_->selected_file_brain_->selected_node_ = new_selected->ToElement();
                        }
                    }else {
                        xml_files_manager_->selected_file_brain_->selected_node_ = selected_node->InsertFirstChild(copy_node_child)->ToElement();
                    }
                                        
                }
                
                xml_files_manager_->selected_file_brain_->doc_->SaveFile(xml_files_manager_->selected_file_brain_->file_path_.c_str());
                xml_files_manager_->selected_file_brain_->file_saved_.emit();
            }
        } });

        css_files_manager_->file_saved().connect(this, [=]()
                                                 { tailwind_config_->generateCssFile(); });
        xml_files_manager_->file_saved().connect(this, [=]()
                                                 { tailwind_config_->generateCssFile(); });

        tailwind_config_->generateCssFile();
    }

}
