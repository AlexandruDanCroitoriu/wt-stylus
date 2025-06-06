
#pragma once
#include <Wt/WDialog.h>
#include <Wt/WDoubleSpinBox.h>

#include "002-Dbo/Session.h"

#include "101-Stylus/000-Utils/StylusState.h"
#include "101-Stylus/001-XmlFilesManager/XmlFilesManager.h"
#include "101-Stylus/002-CssFilesManager/CssFilesManager.h"
#include "101-Stylus/003-JsFilesManager/JsFilesManager.h"
#include "101-Stylus/004-TailwindConfigManager/TailwindConfigManager.h"
#include "101-Stylus/005-Images/ImagesManager.h"
#include "101-Stylus/006-Settings/Settings.h"

namespace Stylus {

    
    class Stylus : public Wt::WDialog
    {
    public:
        Stylus(Session& session);
            
        XmlFilesManager* xml_files_manager_;
        CssFilesManager* css_files_manager_;
        JsFilesManager* js_files_manager_;
        TailwindConfigManager* tailwind_config_;
        ImagesManager* images_manager_;
        Settings* settings_;
        
    private:
        std::shared_ptr<StylusState> state_;
        // void generateCssFile();
        Session& session_;
        void setupStylus();
        Wt::Dbo::ptr<Permission> stylus_permission_;

};
}