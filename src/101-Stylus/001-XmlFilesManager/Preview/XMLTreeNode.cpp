#include "101-Stylus/001-XmlFilesManager/Preview/XMLTreeNode.h"
#include <Wt/WText.h>
#include <Wt/WApplication.h>

namespace Stylus
{

    XMLTreeNode::XMLTreeNode(std::shared_ptr<XMLFileBrain> file_brain, tinyxml2::XMLElement* node, bool scroll_into_view)
    : Wt::WContainerWidget(), node_(node), file_brain_(file_brain)
    {
        setStyleClass("flex flex-col text-sm");
        // acceptDrops("file", "Wt-item");
        // recive error if given a messages tag name to an element

        label_wrapper_ = addWidget(std::make_unique<Wt::WContainerWidget>());
        content_wrapper_ = addWidget(std::make_unique<Wt::WContainerWidget>());

        label_wrapper_->setStyleClass("flex space-x-2 truncate rounded-md mr-[3px] cursor-pointer overflow-visible");
        content_wrapper_->setStyleClass("flex flex-col ml-[10px]");

        auto tag_name = label_wrapper_->addWidget(std::make_unique<Wt::WText>(node->Name()));
        tag_name->setStyleClass("font-medium pl-[5px]");

        if(std::string(node->Name()).compare("messages") == 0){
            tag_name->addStyleClass("preview-messages-node");
        }else if(std::string(node->Name()).compare("message") == 0){
            tag_name->addStyleClass("preview-message-node");
            if(node->Attribute("id")){
                auto id = node->Attribute("id");
                auto id_text = label_wrapper_->addWidget(std::make_unique<Wt::WText>(id));
                id_text->setStyleClass("truncate italic font-light text-[#ff0000] text-bold select-none");
                id_text->setStyleClass("preview-message-node-id");
            }else {
                auto id_text = label_wrapper_->addWidget(std::make_unique<Wt::WText>("No ID"));
                id_text->setStyleClass("truncate italic font-light text-[#ff0000] text-bold select-none");
                id_text->setStyleClass("preview-message-node-id");
            }
        }else if(file_brain_->state_->isCondNode(node)){
            // std::cout << "\n\nCondition node\n";
                tag_name->addStyleClass("preview-condition-node");
                Wt::WText* condition_switcher; 
                if(node->BoolAttribute("true")){
                    condition_switcher = label_wrapper_->addWidget(std::make_unique<Wt::WText>("true"));
                    condition_switcher->setStyleClass("outline-[#4ade80]/70 bg-[#ecfdf5]/10");
                }else {
                    condition_switcher = label_wrapper_->addWidget(std::make_unique<Wt::WText>("false"));
                    condition_switcher->setStyleClass("outline-[#ef4444]/70 bg-[#fef2f2]/10");
                }
                condition_switcher->addStyleClass("truncate px-[3px] text-[12px] italic font-light rounded-[30%] outline-1 outline-solid");
                condition_switcher->clicked().preventPropagation();
                condition_switcher->clicked().connect(this, [=]()
                {
                    if(node->BoolAttribute("true")){
                        node->DeleteAttribute("true");
                    }else {
                        node->SetAttribute("true", true);
                    }
                    file_brain_->doc_->SaveFile(file_brain_->file_path_.c_str());
                    file_brain_->file_saved_.emit();
                });
        } else {
            tag_name->addStyleClass("preview-tree-node");

        }

        if(node_ == file_brain_->selected_node_)
        {
            label_wrapper_->addStyleClass("selected-xml-tree-node");
            if(scroll_into_view){
                label_wrapper_->doJavaScript(label_wrapper_->jsRef() + ".scrollIntoView({ behavior: 'smooth', block: 'center' });");
            }
        }

        label_wrapper_->mouseWentUp().connect(this, [=](const Wt::WMouseEvent& event)
        {
            if (event.button() == Wt::MouseButton::Right) {
                showPopup(event);
            }
        });

        label_wrapper_->clicked().connect(this, [=]()
        {
            file_brain_->xml_node_selected_.emit(node_, false);
        });
        label_wrapper_->mouseWentOver().preventPropagation();
        label_wrapper_->mouseWentOver().connect(this, [=]()
        {
            label_wrapper_->toggleStyleClass("selected-xml-tree-node-hover", true, true);
        });
        label_wrapper_->mouseWentOut().connect(this, [=]()
        {
            label_wrapper_->toggleStyleClass("selected-xml-tree-node-hover", false, true);
        });
        auto child_node = node->FirstChild();
        while (child_node) {
            if (child_node->ToElement()) {
                content_wrapper_->addWidget(std::make_unique<XMLTreeNode>(file_brain, child_node->ToElement(), scroll_into_view));
            } else if (child_node->ToText()) {
                if(file_brain_->state_->trimAllWitespace(child_node->ToText()->Value()).compare("${") == 0 && child_node->NextSiblingElement() &&
                    file_brain_->state_->isCondNode(child_node->NextSiblingElement()))
                {
                    // text outside the condition ${ 
                    // content_wrapper_->addWidget(std::make_unique<Wt::WText>(child_node->ToText()->Value()))->setStyleClass("preview-condition-node inline-block hover:bg-gray-400 p-1");
                }else if(file_brain_->state_->trimAllWitespace(child_node->ToText()->Value()).compare("}") == 0 && child_node->PreviousSiblingElement() &&
                    file_brain_->state_->isCondNode(child_node->PreviousSiblingElement()))
                {
                    // text outside the condition }
                    // content_wrapper_->addWidget(std::make_unique<Wt::WText>(child_node->ToText()->Value()))->setStyleClass("preview-condition-node inline-block hover:bg-gray-400 p-1");
                }else if(file_brain_->state_->trimAllWitespace(child_node->ToText()->Value()).compare("}") == 0 && 
                    file_brain_->state_->isCondNode(child_node->Parent()->ToElement()))
                {
                    // text inside the condition }
                    // content_wrapper_->addWidget(std::make_unique<Wt::WText>(child_node->ToText()->Value()))->setStyleClass("preview-condition-node inline-block hover:bg-gray-400 p-1");
                }else if(file_brain_->state_->trimAllWitespace(child_node->ToText()->Value()).compare("${") == 0 &&
                    file_brain_->state_->isCondNode(child_node->Parent()->ToElement()))
                {
                    // text inside the condition ${ 
                    // content_wrapper_->addWidget(std::make_unique<Wt::WText>(child_node->ToText()->Value()))->setStyleClass("preview-condition-node inline-block hover:bg-gray-400 p-1");
                }else if(child_node->PreviousSiblingElement() || child_node->NextSiblingElement())
                {                    
                    // text outside the condition and between elements (Wrongly placed and not sigle paranted)
                    auto text_node = content_wrapper_->addWidget(std::make_unique<Wt::WText>(child_node->ToText()->Value()));
                    text_node->setStyleClass("preview-error-text");
                }else
                {
                    auto text_node = label_wrapper_->addWidget(std::make_unique<Wt::WText>(child_node->ToText()->Value(), Wt::TextFormat::Plain));
                    // auto text_node = content_wrapper_->addWidget(std::make_unique<Wt::WText>(child_node->ToText()->Value()));
                    text_node->setStyleClass("select-none preview-tree-node-text");
                }
            }
            child_node = child_node->NextSibling();
        }
    }


    void XMLTreeNode::showPopup(const Wt::WMouseEvent& event)
    {
        if (!popup_) {
            popup_ = std::make_unique<Wt::WPopupMenu>();

            
            popup_->addItem("test dose nothing")->clicked();
            // auto test_menu_item_1 = popup_->addItem("Copy Node")->clicked().connect(this, [=](){ 
            //     file_brain_->state_->setCopyNode(node_);
            // });
            // popup_->addSeparator();
            // auto test_menu_item_2 = popup_->addItem("Paste Node")->clicked().connect(this, [=](){ 
            //     auto copied_node = file_brain_->state_->copy_node_;
            //     if(!copied_node)
            //         return;
            //     auto new_node = copied_node->DeepClone(file_brain_->doc_.get());
            //     if(node_->FirstChild() && node_->FirstChild()->ToText())
            //     {
                    
            //     }
            // });
          
        }

        if (popup_->isHidden())
            popup_->popup(event);
        else
            popup_->hide();
    }


}
