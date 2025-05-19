#include "101-Stylus/001-XmlFilesManager/Preview/XMLFileBrain.h"



namespace Stylus
{
    XMLFileBrain::XMLFileBrain(std::shared_ptr<StylusState> state, std::string file_path)
    : state_(state),
    doc_(std::make_shared<tinyxml2::XMLDocument>())
    {
        setFile(file_path);
    }
    
    void XMLFileBrain::setFile(std::string file_path)
    {
        file_path_ = file_path;
        // Load the XML file
        tinyxml2::XMLError eResult = doc_->LoadFile(file_path.c_str());
        if (eResult != tinyxml2::XML_SUCCESS) {
            std::cout << "\n\nError loading XML file: " << doc_->ErrorIDToName(eResult) << "\n\n";
            std::cout << "File not found: " << file_path << "\n\n";
            selected_node_ = nullptr;
        }else {
            selected_node_ = doc_->RootElement();
        }
        xml_node_selected_.emit(selected_node_, false);
        
    }

    std::string XMLFileBrain::trimWitespace(std::string str)
    {
        str.erase(0, str.find_first_not_of(" \t\n\r\f\v")); // trim from start
        str.erase(str.find_last_not_of(" \t\n\r\f\v") + 1); // trim from end
        return str;
    }
    std::string XMLFileBrain::trimAllWitespace(std::string str)
    {
        str.erase(std::remove_if(str.begin(), str.end(), ::isspace), str.end());
        return str;
    }

    bool XMLFileBrain::isCondNode(tinyxml2::XMLElement* node)
    {
        auto prev_node = node->PreviousSibling();
        auto next_node = node->NextSibling();
        if(prev_node && prev_node->ToText() && next_node && next_node->ToText()){
            
            std::string prev_node_text = trimAllWitespace(prev_node->ToText()->Value());
            std::string next_node_text = trimAllWitespace(next_node->ToText()->Value());
            // if(std::string(node->Name()).compare("cond5") == 0)
            // {
            //     std::cout << "\nprev_node_text: <" << prev_node_text << "> length: "<< prev_node_text.length() <<"\n";
            //     std::cout << "next_node_text: <" << next_node_text << "> length: "<< next_node_text.length() <<"\n\n";
            // }

            if(prev_node_text.length() < 2 || next_node_text.length() < 1)
                return false;
            prev_node_text = prev_node_text.substr(prev_node_text.length()-2, 2);
            next_node_text = next_node_text.substr(0, 1);
            // if(std::string(node->Name()).compare("cond5") == 0)
            // {
            //     std::cout << "\nprev_node_text: <" << prev_node_text << "> length: "<< prev_node_text.length() <<"\n";
            //     std::cout << "next_node_text: <" << next_node_text << "> length: "<< next_node_text.length() <<"\n\n";
            // }
            if(prev_node_text.compare("${") == 0 && next_node_text.compare("}") == 0)
            {
                return true;
            }
        }
        return false;
    }


}
