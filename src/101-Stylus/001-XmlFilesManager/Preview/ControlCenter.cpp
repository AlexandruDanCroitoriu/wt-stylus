#include "101-Stylus/001-XmlFilesManager/Preview/ControlCenter.h"
#include <Wt/WApplication.h>
#include <Wt/WLabel.h>
#include <regex>

namespace Stylus 
{

    ControlCenter::ControlCenter()
    {
        setStyleClass("flex flex-col text-sm");
        setMinimumSize(Wt::WLength(300, Wt::LengthUnit::Pixel), Wt::WLength(100, Wt::LengthUnit::ViewportHeight));
        setMaximumSize(Wt::WLength(300, Wt::LengthUnit::Pixel), Wt::WLength(100, Wt::LengthUnit::ViewportHeight));
        setStyleClass("stylus-background z-[100] overflow-x-visible");


        auto content_wrapper = addWidget(std::make_unique<Wt::WContainerWidget>());
        content_wrapper->setStyleClass("h-screen border-l border-solid border-[#000] space-y-2 flex flex-col stylus-background");

        auto menu_wrapper = addWidget(std::make_unique<Wt::WContainerWidget>());
        menu_wrapper->setStyleClass("w-[20px] h-screen border-l border-solid border-[#000] absolute top-0 right-[-20px] stylus-background");   
        
        is_condition_ = content_wrapper->addWidget(std::make_unique<Wt::WCheckBox>());
        is_condition_->setText("set as condition");
        is_condition_->disable();
        is_condition_->setStyleClass("pr-[10px]");

        auto elem_tag_wrapper = content_wrapper->addWidget(std::make_unique<Wt::WContainerWidget>());
        elem_tag_wrapper->setStyleClass("relative");
        auto elem_label = elem_tag_wrapper->addWidget(std::make_unique<Wt::WLabel>(""));
        elem_label->setStyleClass("absolute -top-[10px] left-[6px] text-xs font-medium !text-red-500 dark:!text-red-400 stylus-background");
        elem_tag_ = elem_tag_wrapper->addWidget(std::make_unique<Wt::WLineEdit>());
        elem_tag_->setPlaceholderText("Element Name");
        elem_tag_->setStyleClass("shadow appearance-none border rounded w-full py-2 px-3 text-gray-700 leading-tight focus:outline-none focus:shadow-outline dark:bg-gray-800 dark:border-gray-600 dark:text-gray-200 dark:focus:border-blue-500");
        elem_label->setBuddy(elem_tag_);
        elem_tag_->disable();

        auto elem_classes_wrapper = content_wrapper->addWidget(std::make_unique<Wt::WContainerWidget>());
        elem_classes_wrapper->setStyleClass("relative");
        auto elem_classes_label = elem_classes_wrapper->addWidget(std::make_unique<Wt::WLabel>(""));
        elem_classes_label->setStyleClass("absolute -top-[10px] left-[6px] text-xs font-medium !text-red-500 dark:!text-red-400 stylus-background");
        elem_classes_ = elem_classes_wrapper->addWidget(std::make_unique<Wt::WLineEdit>());
        elem_classes_label->setBuddy(elem_classes_);
        elem_classes_->setPlaceholderText("Element Classes");
        elem_classes_->setStyleClass("shadow appearance-none border rounded w-full py-2 px-3 text-gray-700 leading-tight focus:outline-none focus:shadow-outline dark:bg-gray-800 dark:border-gray-600 dark:text-gray-200 dark:focus:border-blue-500");
        elem_classes_->disable();

        auto elem_text_wrapper = content_wrapper->addWidget(std::make_unique<Wt::WContainerWidget>());
        elem_text_wrapper->setStyleClass("relative");
        auto elem_text_label = elem_text_wrapper->addWidget(std::make_unique<Wt::WLabel>(""));
        elem_text_label->setStyleClass("absolute -top-[10px] left-[6px] text-xs font-medium !text-red-500 dark:!text-red-400 stylus-background");
        elem_text_ = elem_text_wrapper->addWidget(std::make_unique<Wt::WTextArea>());
        elem_text_label->setBuddy(elem_text_);
        elem_text_->setPlaceholderText("Element Text");
        elem_text_->setStyleClass("shadow appearance-none border rounded w-full py-2 px-3 text-gray-700 leading-tight focus:outline-none focus:shadow-outline dark:bg-gray-800 dark:border-gray-600 dark:text-gray-200 dark:focus:border-blue-500");
        elem_text_->setRows(5);
        elem_text_->disable();

        is_condition_->changed().connect([=]() {
            std::cout << "\n\n is condition checkbox changed \n\n";
            if(!file_brain_ || !file_brain_->selected_node_ || 
                file_brain_->selected_node_ == file_brain_->doc_->RootElement()
            ) 
            {
                return;
            }
            auto selected_node = file_brain_->selected_node_;
            if(is_condition_->isChecked()) {
                std::cout << "\n\n is condition checkbox checked \n\n";
                selected_node->SetAttribute("true", true);
                selected_node->InsertFirstChild(file_brain_->doc_->NewText("}"));
                selected_node->InsertEndChild(file_brain_->doc_->NewText("${"));
                if(selected_node->PreviousSibling()){
                    selected_node->Parent()->InsertAfterChild(selected_node->PreviousSibling(), file_brain_->doc_->NewText("${"));
                }else {
                    selected_node->Parent()->InsertFirstChild(file_brain_->doc_->NewText("${"));
                }
                selected_node->Parent()->InsertAfterChild(selected_node, file_brain_->doc_->NewText("}"));
            
            } else {
                std::cout << "\n\n is condition checkbox unchecked \n\n";
                selected_node->DeleteAttribute("true");
                selected_node->DeleteChild(selected_node->FirstChild());
                selected_node->DeleteChild(selected_node->LastChild());
                selected_node->Parent()->DeleteChild(selected_node->PreviousSibling());
                selected_node->Parent()->DeleteChild(selected_node->NextSibling());
            }
            file_brain_->doc_->SaveFile(file_brain_->file_path_.c_str());
            file_brain_->file_saved_.emit();
        });
        elem_tag_->keyWentDown().connect([=](Wt::WKeyEvent event) { 
            Wt::WApplication::instance()->globalKeyWentDown().emit(event); 
            if(event.modifiers().test(Wt::KeyboardModifier::Control) && event.key() == Wt::Key::S){
                if(!file_brain_ || !file_brain_->selected_node_ ||
                    file_brain_->selected_node_->Name() == "messages" || 
                    file_brain_->selected_node_->Name() == "message" || 
                    file_brain_->selected_node_ == file_brain_->doc_->RootElement()
                ) {
                    elem_tag_->label()->setText("root nodes cannot be renamed");
                    return;
                }
                auto selected_node = file_brain_->selected_node_;
                if(elem_tag_->text().empty()) {
                    elem_tag_->label()->setText("empty tag name not allowed");
                    return;
                }
                if(selected_node->Name() != elem_tag_->text().toUTF8()) {
                    selected_node->SetName(elem_tag_->text().toUTF8().c_str());
                    file_brain_->doc_->SaveFile(file_brain_->file_path_.c_str());
                    file_brain_->file_saved_.emit();
                }
            }
        });
        elem_text_->keyWentDown().connect([=](Wt::WKeyEvent event) { 
            Wt::WApplication::instance()->globalKeyWentDown().emit(event); 
            if(event.modifiers().test(Wt::KeyboardModifier::Control) && event.key() == Wt::Key::S){
                if(!file_brain_ || !file_brain_->selected_node_) return;
                auto selected_node = file_brain_->selected_node_;
                if(file_brain_->state_->isCondNode(selected_node)) {
                    if(selected_node->FirstChildElement()) {
                        elem_text_->setText("");
                        elem_text_->disable();
                        return;
                    }else if(selected_node->FirstChild()->NextSibling() == selected_node->LastChild()) {
                        auto new_text_node = selected_node->GetDocument()->NewText(elem_text_->text().toUTF8().c_str());
                        selected_node->InsertAfterChild(selected_node->FirstChild(), new_text_node);
                    }else {
                        auto text_node = selected_node->FirstChild()->NextSibling();
                        text_node->SetValue(elem_text_->text().toUTF8().c_str());
                    }
                }else {

                    if(elem_text_->text().empty()) {
                        selected_node->SetText("");
                    } else {
                        selected_node->SetText(elem_text_->text().toUTF8().c_str());
                    }
                }
                file_brain_->doc_->SaveFile(file_brain_->file_path_.c_str());
                file_brain_->file_saved_.emit();
            }
        });
        is_condition_->keyWentDown().connect([=](Wt::WKeyEvent event) { Wt::WApplication::instance()->globalKeyWentDown().emit(event); });

        style_classes_wrapper_ = content_wrapper->addWidget(std::make_unique<Wt::WContainerWidget>());
        style_classes_wrapper_->setStyleClass("flex-1 flex flex-wrap space-x-[3px] space-y-[3px] items-start justify-start content-baseline");

    }

