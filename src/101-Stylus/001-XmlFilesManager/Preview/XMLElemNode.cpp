#include "101-Stylus/001-XmlFilesManager/Preview/XMLElemNode.h"
#include <Wt/WText.h>
#include <Wt/WApplication.h>

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
            }
            else
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
                // doJavaScript(jsRef() + ".scrollIntoView({ behavior: 'smooth', block: 'center' });");
            }
        }

        clicked().connect(this, [=]()
                          { file_brain_->xml_node_selected_.emit(node_, false); });
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
                if(file_brain_->state_->isCondNode(child_node->ToElement()))
                {
                    if(child_node->ToElement()->BoolAttribute("true"))
                    {
                        auto first_child_inside_cond = child_node->FirstChild();
                        while(first_child_inside_cond)
                        {
                            if(first_child_inside_cond->ToText())
                            {
                                addTextNode(first_child_inside_cond->ToText());
                            }else if(first_child_inside_cond->ToElement())
                            {
                                // addChildNode(first_child_inside_cond, scroll_into_view);
                                addWidget(std::make_unique<XMLElemNode>(file_brain_, first_child_inside_cond->ToElement(), scroll_into_view));
                            }else {
                                addWidget(std::make_unique<Wt::WText>("This node is not text or element nodes, WHAT DID YOU DO ?"))->setStyleClass("font-bold text-[#ff0000] outline-2 outline-[#ff0000] rounded-md p-[2px] hover:bg-[#ff0000]/30 cursor-pointer");
                            }
                            first_child_inside_cond = first_child_inside_cond->NextSibling();
                        }
                    }
                }else {
                    addWidget(std::make_unique<XMLElemNode>(file_brain_, child_node->ToElement(), scroll_into_view));
                }
            }else if(child_node->ToText())
            {
                addTextNode(child_node->ToText());
                // std::cout << "\n node text : " << child_node->ToText()->Value();
            }else {
                addWidget(std::make_unique<Wt::WText>("This node is not text or element nodes, WHAT DID YOU DO ?"))->setStyleClass("font-bold text-[#ff0000] outline-2 outline-[#ff0000] rounded-md p-[2px] hover:bg-[#ff0000]/30 cursor-pointer");
            }
            
            child_node = child_node->NextSibling();
        }
    }

    void XMLElemNode::addTextNode(tinyxml2::XMLText* text_node)
    {
        if(file_brain_->state_->trimAllWitespace(text_node->ToText()->Value()).compare("${") == 0 && text_node->NextSiblingElement() &&
            file_brain_->state_->isCondNode(text_node->NextSiblingElement()))
        {
            // text outside the condition ${ 
            // addWidget(std::make_unique<Wt::WText>(text_node->Value()))->setStyleClass("preview-condition-node");
        }else if(file_brain_->state_->trimAllWitespace(text_node->ToText()->Value()).compare("}") == 0 && text_node->PreviousSiblingElement() &&
            file_brain_->state_->isCondNode(text_node->PreviousSiblingElement()))
        {
            // text outside the condition }
            // addWidget(std::make_unique<Wt::WText>(text_node->Value()))->setStyleClass("preview-condition-node");
        }else if(file_brain_->state_->trimAllWitespace(text_node->ToText()->Value()).compare("}") == 0 && 
            file_brain_->state_->isCondNode(text_node->Parent()->ToElement()))
        {
            // text inside the condition }
            // addWidget(std::make_unique<Wt::WText>(text_node->Value()))->setStyleClass("preview-condition-node");
        }else if(file_brain_->state_->trimAllWitespace(text_node->ToText()->Value()).compare("${") == 0 &&
            file_brain_->state_->isCondNode(text_node->Parent()->ToElement()))
        {
            // text inside the condition ${ 
            // addWidget(std::make_unique<Wt::WText>(text_node->Value()))->setStyleClass("preview-condition-node");
        }else if(text_node->PreviousSiblingElement() || text_node->NextSiblingElement())
        {
            auto error_text = addWidget(std::make_unique<Wt::WText>(text_node->Value()));
            error_text->setStyleClass("font-bold text-[#ff0000] outline-2 outline-[#ff0000] rounded-md p-[2px] hover:bg-[#ff0000]/30 cursor-pointer");
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
        }else {
            addWidget(std::make_unique<Wt::WText>(text_node->Value()));
        }
    }

}