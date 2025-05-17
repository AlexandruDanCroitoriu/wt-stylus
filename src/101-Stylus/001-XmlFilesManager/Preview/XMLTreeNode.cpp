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

        label_wrapper_->setStyleClass("flex space-x-2 truncate rounded-md mr-[3px] cursor-pointer");
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
        }else {
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
        auto first_child = node->FirstChild();
        while (first_child) {
            if (first_child->ToElement()) {
                auto child_node = std::make_unique<XMLTreeNode>(file_brain, first_child->ToElement(), scroll_into_view);
                content_wrapper_->addWidget(std::move(child_node));
            } else if (first_child->ToText()) {
                
                if(first_child->PreviousSiblingElement() || first_child->NextSiblingElement()){
                    auto text_node = content_wrapper_->addWidget(std::make_unique<Wt::WText>(first_child->ToText()->Value()));
                    text_node->setStyleClass("truncate italic font-light text-[#ff0000] text-bold select-none");
                }else {
                    auto text_node = label_wrapper_->addWidget(std::make_unique<Wt::WText>(first_child->ToText()->Value()));
                    text_node->setStyleClass("select-none preview-tree-node-text");
                }
            }
            first_child = first_child->NextSibling();
        }
    }


    void XMLTreeNode::showPopup(const Wt::WMouseEvent& event)
    {
        if (!popup_) {
            popup_ = std::make_unique<Wt::WPopupMenu>();

            
            // popup_->addItem("Create New File")->clicked().connect(this, [=](){ createNewFileDialog(); });
            auto test_menu_item_1 = popup_->addItem("Copy Node")->clicked().connect(this, [=](){ 
                file_brain_->state_->setCopyNode(node_);
            });
            popup_->addSeparator();
            auto test_menu_item_2 = popup_->addItem("Paste Node")->clicked().connect(this, [=](){ 
                auto copied_node = file_brain_->state_->copy_node_;
                if(!copied_node)
                    return;
                auto new_node = copied_node->DeepClone(file_brain_->doc_.get());
                if(node_->FirstChild() && node_->FirstChild()->ToText())
                {
                    
                }
            });
          
        }

        if (popup_->isHidden())
            popup_->popup(event);
        else
            popup_->hide();
    }


}
