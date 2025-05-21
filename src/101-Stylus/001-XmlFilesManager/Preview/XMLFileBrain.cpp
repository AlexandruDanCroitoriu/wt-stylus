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
        // std::cout << "\n\nFile loaded: " << file_path_ << "\n\n";
        // organizeXmlNode(selected_node_);
        // if(file_path_.compare("../static/stylus-resources/xml/000-examples/test-tree") == 0){
        //     // save file
        //     doc_->SaveFile(std::string("../test.xml").c_str());
        // }
        xml_node_selected_.emit(selected_node_, false);
        
    }

    void XMLFileBrain::organizeXmlNode(tinyxml2::XMLElement* node)
    {
        if(node != node->GetDocument()->RootElement())
        {
            if(isCondNode(node))
            {
                std::cout << "\n\nCondition node\n";
                if(node->PreviousSibling() && node->PreviousSibling()->ToText() && trimAllWitespace(node->PreviousSibling()->ToText()->Value()).compare("${") != 0)
                {
                    std::cout << "\n\n selected previous sibling check \n\n";
                    std::string text = node->PreviousSibling()->ToText()->Value();
                    std::string before_start_cond = text.substr(0, text.length()-2); 
                    std::string after_start_cond = text.substr(text.length()-2, 2);
                    std::cout << "Splitting previous sibling text: '" << trimAllWitespace(text) << "' before_start_cond '" << trimAllWitespace(before_start_cond) << "' after_start_cond '" << trimAllWitespace(after_start_cond) << "'\n";
                    node->PreviousSibling()->ToText()->SetValue(before_start_cond.c_str());
                    auto parent_node = node->Parent();
                    parent_node->InsertAfterChild(node->PreviousSibling(), doc_->NewText(after_start_cond.c_str()));                    
                }
                if(node->NextSibling() && node->NextSibling()->ToText() && trimAllWitespace(node->NextSibling()->ToText()->Value()).compare("}") != 0)
                {
                    std::cout << "\n\n selected next sibling check \n\n";
                    std::string text = node->NextSibling()->ToText()->Value();
                    std::string after_cond_text = text.substr(0, 1);
                    std::string after_after_cont_text = text.substr(1, text.length()-1);
                    std::cout << "Splitting next sibling text: '" << trimAllWitespace(text) << "' after_cond_text '" << trimAllWitespace(after_cond_text) << "' after_after_cont_text '" << trimAllWitespace(after_after_cont_text) << "'\n";
                    node->NextSibling()->ToText()->SetValue(after_cond_text.c_str());
                    auto parent_node = node->Parent();
                    parent_node->InsertAfterChild(node->NextSibling(), doc_->NewText(after_after_cont_text.c_str()));
                }
                if(node->FirstChild() && node->FirstChild()->ToText() && trimAllWitespace(node->FirstChild()->ToText()->Value()).compare("}") != 0)
                {
                    std::cout << "\n\n selected first child check \n\n";
                    std::string text = node->FirstChild()->ToText()->Value();
                    std::string first_child_text = text.substr(0, 1);
                    std::string after_first_child_text = text.substr(1, text.length()-1);
                    std::cout << "Splitting first child text:'" << trimAllWitespace(text) << "' \nfirst_child_text '" << trimAllWitespace(first_child_text) << "' \nafter_first_child_text '" << trimAllWitespace(after_first_child_text) << "'\n";
                    node->FirstChild()->ToText()->SetValue(first_child_text.c_str());
                    node->InsertAfterChild(node->FirstChild(), doc_->NewText(after_first_child_text.c_str()));
                }
                if(node->LastChild() && node->LastChild()->ToText() && trimAllWitespace(node->LastChild()->ToText()->Value()).compare("${") != 0)
                {
                    std::cout << "\n\n selected last child check \n\n";
                    std::string text = node->LastChild()->ToText()->Value();
                    std::string last_child_text = text.substr(text.length()-2, 2);
                    std::string before_end_cond = text.substr(0, text.length()-2);
                    std::cout << "Splitting last child text: '" << trimAllWitespace(text) << "' before_end_cond '" << trimAllWitespace(before_end_cond) << "' last_child_text '" << trimAllWitespace(last_child_text) << "'\n";
                    node->LastChild()->ToText()->SetValue(last_child_text.c_str());
                    node->InsertAfterChild(node->LastChild()->PreviousSibling(), doc_->NewText(before_end_cond.c_str()));
                }
            }
        }
        auto child_node = node->FirstChild();
        while (child_node) {
            if (child_node->ToElement()) {
                organizeXmlNode(child_node->ToElement());
            } else if (child_node->ToText()) {
            }
            child_node = child_node->NextSibling();
        }
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
