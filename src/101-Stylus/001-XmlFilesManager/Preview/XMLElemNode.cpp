#include "101-Stylus/001-XmlFilesManager/Preview/XMLElemNode.h"
#include "101-Stylus/001-XmlFilesManager/Preview/XMLTreeNode.h"
#include <Wt/WText.h>
#include <Wt/WApplication.h>
#include <Wt/WTemplate.h>

namespace Stylus
{

    XMLElemNode::XMLElemNode(std::shared_ptr<XMLFileBrain> file_brain, tinyxml2::XMLElement *node, bool scroll_into_view)
        : Wt::WContainerWidget(), node_(node), file_brain_(file_brain)
    {
        // acceptDrops("file", "Wt-item");
        // recive error if given a messages tag name to an element
        if (std::string(node->Name()).compare("messages") != 0 &&
            std::string(node->Name()).compare("message") != 0)
        {
            setHtmlTagName(node->Name());
        }
        for (auto attr = node->FirstAttribute(); attr != nullptr; attr = attr->Next())
        {
            if (std::string(attr->Name()).compare("class") == 0)
            {
                setStyleClass(attr->Value());
            }else if(std::string(attr->Name()).compare("id") != 0)
            {
                setAttributeValue(attr->Name(), attr->Value());
            }
        }
        if (node_ == file_brain_->selected_node_)
        {
            addStyleClass("selected-xml-node");
            if (scroll_into_view)
            {
                // doJavaScript(jsRef() + ".scrollIntoView({ behavior: 'smooth', block: 'center' });");
                doJavaScript(jsRef() + ".scrollIntoView({ block: 'center' });");
            }
        }

        clicked().connect(this, [=]()
                          { if(file_brain_->selected_node_ != node_) file_brain_->xml_node_selected_.emit(node_, false); });
        mouseWentOver().preventPropagation();
        mouseWentOver().connect(this, [=]()
                                { toggleStyleClass("selected-xml-node-hover", true, true); });
        mouseWentOut().connect(this, [=]()
                               { toggleStyleClass("selected-xml-node-hover", false, true); });


        auto child_node = node_->FirstChild();
        while (child_node)
        {
            if(child_node->ToElement())
            {
                addElemNode(child_node->ToElement(), scroll_into_view);
            }else if(child_node->ToText())
            {
                addTextNode(child_node->ToText());
            }
            // else {
            //     addWidget(std::make_unique<Wt::WText>("This node is not text or element nodes, WHAT DID YOU DO ?"))->setStyleClass("font-bold text-[#ff0000] outline-2 outline-[#ff0000] rounded-md p-[2px] hover:bg-[#ff0000]/30 cursor-pointer");
            // }
            
            child_node = child_node->NextSibling();
        }
    }

    void XMLElemNode::addElemNode(tinyxml2::XMLElement* elem_node, bool scroll_into_view)
    {
        if(file_brain_->state_->isCondNode(elem_node))
        {
            if(!elem_node->BoolAttribute("true"))
            {
                auto first_child_inside_cond = elem_node->FirstChild();
                while(first_child_inside_cond)
                {
                    if(first_child_inside_cond->ToText())
                    {
                        addTextNode(first_child_inside_cond->ToText());
                    }else if(first_child_inside_cond->ToElement())
                    {
                        addElemNode(first_child_inside_cond->ToElement(), scroll_into_view);
                    }else {
                        addWidget(std::make_unique<Wt::WText>("This node is not text or element nodes, WHAT DID YOU DO ?"))->setStyleClass("font-bold text-[#ff0000] outline-2 outline-[#ff0000] rounded-md p-[2px] hover:bg-[#ff0000]/30 cursor-pointer");
                    }
                    first_child_inside_cond = first_child_inside_cond->NextSibling();
                }
            }
        }else if(std::string(elem_node->Name()).compare("template") == 0){
            auto first_child_inside_temp = elem_node->FirstChild();
            while(first_child_inside_temp)
            {
                if(first_child_inside_temp->ToText())
                {
                    addTextNode(first_child_inside_temp->ToText());
                }else if(first_child_inside_temp->ToElement())
                {
                    addElemNode(first_child_inside_temp->ToElement(), scroll_into_view);
                }else {
                    addWidget(std::make_unique<Wt::WText>("This node is not text or element nodes, WHAT DID YOU DO ?"))->setStyleClass("font-bold text-[#ff0000] outline-2 outline-[#ff0000] rounded-md p-[2px] hover:bg-[#ff0000]/30 cursor-pointer");
                }
                first_child_inside_temp = first_child_inside_temp->NextSibling();
            }
        }else{
            addWidget(std::make_unique<XMLElemNode>(file_brain_, elem_node, scroll_into_view));
        }
    }



