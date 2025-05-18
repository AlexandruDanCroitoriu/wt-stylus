#include "101-Stylus/001-XmlFilesManager/Preview/XMLElemNode.h"
#include <Wt/WText.h>
#include <Wt/WApplication.h>

namespace Stylus
{

    XMLElemNode::XMLElemNode(std::shared_ptr<XMLFileBrain> file_brain, tinyxml2::XMLElement* node, bool scroll_into_view)
    : Wt::WContainerWidget(), node_(node), file_brain_(file_brain)
    {
        // acceptDrops("file", "Wt-item");
        // recive error if given a messages tag name to an element
        if(std::string(node->Name()).compare("messages") != 0 &&
            std::string(node->Name()).compare("message") != 0){
            setHtmlTagName(node->Name());
        }
        for(auto attr = node->FirstAttribute(); attr != nullptr; attr = attr->Next()) {
            if(std::string(attr->Name()).compare("class") == 0)
            {
                setStyleClass(attr->Value());
            }else {
                setAttributeValue(attr->Name(), attr->Value());
            }
        }
        if(node_ == file_brain_->selected_node_)
        {
            addStyleClass("selected-xml-node");
            if(scroll_into_view){
                // doJavaScript(jsRef() + ".scrollIntoView({ behavior: 'smooth', block: 'center' });");
                doJavaScript(jsRef() + ".scrollIntoView({ block: 'center' });");
                // doJavaScript(jsRef() + ".scrollIntoView({ behavior: 'smooth', block: 'center' });");
                
            }
            
        }

        clicked().connect(this, [=]()
        {
            file_brain_->xml_node_selected_.emit(node_, false);
        });
        mouseWentOver().preventPropagation();
        mouseWentOver().connect(this, [=]()
        {
            toggleStyleClass("selected-xml-node-hover", true, true);
        });
        mouseWentOut().connect(this, [=]()
        {
            toggleStyleClass("selected-xml-node-hover", false, true);
        });
        if(node->PreviousSibling() && node->PreviousSibling()->ToText() &&
            node->NextSibling() && node->NextSibling()->ToText()
        ){
            std::string prev_node_text = node->PreviousSibling()->ToText()->Value();
            std::string next_node_text = node->NextSibling()->ToText()->Value();   
            
            //remove whitespace 
            prev_node_text.erase(remove_if(prev_node_text.begin(), prev_node_text.end(), isspace), prev_node_text.end());
            next_node_text.erase(remove_if(next_node_text.begin(), next_node_text.end(), isspace), next_node_text.end());

            if(prev_node_text.compare("${") == 0 && next_node_text.compare("}") == 0 &&
                node->BoolAttribute("true") == false){
                    return;   
            }
        }
        auto child_node = node->FirstChild();
        while (child_node) {
            if (child_node->ToElement()) {
                auto tree_child_node = addWidget(std::make_unique<XMLElemNode>(file_brain, child_node->ToElement(), scroll_into_view));
            } else if (child_node->ToText()) {
                std::string child_text = child_node->ToText()->Value();
                // remove whitespace
                std::string child_text_nowitespace = child_text;
                child_text_nowitespace.erase(remove_if(child_text_nowitespace.begin(), child_text_nowitespace.end(), isspace), child_text_nowitespace.end());
                // std::cout << "\n\nText content: <" << child_text << "> \n";
                
                if((child_node->PreviousSiblingElement() || child_node->NextSiblingElement()) &&
                    (child_text_nowitespace.compare("}") == 0 || child_text_nowitespace.compare("${") == 0)){
                        // std::cout << "\n\n text node of start condition: <" << child_text << ">\n";
                }else if(child_node->PreviousSiblingElement() || child_node->NextSiblingElement()) {
                    auto text_node = addWidget(std::make_unique<Wt::WText>(child_node->ToText()->Value()));
                    text_node->setStyleClass("font-bold text-[#ff0000] outline-2 outline-[#ff0000] rounded-md p-[2px] hover:bg-[#ff0000]/30 cursor-pointer");
                    text_node->clicked().preventPropagation();
                    text_node->clicked().connect(this, [=](const Wt::WMouseEvent& event)
                    {
                        auto new_node = file_brain_->doc_->NewElement("span");
                        auto parent_node = child_node->Parent();
                        auto prev_node = child_node->PreviousSibling();

                        std::string text = child_node->ToText()->Value();
                        // remove whitespace from start and end of the string
                        text.erase(0, text.find_first_not_of(" \t\n\r\f\v"));
                        text.erase(text.find_last_not_of(" \t\n\r\f\v") + 1);
                        if(prev_node && parent_node)
                        {
                            parent_node->InsertAfterChild(prev_node, new_node);
                        }else {
                            parent_node->InsertFirstChild(new_node);
                        }
                        new_node->SetText(text.c_str());
                        file_brain_->doc_->DeleteNode(child_node);
                        file_brain_->doc_->SaveFile(file_brain_->file_path_.c_str());
                        file_brain_->file_saved_.emit();
                        file_brain_->xml_node_selected_.emit(new_node, true);
                    });
                }else {
                    auto text_node = addWidget(std::make_unique<Wt::WText>(child_node->ToText()->Value()));
                }
            }
            child_node = child_node->NextSibling();
            
        }
    }

}
