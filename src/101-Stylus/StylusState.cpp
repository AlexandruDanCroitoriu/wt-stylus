#include "101-Stylus/StylusState.h"
#include <Wt/WApplication.h>
#include <Wt/WServer.h>
#include <Wt/WIOService.h>
#include <Wt/WRandom.h>
#include <fstream>
#include <iostream>
#include <filesystem>

namespace Stylus {

    StylusState::StylusState()
    {
        state_file_path_ = "../static/stylus/stylus-state.xml";

        js_editor_data_.extension_ = "js";
        js_editor_data_.root_folder_path_ = "../static/stylus-resources/js/";
        js_editor_data_.root_resource_url_ = "static/stylus-resources/js/";

        xml_editor_data_.extension_ = "xml";
        xml_editor_data_.root_folder_path_ = "../static/stylus-resources/xml/";
        xml_editor_data_.root_resource_url_ = "static/stylus-resources/xml/";

        css_editor_data_.extension_ = "css";
        css_editor_data_.root_folder_path_ = "../static/stylus-resources/tailwind4/css/";
        css_editor_data_.root_resource_url_ = "static/stylus-resources/tailwind4/css/";

        tailwind_config_editor_data_.extension_ = "css";
        tailwind_config_editor_data_.root_folder_path_ = "../static/stylus-resources/tailwind-config/";
        tailwind_config_editor_data_.root_resource_url_ = "static/stylus-resources/tailwind-config/";

        tailwind_input_file_path_ = "../static/stylus-resources/tailwind4/input.css";

        doc_.LoadFile(state_file_path_.c_str());
        if (doc_.ErrorID() != tinyxml2::XML_SUCCESS)
        {
            std::cerr << "Error loading XML file: " << doc_.ErrorID() << std::endl;
            // crete file 
            std::ofstream file(state_file_path_);
            if (!file.is_open())
            {
                std::cerr << "Error creating file: " << state_file_path_ << std::endl;
                return;
            }
            doc_.LoadFile(state_file_path_.c_str());
        }
        stylus_node_ = doc_.FirstChildElement("stylus");
        if (stylus_node_ == nullptr)
        {
            std::cerr << "Error finding <stylus> node in XML file." << std::endl;
            stylus_node_ = doc_.NewElement("stylus");
            stylus_node_->SetAttribute("navigation-bar-hidden", "false");
            stylus_node_->SetAttribute("selected-menu", "templates");
            stylus_node_->SetAttribute("open", "true");
            stylus_node_->SetAttribute("dark-mode", "true");
            doc_.InsertFirstChild(stylus_node_);
        }
        xml_node_ = stylus_node_->FirstChildElement("xml-manager");
        if (xml_node_ == nullptr)
        {
            std::cerr << "Error finding <xml-manager> node in XML file." << std::endl;
            xml_node_ = doc_.NewElement("xml-manager");
            xml_node_->SetAttribute("editor-width", 500);
            xml_node_->SetAttribute("sidebar-width", 300);
            xml_node_->SetAttribute("selected-file-path", "");
            xml_node_->SetAttribute("navigation-bar-hidden", "false");
            stylus_node_->InsertEndChild(xml_node_);
        }
        css_node_ = stylus_node_->FirstChildElement("css-manager");
        if (css_node_ == nullptr)
        {
            std::cerr << "Error finding <css-manager> node in XML file." << std::endl;
            css_node_ = doc_.NewElement("css-manager");
            css_node_->SetAttribute("sidebar-width", 300);
            css_node_->SetAttribute("selected-file-path", "");
            css_node_->SetAttribute("navigation-bar-hidden", "false");
            stylus_node_->InsertEndChild(css_node_);
        }
        js_node_ = stylus_node_->FirstChildElement("js-manager");
        if (js_node_ == nullptr)
        {
            std::cerr << "Error finding <js-manager> node in XML file." << std::endl;
            js_node_ = doc_.NewElement("js-manager");
            js_node_->SetAttribute("sidebar-width", 300);
            js_node_->SetAttribute("selected-file-path", "");
            js_node_->SetAttribute("navigation-bar-hidden", "false");
            stylus_node_->InsertEndChild(js_node_);
        }
        tailwind_config_node_ = stylus_node_->FirstChildElement("tailwind-config");
        if (tailwind_config_node_ == nullptr)
        {
            std::cerr << "Error finding <tailwind-config> node in XML file." << std::endl;
            tailwind_config_node_ = doc_.NewElement("tailwind-config");
            tailwind_config_node_->SetAttribute("editor-width", 500);
            tailwind_config_node_->SetAttribute("selected-file-name", "");
            stylus_node_->InsertEndChild(tailwind_config_node_);
        }

        doc_.SaveFile(state_file_path_.c_str());
        if (doc_.ErrorID() != tinyxml2::XML_SUCCESS)
        {
            std::cerr << "Error saving XML file: " << doc_.ErrorID() << std::endl;
        }
    }


    std::string StylusState::getFileText(std::string file_path)
    {
        std::ifstream file(file_path);
        if (!file.is_open())
        {
            std::cout << "\n\n Failed to read file: " << file_path << "\n\n";
            return "!Failed to read file!";
        }
    
        std::string file_content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        file.close();
        Wt::WString file_content_wt = Wt::WString::fromUTF8(file_content);
        return file_content;
    }
    
}