    void XMLElemNode::addTextNode(tinyxml2::XMLText* text_node)
    {
        if(XMLTreeNode::trimAllWitespace(text_node->ToText()->Value()).compare("${") == 0 && text_node->NextSiblingElement() &&
            file_brain_->state_->isCondNode(text_node->NextSiblingElement()))
        {
            // text outside the condition ${ 
            // addWidget(std::make_unique<Wt::WText>(text_node->Value()))->setStyleClass("preview-condition-node");
        }else if(XMLTreeNode::trimAllWitespace(text_node->ToText()->Value()).compare("}") == 0 && text_node->PreviousSiblingElement() &&
            file_brain_->state_->isCondNode(text_node->PreviousSiblingElement()))
        {
            // text outside the condition }
            // addWidget(std::make_unique<Wt::WText>(text_node->Value()))->setStyleClass("preview-condition-node");
        }else if(XMLTreeNode::trimAllWitespace(text_node->ToText()->Value()).compare("}") == 0 && 
            file_brain_->state_->isCondNode(text_node->Parent()->ToElement()))
        {
            // text inside the condition }
            // addWidget(std::make_unique<Wt::WText>(text_node->Value()))->setStyleClass("preview-condition-node");
        }else if(XMLTreeNode::trimAllWitespace(text_node->ToText()->Value()).compare("${") == 0 &&
            file_brain_->state_->isCondNode(text_node->Parent()->ToElement()))
        {
            // text inside the condition ${ 
            // addWidget(std::make_unique<Wt::WText>(text_node->Value()))->setStyleClass("preview-condition-node");
        }else if(text_node->PreviousSiblingElement() || text_node->NextSiblingElement())
        {
            if(file_brain_->state_->isCondNode(text_node->Parent()->ToElement()) && text_node->Parent()->ToElement()->BoolAttribute("true"))
            {
                auto error_text = addWidget(std::make_unique<Wt::WText>(text_node->Value()));
                error_text->setStyleClass("preview-error-text outline-2 outline-[#ff0000] rounded-md p-[2px] hover:bg-[#ff0000]/30 cursor-pointer");
                error_text->mouseWentUp().connect(this, [=](const Wt::WMouseEvent& event)
                {
                    if (event.button() == Wt::MouseButton::Right) {
                        
                        auto parent_node = text_node->Parent();
                        auto new_span = file_brain_->doc_->NewElement("span");
                        if(text_node->PreviousSibling()){
                            parent_node->InsertAfterChild(text_node->PreviousSibling(), new_span);
                        }else {
                            parent_node->InsertFirstChild(new_span);
                        }
                        new_span->InsertFirstChild(text_node);
                        file_brain_->doc_->SaveFile(file_brain_->file_path_.c_str());
                        file_brain_->file_saved_.emit();
                    }
                });
            }
        }else if(file_brain_->state_->getTempNodeVarData(text_node->Parent()->ToElement()).var_name_.compare("") != 0){
            std::cout << "Adding variable holder for " << text_node->Parent()->ToElement()->Name() << std::endl;
            auto temp_var_data = file_brain_->state_->getTempNodeVarData(text_node->Parent()->ToElement());
            auto message_attr = TempNodeVarData::getMessageAttributeData(temp_var_data.attributes_["message_path"]);
            if(temp_var_data.function_.compare("tr") == 0 ){
                if(message_attr.folder_name_.compare("") != 0 && message_attr.file_name_.compare("") != 0)
                {
                    auto message_file_brain = file_brain_->state_->xml_file_brains_[message_attr.folder_name_ + "/" + message_attr.file_name_];
                    if(message_file_brain)
                    {
                        auto message_node = message_file_brain->id_and_message_nodes_[temp_var_data.var_name_];
                        if(message_node){
                            std::cout << "Adding message node for " << message_attr.folder_name_ << "/" << message_attr.file_name_ << "/" << temp_var_data.var_name_ << std::endl;
                            auto message_child = message_node->FirstChild();
                            while(message_child)
                            {
                                if(message_child->ToText())
                                {
                                    addTextNode(message_child->ToText());
                                }else if(message_child->ToElement())
                                {
                                    addElemNode(message_child->ToElement(), false);
                                }
                            message_child = message_child->NextSibling();                                   
                            }
                        }else 
                        {
                            std::cout << "Message node not found for " << message_attr.folder_name_ << "/" << message_attr.file_name_ << "/" << temp_var_data.var_name_ << std::endl;
                            addWidget(std::make_unique<Wt::WText>(temp_var_data.var_name_ + " " + message_attr.folder_name_ + "/" + message_attr.file_name_))->setStyleClass("preview-error-text outline-2 outline-[#ff0000] rounded-md p-[2px] hover:bg-[#ff0000]/30 cursor-pointer");
                        }  
                    }else {
                        std::cout << "Message file brain not found for " << message_attr.folder_name_ << "/" << message_attr.file_name_ << std::endl;
                        addWidget(std::make_unique<Wt::WText>(temp_var_data.var_name_ + " " + message_attr.folder_name_ + "/" + message_attr.file_name_))->setStyleClass("preview-error-text outline-2 outline-[#ff0000] rounded-md p-[2px] hover:bg-[#ff0000]/30 cursor-pointer");
                        
                    }
                }else {
                    std::cout << "Message attribute data is not valid template variable: " << temp_var_data.var_name_ << std::endl;
                    addWidget(std::make_unique<Wt::WText>(temp_var_data.var_name_ + " | message_path attribute missing or incorect"))->setStyleClass("preview-error-text outline-2 outline-[#ff0000] rounded-md p-[2px] hover:bg-[#ff0000]/30 cursor-pointer");
                }
            }else {
                std::cout << "add variable text as is for " << text_node->Value() << std::endl;
                auto var_name = addWidget(std::make_unique<Wt::WText>(text_node->Value()));                
            }
        }else {
            if(file_brain_->state_->isCondNode(text_node->Parent()->ToElement()))
            {
                if(text_node->Parent()->ToElement()->BoolAttribute("true")){
                    addWidget(std::make_unique<Wt::WText>(text_node->Value(), Wt::TextFormat::Plain))->setStyleClass("text-node");
                }
            }else {
                    addWidget(std::make_unique<Wt::WText>(text_node->Value(), Wt::TextFormat::Plain))->setStyleClass("text-node");
            }
        }
    }

}