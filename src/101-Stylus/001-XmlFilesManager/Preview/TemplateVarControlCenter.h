#pragma once
#include "101-Stylus/000-Utils/tinyxml2/tinyxml2.h"
#include "101-Stylus/000-Utils/XMLFileBrain.h"

#include <Wt/WContainerWidget.h>
#include <Wt/WLineEdit.h>
#include <Wt/WCheckBox.h>
#include <Wt/WTextArea.h>

#include <vector>
#include <map>

namespace Stylus 
{

    
    class TemplateVarControlCenter : public Wt::WContainerWidget
    {
        public:
            TemplateVarControlCenter();

            void setFileBrain(std::shared_ptr<XMLFileBrain> file_brain);
            void disableAll();
        private:
            std::shared_ptr<XMLFileBrain> file_brain_;
            TempNodeVarData temp_var_data_;
            
            Wt::WLineEdit* temp_var_name_;
            Wt::WLineEdit* temp_var_classes_;
            std::vector<std::string> style_classes_;
            Wt::WContainerWidget* style_classes_wrapper_;

            std::string getTempVar();

            void setTempVarName();
            void setClasses();
    };
}