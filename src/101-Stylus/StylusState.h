#pragma once
#include <Wt/WString.h>
#include "101-Stylus/000-Utils/tinyxml2/tinyxml2.h"

namespace Stylus
{

    struct StylusEditorManagementData {
        std::string extension_;
        std::string root_folder_path_;
        std::string root_resource_url_;
    };

    struct StylusState {
        StylusState();
        tinyxml2::XMLDocument doc_;
        std::string state_file_path_;
        tinyxml2::XMLElement* stylus_node_ = nullptr;
        tinyxml2::XMLElement* xml_node_ = nullptr;
        tinyxml2::XMLElement* css_node_ = nullptr;
        tinyxml2::XMLElement* js_node_ = nullptr;
        tinyxml2::XMLElement* tailwind_config_node_ = nullptr;

        std::string tailwind_input_file_path_;
    
        StylusEditorManagementData xml_editor_data_;
        StylusEditorManagementData css_editor_data_;
        StylusEditorManagementData js_editor_data_;
        StylusEditorManagementData tailwind_config_editor_data_;

        std::string getFileText(std::string file_path);

    };

}