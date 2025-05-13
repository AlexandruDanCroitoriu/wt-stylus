
#pragma once
#include <Wt/WDialog.h>
#include <Wt/WDoubleSpinBox.h>

#include "101-Stylus/001-XmlFilesManager/XmlFilesManager.h"
#include "101-Stylus/002-CssFilesManager/CssFilesManager.h"
#include "101-Stylus/003-JsFilesManager/JsFilesManager.h"
#include "101-Stylus/004-TailwindConfigManager/TailwindConfigManager.h"
#include "101-Stylus/StylusState.h"

namespace Stylus {

    
    class Stylus : public Wt::WDialog
    {
    public:
        Stylus();
            
        XmlFilesManager* xml_files_manager_;
        CssFilesManager* css_files_manager_;
        JsFilesManager* js_files_manager_;
        TailwindConfigManager* tailwind_config_;

        
        private:
        std::shared_ptr<StylusState> state_;
        // void generateCssFile();


};
}