    void ControlCenter::disableAll()
    {
        elem_tag_->disable();
        elem_classes_->disable();
        elem_text_->disable();
        is_condition_->disable();
        style_classes_wrapper_->clear();
        style_classes_.clear();
    }


    void ControlCenter::setFileBrain(std::shared_ptr<XMLFileBrain> file_brain)
    {

        file_brain_ = file_brain;
        auto selected_node = file_brain_->selected_node_;

        setTagName();
        setClasses();
        setText();
        setCondition();
    }

    void ControlCenter::setTagName()
    {
        if(!file_brain_) return;
        if(!file_brain_->selected_node_) return;
        auto selected_node = file_brain_->selected_node_;
        std::string tag_name = selected_node->Name();
        elem_tag_->setText(tag_name);
        
        if(tag_name.compare("messages") == 0 || tag_name.compare("message") == 0){
            elem_tag_->disable();
        }else {
            elem_tag_->enable();
        }
    

    }
    void ControlCenter::setClasses()
    {
        style_classes_wrapper_->clear();
        style_classes_.clear();
        if(!file_brain_ || !file_brain_->selected_node_) {
            elem_classes_->disable();
            elem_classes_->setText("");
            return;
        }
        auto selected_node = file_brain_->selected_node_;

        elem_classes_->enable();
        if(selected_node->Attribute("class")){

            std::string classes = selected_node->Attribute("class");
            // split by whitespace and add to style_classes_ 
            std::regex re("\\s+");
            std::sregex_token_iterator it(classes.begin(), classes.end(), re, -1);
            std::sregex_token_iterator end;
            for (; it != end; ++it)
            {
                if (!it->str().empty()) {
                    style_classes_[it->str()] = it->str();
                    auto style_class = style_classes_wrapper_->addWidget(std::make_unique<Wt::WText>(it->str()));
                    style_class->setStyleClass("p-[6px] border border-solid border-[#000] hover:border-[gray] cursor-pointer text-nowrap");
                    
                }
            }
            
        }
    }
    void ControlCenter::setText()
    {
        if(!file_brain_ || !file_brain_->selected_node_ || file_brain_->selected_node_->FirstChildElement()) 
        {
            elem_text_->disable();
            elem_text_->setText("");
            return;
        }else if (file_brain_->selected_node_->GetText() && elem_text_->text().toUTF8().compare(file_brain_->selected_node_->GetText()) == 0) {
            elem_text_->enable();
            return; // no need to update if the text is the same
        }
        auto selected_node = file_brain_->selected_node_;
        if(file_brain_->state_->isCondNode(selected_node)) {
            std::cout << "\n\nControlCenter::setText() - selected node is a condition node, disabling text input" << std::endl;
            if(selected_node->FirstChildElement()) {
                std::cout << "\n\nControlCenter::setText() - selected node has child elements, disabling text input" << std::endl;
                elem_text_->setText(""); 
                elem_text_->disable(); 
            } else if(selected_node->FirstChild()->NextSibling() == selected_node->LastChild()) {
                std::cout << "\n\nControlCenter::setText() - selected node has only one child, disabling text input" << std::endl;
                elem_text_->setText("");
                elem_text_->enable(); 
            } else {
                std::cout << "\n\nControlCenter::setText() - selected node has text, enabling text input" << std::endl;
                elem_text_->setText(selected_node->FirstChild()->NextSibling()->Value()); 
                elem_text_->enable(); 
            }
        }else {
            if(selected_node->GetText() != nullptr) {
                // trim shitespace from start
                std::string text = selected_node->GetText();
                text.erase(0, text.find_first_not_of(" \t\n\r"));
                elem_text_->setText(text);
            } else {
                elem_text_->setText("");
            }
            elem_text_->enable();
        }
    }
    void ControlCenter::setCondition()
    {
        if(!file_brain_ || !file_brain_->selected_node_ || file_brain_->selected_node_ == file_brain_->doc_->RootElement())  
        {
            is_condition_->disable();
            is_condition_->setChecked(false);
            return;
        }
        is_condition_->enable();
        auto selected_node = file_brain_->selected_node_;
        if(file_brain_->state_->isCondNode(selected_node)) {
            is_condition_->setChecked(true);
        } else {
            is_condition_->setChecked(false);
        }
    }


}