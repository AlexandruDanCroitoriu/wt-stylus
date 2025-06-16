#include "101-Stylus/001-XmlFilesManager/Preview/TemplateVarControlCenter.h"
#include <Wt/WApplication.h>
#include <Wt/WLabel.h>
#include <Wt/WPushButton.h>
#include <regex>
#include <Wt/WRegExpValidator.h>
#include <Wt/WComboBox.h>

namespace Stylus 
{

    TemplateVarControlCenter::TemplateVarControlCenter()
    {
        setMinimumSize(Wt::WLength(300, Wt::LengthUnit::Pixel), Wt::WLength(100, Wt::LengthUnit::ViewportHeight));
        setMaximumSize(Wt::WLength(300, Wt::LengthUnit::Pixel), Wt::WLength(100, Wt::LengthUnit::ViewportHeight));
        setStyleClass("preview-background z-[100] overflow-x-visible mt-[16px]");

        auto content_wrapper = addWidget(std::make_unique<Wt::WContainerWidget>());
        content_wrapper->setStyleClass("h-screen border-l border-solid border-[#000] space-y-2 flex flex-col");

        auto menu_wrapper = addWidget(std::make_unique<Wt::WContainerWidget>());
        menu_wrapper->setStyleClass("w-[20px] h-screen border-l border-solid border-[#000] absolute top-0 right-[-20px]");   
        
        std::string input_styles = "shadow appearance-none border rounded w-full py-2 px-3 text-gray-700 leading-tight focus:outline-none focus:shadow-outline dark:bg-gray-800 dark:border-gray-600 dark:text-gray-200 dark:focus:border-blue-500 disabled:bg-gray-200 disabled:dark:bg-gray-700 disabled:cursor-not-allowed";

        auto temp_var_name_wrapper = content_wrapper->addWidget(std::make_unique<Wt::WContainerWidget>());
        temp_var_name_wrapper->setStyleClass("relative");
        auto temp_var_name_label = temp_var_name_wrapper->addWidget(std::make_unique<Wt::WLabel>(""));
        temp_var_name_label->setStyleClass("absolute -top-[10px] left-[6px] text-xs font-medium !text-red-500 dark:!text-red-400");
        temp_var_name_ = temp_var_name_wrapper->addWidget(std::make_unique<Wt::WLineEdit>());
        temp_var_name_label->setBuddy(temp_var_name_);
        temp_var_name_->setPlaceholderText("Template Variable Name");
        temp_var_name_->setStyleClass(input_styles);
        temp_var_name_->disable();

        auto temp_var_classes_wrapper = content_wrapper->addWidget(std::make_unique<Wt::WContainerWidget>());
        temp_var_classes_wrapper->setStyleClass("relative");
        auto temp_var_classes_label = temp_var_classes_wrapper->addWidget(std::make_unique<Wt::WLabel>(""));
        temp_var_classes_label->setStyleClass("absolute -top-[10px] left-[6px] text-xs font-medium !text-red-500 dark:!text-red-400");
        temp_var_classes_ = temp_var_classes_wrapper->addWidget(std::make_unique<Wt::WLineEdit>());
        temp_var_classes_label->setBuddy(temp_var_classes_);
        temp_var_classes_->setPlaceholderText("Template variable Classes");
        temp_var_classes_->setStyleClass(input_styles);
        temp_var_classes_->disable();

        style_classes_wrapper_ = content_wrapper->addWidget(std::make_unique<Wt::WContainerWidget>());
        style_classes_wrapper_->setStyleClass("flex-1 flex flex-wrap space-x-[3px] space-y-[3px] items-start justify-start content-baseline");

        temp_var_classes_->keyWentDown().connect([=](Wt::WKeyEvent event) { 
            Wt::WApplication::instance()->globalKeyWentDown().emit(event); 
            if(event.modifiers().test(Wt::KeyboardModifier::Control) && event.key() == Wt::Key::S && temp_var_data_.var_name_.compare("") != 0){
                std::cout << "\n\nTemplateVarControlCenter::setClasses() - temp_var_classes_ keyWentDown event triggered\n\n";
                if(!file_brain_ || !file_brain_->selected_node_){
                    temp_var_classes_->label()->setText("no selected node or brain");   
                    return;
                }
                if(std::regex_search(temp_var_classes_->valueText().toUTF8(), std::regex(" "))) {
                    temp_var_classes_->label()->setText("no spaces allowed in class names");
                    return;
                }
                if(temp_var_classes_->valueText().toUTF8().empty()) {
                    temp_var_classes_->label()->setText("no classes to add");
                    return;
                }
                std::cout << "\n\nTemplateVarControlCenter::setClasses() - saving classes: " << temp_var_classes_->valueText().toUTF8() << "\n\n";
                // add the new class to the style_classes_ vector
                style_classes_.push_back(temp_var_classes_->valueText().toUTF8());
                // update the class attribute of the selected node
                file_brain_->selected_node_->FirstChild()->ToText()->SetValue(getTempVar().c_str());
                file_brain_->doc_->SaveFile(file_brain_->file_path_.c_str());
                file_brain_->file_saved_.emit();
            }
        });

        temp_var_name_->keyWentDown().connect([=](Wt::WKeyEvent event) { 
            Wt::WApplication::instance()->globalKeyWentDown().emit(event); 
            if(event.modifiers().test(Wt::KeyboardModifier::Control) && event.key() == Wt::Key::S && temp_var_data_.var_name_.compare("") != 0){
                std::cout << "\n\nTemplateVarControlCenter::setTempVarName() - temp_var_name_ keyWentDown event triggered\n\n";
                if(!file_brain_ || !file_brain_->selected_node_){
                    temp_var_name_->label()->setText("no selected node or brain");
                    return;
                }
                if(temp_var_name_->valueText().toUTF8().empty()) {
                    temp_var_name_->label()->setText("no name to set");
                    return;
                }
                std::cout << "\n\nTemplateVarControlCenter::setTempVarName() - saving name: " << temp_var_name_->valueText().toUTF8() << "\n\n";
                // update the var_name of the temp_var_data_
                temp_var_data_.var_name_ = temp_var_name_->valueText().toUTF8();
                // update the text of the selected node
                file_brain_->selected_node_->FirstChild()->ToText()->SetValue(getTempVar().c_str());
                file_brain_->doc_->SaveFile(file_brain_->file_path_.c_str());
                file_brain_->file_saved_.emit();
            }
        });
      
    }

