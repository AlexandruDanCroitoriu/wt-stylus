#pragma once
#include "101-Stylus/000-Utils/tinyxml2/tinyxml2.h"
#include "101-Stylus/001-XmlFilesManager/Preview/XMLFileBrain.h"

#include <Wt/WContainerWidget.h>
#include <Wt/WLineEdit.h>
#include <Wt/WCheckBox.h>
#include <Wt/WTextArea.h>

#include <vector>
#include <map>

namespace Stylus 
{

    
    class ControlCenter : public Wt::WContainerWidget
    {
        public:
            ControlCenter();

            void setFileBrain(std::shared_ptr<XMLFileBrain> file_brain);
        private:
            std::shared_ptr<XMLFileBrain> file_brain_;
            Wt::WLineEdit* elem_tag_;
            Wt::WTextArea* elem_text_;
            Wt::WLineEdit* elem_classes_;
            Wt::WCheckBox* is_condition_;
            std::map<std::string, std::string> style_classes_;
            Wt::WContainerWidget* style_classes_wrapper_;

            void setTagName();
            void setClasses();
            void setText();
            void setCondition();
            
    };
}