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
    : doc_(std::make_shared<tinyxml2::XMLDocument>())
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

        doc_->LoadFile(state_file_path_.c_str());
        if (doc_->ErrorID() != tinyxml2::XML_SUCCESS)
        {
            std::cerr << "Error loading stylus state XML file: " << doc_->ErrorID() << std::endl;
            // crete file 
            std::ofstream file(state_file_path_);
            if (!file.is_open())
            {
                std::cerr << "Error creating file: " << state_file_path_ << std::endl;
                return;
            }
            doc_->LoadFile(state_file_path_.c_str());
        }
        stylus_node_ = doc_->FirstChildElement("stylus");
        if (stylus_node_ == nullptr)
        {
            std::cerr << "Error finding <stylus> node in XML file." << std::endl;
            stylus_node_ = doc_->NewElement("stylus");
            stylus_node_->SetAttribute("navigation-bar-hidden", "false");
            stylus_node_->SetAttribute("selected-menu", "templates");
            stylus_node_->SetAttribute("open", "true");
            stylus_node_->SetAttribute("dark-mode", "true");
            doc_->InsertFirstChild(stylus_node_);
        }
        xml_node_ = stylus_node_->FirstChildElement("xml-manager");
        if (xml_node_ == nullptr)
        {
            std::cerr << "Error finding <xml-manager> node in XML file." << std::endl;
            xml_node_ = doc_->NewElement("xml-manager");
            xml_node_->SetAttribute("editor-width", 500);
            xml_node_->SetAttribute("editor-hidden", false);
            xml_node_->SetAttribute("preview-tree-hidden", false);
            xml_node_->SetAttribute("preview-elem-hidden", false);
            xml_node_->SetAttribute("control-center-hidden", false);
            xml_node_->SetAttribute("sidebar-width", 300);
            xml_node_->SetAttribute("selected-file-path", "");
            xml_node_->SetAttribute("navigation-bar-hidden", "false");
            xml_node_->SetAttribute("preview-widget-sidebar-width", "300");
            xml_node_->SetAttribute("preview-widget-width", "300");
            
            stylus_node_->InsertEndChild(xml_node_);
        }
        css_node_ = stylus_node_->FirstChildElement("css-manager");
        if (css_node_ == nullptr)
        {
            std::cerr << "Error finding <css-manager> node in XML file." << std::endl;
            css_node_ = doc_->NewElement("css-manager");
            css_node_->SetAttribute("sidebar-width", 300);
            css_node_->SetAttribute("selected-file-path", "");
            css_node_->SetAttribute("navigation-bar-hidden", "false");
            stylus_node_->InsertEndChild(css_node_);
        }
        js_node_ = stylus_node_->FirstChildElement("js-manager");
        if (js_node_ == nullptr)
        {
            std::cerr << "Error finding <js-manager> node in XML file." << std::endl;
            js_node_ = doc_->NewElement("js-manager");
            js_node_->SetAttribute("sidebar-width", 300);
            js_node_->SetAttribute("selected-file-path", "");
            js_node_->SetAttribute("navigation-bar-hidden", "false");
            stylus_node_->InsertEndChild(js_node_);
        }
        tailwind_config_node_ = stylus_node_->FirstChildElement("tailwind-config");
        if (tailwind_config_node_ == nullptr)
        {
            std::cerr << "Error finding <tailwind-config> node in XML file." << std::endl;
            tailwind_config_node_ = doc_->NewElement("tailwind-config");
            tailwind_config_node_->SetAttribute("editor-width", 500);
            tailwind_config_node_->SetAttribute("selected-file-name", "");
            stylus_node_->InsertEndChild(tailwind_config_node_);
        }
        settings_node_ = stylus_node_->FirstChildElement("settings");
        if (settings_node_ == nullptr)
        {
            std::cerr << "Error finding <settings> node in XML file." << std::endl;
            settings_node_ = doc_->NewElement("settings");
            stylus_node_->InsertEndChild(settings_node_);
        }
        images_manager_node_ = stylus_node_->FirstChildElement("images-manager");
        if (images_manager_node_ == nullptr)
        {
            std::cerr << "Error finding <images-manager> node in XML file." << std::endl;
            images_manager_node_ = doc_->NewElement("images-manager");
        }
        copy_node_ = doc_->FirstChildElement("copy");
        if (copy_node_ == nullptr)
        {
            std::cerr << "Error finding <copy> node in XML file." << std::endl;
            copy_node_ = doc_->NewElement("copy");
            doc_->InsertEndChild(copy_node_);
        }
        organizeXmlNode(copy_node_, state_file_path_);
        doc_->SaveFile(state_file_path_.c_str());
        if (doc_->ErrorID() != tinyxml2::XML_SUCCESS)
        {
            std::cerr << "Error saving XML file: " << doc_->ErrorID() << std::endl;
        }
        std::cout << "\n\nStylusState initialized successfully.\n\n";
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
    
    void StylusState::organizeXmlNode(tinyxml2::XMLElement* node, std::string file_path)
    {
        // std::cout << "\n\nOrganizing XML node: " << node->Name() << "\n\n";
        if(node != node->GetDocument()->RootElement())
        {
            if(isCondNode(node))
            {
                // std::cout << "\n\nCondition node\n";
                if(node->PreviousSibling() && node->PreviousSibling()->ToText() && trimAllWitespace(node->PreviousSibling()->ToText()->Value()).compare("${") != 0)
                {
                    std::cout << "\n\n selected previous sibling check \n\n";
                    std::string text = node->PreviousSibling()->ToText()->Value();
                    std::string before_start_cond = text.substr(0, text.length()-2); 
                    std::string after_start_cond = text.substr(text.length()-2, 2);
                    // std::cout << "Splitting previous sibling text: '" << trimAllWitespace(text) << "' before_start_cond '" << trimAllWitespace(before_start_cond) << "' after_start_cond '" << trimAllWitespace(after_start_cond) << "'\n";
                    node->PreviousSibling()->ToText()->SetValue(before_start_cond.c_str());
                    auto parent_node = node->Parent();
                    parent_node->InsertAfterChild(node->PreviousSibling(), node->GetDocument()->NewText(after_start_cond.c_str()));                   
                }
                if(node->NextSibling() && node->NextSibling()->ToText() && trimAllWitespace(node->NextSibling()->ToText()->Value()).compare("}") != 0)
                {
                    std::cout << "\n\n selected next sibling check \n\n";
                    std::string text = node->NextSibling()->ToText()->Value();
                    std::string after_cond_text = text.substr(0, 1);
                    std::string after_after_cont_text = text.substr(1, text.length()-1);
                    // std::cout << "Splitting next sibling text: '" << trimAllWitespace(text) << "' after_cond_text '" << trimAllWitespace(after_cond_text) << "' after_after_cont_text '" << trimAllWitespace(after_after_cont_text) << "'\n";
                    node->NextSibling()->ToText()->SetValue(after_cond_text.c_str());
                    auto parent_node = node->Parent();
                    parent_node->InsertAfterChild(node->NextSibling(), node->GetDocument()->NewText(after_after_cont_text.c_str()));
                }
                if(node->FirstChild() && node->FirstChild()->ToText() && trimAllWitespace(node->FirstChild()->ToText()->Value()).compare("}") != 0)
                {
                    std::cout << "\n\n selected first child check \n\n";
                    std::string text = node->FirstChild()->ToText()->Value();
                    std::string first_child_text = text.substr(0, 1);
                    std::string after_first_child_text = text.substr(1, text.length()-1);
                    // std::cout << "Splitting first child text:'" << trimAllWitespace(text) << "' \nfirst_child_text '" << trimAllWitespace(first_child_text) << "' \nafter_first_child_text '" << trimAllWitespace(after_first_child_text) << "'\n";
                    node->FirstChild()->ToText()->SetValue(first_child_text.c_str());
                    node->InsertAfterChild(node->FirstChild(), node->GetDocument()->NewText(after_first_child_text.c_str()));
                }
                if(node->LastChild() && node->LastChild()->ToText() && trimAllWitespace(node->LastChild()->ToText()->Value()).compare("${") != 0)
                {
                    std::cout << "\n\n selected last child check \n\n";
                    std::string text = node->LastChild()->ToText()->Value();
                    std::cout << "text: '" << trimAllWitespace(text) << "'\n";
                    std::string last_child_text = text.substr(text.length()-2, 2);
                    std::string before_end_cond = text.substr(0, text.length()-2);
                    // std::cout << "Splitting last child text: '" << trimAllWitespace(text) << "' before_end_cond '" << trimAllWitespace(before_end_cond) << "' last_child_text '" << trimAllWitespace(last_child_text) << "'\n";
                    node->LastChild()->ToText()->SetValue(last_child_text.c_str());
                    node->InsertAfterChild(node->LastChild()->PreviousSibling(), node->GetDocument()->NewText(before_end_cond.c_str()));
                }
                // if(node->LastChild() && node->LastChild()->ToText() && trimAllWitespace(node->LastChild()->ToText()->Value()).compare("${") != 0)
                // {
                //     std::cout << "\n\n selected last child check \n\n";
                //     std::string text = node->LastChild()->ToText()->Value();
                //     std::string last_child_text = text.substr(text.length()-2, 2);
                //     std::string before_end_cond = text.substr(0, text.length()-2);
                //     std::cout << "Splitting last child text: '" << trimAllWitespace(text) << "' before_end_cond '" << trimAllWitespace(before_end_cond) << "' last_child_text '" << trimAllWitespace(last_child_text) << "'\n";
                //     node->LastChild()->ToText()->SetValue(last_child_text.c_str());
                //     node->InsertAfterChild(node->LastChild()->PreviousSibling(), doc_->NewText(before_end_cond.c_str()));
                // }
            }
        }else {
            if(std::string(node->Name()).compare("messages") != 0 &&
                std::string(node->Name()).compare("message") != 0 && 
                std::string(node->Name()).compare("div") != 0 && 
                std::string(node->Name()).compare("stylus") !=  0)
            {
                node->SetName("div");
                node->GetDocument()->SaveFile(file_path.c_str());
            }
        }
        auto child_node = node->FirstChild();
        while (child_node) {
            if (child_node->ToElement()) {
                organizeXmlNode(child_node->ToElement(), file_path);
            } else if (child_node->ToText()) {
            }
            child_node = child_node->NextSibling();
        }
    }

    
    bool StylusState::isCondNode(tinyxml2::XMLElement* node)
    {
        auto prev_node = node->PreviousSibling();
        auto next_node = node->NextSibling();
        auto first_child = node->FirstChild();
        auto last_child = node->LastChild();
        if(prev_node && prev_node->ToText() &&
            next_node && next_node->ToText() &&
            first_child && first_child->ToText() &&
            last_child && last_child->ToText()            
        ){
            
            std::string prev_node_text = trimAllWitespace(prev_node->ToText()->Value());
            std::string next_node_text = trimAllWitespace(next_node->ToText()->Value());
            std::string first_child_text = trimAllWitespace(first_child->ToText()->Value());
            std::string last_child_text = trimAllWitespace(last_child->ToText()->Value());
          

            if(prev_node_text.length() < 2 || next_node_text.length() < 1 || 
                first_child_text.length() < 1 || last_child_text.length() < 2)
            {
                return false;
            }
            prev_node_text = prev_node_text.substr(prev_node_text.length()-2, 2);
            next_node_text = next_node_text.substr(0, 1);
            first_child_text = first_child_text.substr(0, 1);
            last_child_text = last_child_text.substr(last_child_text.length()-2, 2);
         
            if(prev_node_text.compare("${") == 0 && next_node_text.compare("}") == 0 &&
                first_child_text.compare("}") == 0 && last_child_text.compare("${") == 0)
            {
                return true;
            }
        }
        return false;
    }
    std::string StylusState::trimWitespace(std::string str)
    {
        str.erase(0, str.find_first_not_of(" \t\n\r\f\v")); // trim from start
        str.erase(str.find_last_not_of(" \t\n\r\f\v") + 1); // trim from end
        return str;
    }
    std::string StylusState::trimAllWitespace(std::string str)
    {
        str.erase(std::remove_if(str.begin(), str.end(), ::isspace), str.end());
        return str;
    }
        
}