    void TemplateVarControlCenter::disableAll()
    {
        temp_var_classes_->disable();     
        temp_var_name_->disable();   
        temp_var_name_->setText("");
        temp_var_classes_->setText("");
        style_classes_wrapper_->clear();
        style_classes_.clear();
    }


    void TemplateVarControlCenter::setFileBrain(std::shared_ptr<XMLFileBrain> file_brain)
    {
        // std::cout << "\n\nTemplateVarControlCenter::setFileBrain() - setting file brain\n\n";
        if(!file_brain || !file_brain->selected_node_) {
            std::cout << "\n\nTemplateVarControlCenter::setFileBrain() - no file brain or selected node\n\n";
            disableAll();
            temp_var_classes_->label()->setText("no node selected");
            temp_var_name_->label()->setText("no node selected");
            return;
        }
        file_brain_ = file_brain;
        auto selected_node = file_brain_->selected_node_;
        temp_var_data_ = file_brain_->state_->getTempNodeVarData(selected_node);
        if(temp_var_data_.var_name_.compare("") == 0) {
            std::cout << "\n\nTemplateVarControlCenter::setFileBrain() - no temp var data found\n\n";
            disableAll();
            temp_var_name_->label()->setText("no temp var data found");
            temp_var_classes_->label()->setText("no temp var data found");
            return;
        } 
        std::cout << "\n\n setting classes \n\n";
        setClasses();
        std::cout << "\n\n setting temp var name \n\n";
        setTempVarName();
    }

    void TemplateVarControlCenter::setClasses()
    {
        style_classes_wrapper_->clear();
        style_classes_.clear();
        temp_var_classes_->enable();
        temp_var_classes_->label()->setText("");
        temp_var_classes_->setText("");


        if(temp_var_data_.attributes_["class"].compare("") != 0) {

            std::string classes = temp_var_data_.attributes_["class"];
            // split by whitespace and add to style_classes_ 
            std::regex re("\\s+");
            std::sregex_token_iterator it(classes.begin(), classes.end(), re, -1);
            std::sregex_token_iterator end;
            for (; it != end; ++it)
            {
                if (!it->str().empty()) {
                    std::string style_class_str = it->str();
                    style_classes_.push_back(style_class_str);
                    auto style_class = style_classes_wrapper_->addWidget(std::make_unique<Wt::WPushButton>(style_class_str));
                    style_class->setStyleClass("text-[14px] p-[6px] border border-solid border-[#000] hover:border-[gray] cursor-pointer text-nowrap hover:bg-[red]/20");
                    style_class->clicked().connect([=]() {
                        std::cout << "\n\nTemplateVarControlCenter::setClasses() - style class clicked: " << style_class_str << "\n\n";
                        std::string new_classes;
                        for (const auto& style_class : style_classes_) {
                            if (style_class.compare(style_class_str) == 0) {
                                style_classes_.erase(std::remove(style_classes_.begin(), style_classes_.end(), style_class_str), style_classes_.end());
                                break;
                            }
                        }
                        file_brain_->selected_node_->FirstChild()->ToText()->SetValue(getTempVar().c_str());
                        file_brain_->doc_->SaveFile(file_brain_->file_path_.c_str());
                        file_brain_->file_saved_.emit();
                        temp_var_classes_->setText(style_class_str);
                    });
                }
            }
        }
    }

    void TemplateVarControlCenter::setTempVarName()
    {
        temp_var_name_->enable();
        temp_var_name_->label()->setText("");
        temp_var_name_->setText(temp_var_data_.var_name_);
    }


    std::string TemplateVarControlCenter::getTempVar()
    {
        std::string temp_var = "${";
        if(temp_var_data_.function_.compare("") != 0) {
            temp_var += temp_var_data_.function_ + ":";
        }
        temp_var += temp_var_data_.var_name_;
        if(!temp_var_data_.attributes_.empty()) {
            temp_var += " ";
            for(const auto& attr : temp_var_data_.attributes_) {
                if(attr.first.compare("class") == 0) {
                    std::string classes = "";
                    // split by whitespace and add to style_classes_ 
                    for (const auto& style_class : style_classes_) {
                        classes += style_class + " ";
                    }
                    if(classes.compare("") != 0) {
                        temp_var += attr.first + "=\"" + classes + "\" ";
                    }
                }else {
                    temp_var += attr.first + "=\"" + attr.second + "\" ";
                }
            }
        } 
        temp_var += "}";
        return temp_var;
    }
}