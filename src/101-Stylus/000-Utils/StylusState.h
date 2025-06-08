#pragma once
#include <Wt/WString.h>
#include "101-Stylus/000-Utils/tinyxml2/tinyxml2.h"
#include <Wt/WSignal.h>

namespace Stylus
{

    class XMLFileBrain;

    struct StylusEditorManagementData {
        std::string extension_;
        std::string root_folder_path_;
        std::string root_resource_url_;
    };

    struct MessageAttributeData {
        std::string folder_name_ = "";
        std::string file_name_ = "";
    };

    struct TempNodeVarData {
        std::string function_ = "";
        std::string var_name_ = "";
        std::unordered_map<std::string, std::string> attributes_ = {};
        static MessageAttributeData getMessageAttributeData(std::string message_attribute_value);
    };

    struct StylusState {
        StylusState();
        std::shared_ptr<tinyxml2::XMLDocument> doc_;
        std::string state_file_path_;
        tinyxml2::XMLElement* stylus_node_ = nullptr;
        tinyxml2::XMLElement* xml_node_ = nullptr;
        tinyxml2::XMLElement* css_node_ = nullptr;
        tinyxml2::XMLElement* js_node_ = nullptr;
        tinyxml2::XMLElement* tailwind_config_node_ = nullptr;
        tinyxml2::XMLElement* settings_node_ = nullptr;
        tinyxml2::XMLElement* images_manager_node_ = nullptr;
        tinyxml2::XMLElement* copy_node_ = nullptr;

        std::string tailwind_input_file_path_;
    
        StylusEditorManagementData xml_editor_data_;
        StylusEditorManagementData css_editor_data_;
        StylusEditorManagementData js_editor_data_;
        StylusEditorManagementData tailwind_config_editor_data_;

        std::string getFileText(std::string file_path);
        /* 
        organizeXmlNode is used to split condition ${} brackets into separate text nodes,
        and in the future tu propagate changes to the xml tree
        */
        void organizeXmlNode(tinyxml2::XMLElement* node, std::string file_path); // file path is used to save the file after organizing it.
        bool isCondNode(tinyxml2::XMLElement* node);
        TempNodeVarData getTempNodeVarData(tinyxml2::XMLElement* node);

        // all the brains, they are manadged and refreshed by widgets as this object is passed to them
        std::map<std::string, std::shared_ptr<XMLFileBrain>> xml_file_brains_;
        tinyxml2::XMLElement* getMessageNode(std::string folder_name, std::string file_name, std::string message_id);


        Wt::Signal<> file_saved_;



    };

}