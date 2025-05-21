#include "101-Stylus/001-XmlFilesManager/Preview/ControlCenter.h"

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
        content_wrapper->setStyleClass("p-[4px] h-screen border-l border-solid border-[#000] space-y-2 flex flex-col stylus-background");

        auto menu_wrapper = addWidget(std::make_unique<Wt::WContainerWidget>());
        menu_wrapper->setStyleClass("w-[20px] h-screen border-l border-solid border-[#000] absolute top-0 right-[-20px] stylus-background");   
        
        is_condition_ = content_wrapper->addWidget(std::make_unique<Wt::WCheckBox>());
        is_condition_->setText("set as condition");
        is_condition_->disable();
        // is_condition_->setStyleClass("w-[20px] h-[20px] border-l border-solid border-[#000] absolute top-0 right-[-20px] stylus-background");

        elem_tag_ = content_wrapper->addWidget(std::make_unique<Wt::WLineEdit>());
        elem_tag_->setPlaceholderText("Element Name");
        elem_tag_->setStyleClass("shadow appearance-none border rounded w-full py-2 px-3 text-gray-700 leading-tight focus:outline-none focus:shadow-outline dark:bg-gray-800 dark:border-gray-600 dark:text-gray-200 dark:focus:border-blue-500");
        elem_tag_->disable();

        elem_classes_ = content_wrapper->addWidget(std::make_unique<Wt::WLineEdit>());
        elem_classes_->setPlaceholderText("Element Classes");
        elem_classes_->setStyleClass("shadow appearance-none border rounded w-full py-2 px-3 text-gray-700 leading-tight focus:outline-none focus:shadow-outline dark:bg-gray-800 dark:border-gray-600 dark:text-gray-200 dark:focus:border-blue-500");
        elem_classes_->disable();

        elem_text_ = content_wrapper->addWidget(std::make_unique<Wt::WTextArea>());
        elem_text_->setPlaceholderText("Element Text");
        elem_text_->setStyleClass("shadow appearance-none border rounded w-full py-2 px-3 text-gray-700 leading-tight focus:outline-none focus:shadow-outline dark:bg-gray-800 dark:border-gray-600 dark:text-gray-200 dark:focus:border-blue-500");
        elem_text_->setRows(5);
        elem_text_->disable();

        style_classes_wrapper_ = content_wrapper->addWidget(std::make_unique<Wt::WContainerWidget>());
        style_classes_wrapper_->setStyleClass("flex-1 flex flex-wrap space-x-[3px] space-y-[3px] items-start justify-start content-baseline");

    }

    void ControlCenter::setFileBrain(std::shared_ptr<XMLFileBrain> file_brain)
    {
        style_classes_wrapper_->clear();
        style_classes_.clear();
        file_brain_ = file_brain;
        auto selected_node = file_brain_->selected_node_;
        if (!selected_node) {
            elem_tag_->setText("");
            elem_classes_->setText("");
            elem_text_->setText("");
            elem_tag_->disable();
            elem_classes_->disable();
            elem_text_->disable();
            is_condition_->setChecked(false);
            return;
        }
        std::string name = selected_node->Name();
        elem_tag_->setText(name);
        elem_tag_->enable();
        if(file_brain->state_->isCondNode(selected_node))
        {
            is_condition_->setChecked(true);
            elem_classes_->disable();
            if(selected_node->FirstChild() && selected_node->FirstChild()->ToText())
            {
                elem_text_->setText(selected_node->FirstChild()->ToText()->Value());
                elem_text_->enable();
            }
            else
            {
                elem_text_->setText("");
                elem_text_->disable();
            }
        }
        else
        {
            if(selected_node == file_brain_->doc_->RootElement() )
            
            is_condition_->setChecked(false);
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


            if(selected_node->FirstChild() && selected_node->FirstChild()->ToText())
            {
                elem_text_->setText(selected_node->FirstChild()->ToText()->Value());
                elem_text_->enable();
            }
            else
            {
                elem_text_->setText("");
                elem_text_->disable();
            }
        }
    }


}