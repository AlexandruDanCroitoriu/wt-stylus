#include "101-Stylus/001-XmlFilesManager/XmlFileBrain.h"
#include <iostream>

namespace Stylus {

    XmlFileBrain::XmlFileBrain(std::string file_path)
    : file_path_(file_path)
    {
        // Load the XML file
        tinyxml2::XMLError eResult = doc_.LoadFile(file_path.c_str());
        if (eResult != tinyxml2::XML_SUCCESS) {
            // Handle error
            std::cout << "\n\nError loading XML file: " << doc_.ErrorIDToName(eResult) << "\n\n";
        }

    }

    // derived tree node
    XmlTreeNode::XmlTreeNode(std::shared_ptr<XmlFileBrain> xml_file_brain, tinyxml2::XMLNode* node)
    : Wt::WTreeNode(""), node_(node), xml_file_brain_(xml_file_brain)
    {
        label_wrapper_ = labelArea();
        label_wrapper_->setStyleClass("flex items-center");
        // setSelectable(true);
        // acceptDrops("file", "Wt-item");

        if(node->ToElement() != nullptr)
        {
            label()->setText(node->ToElement()->Name());
        }else if(node->ToText() != nullptr)
        {
            label()->setText(node->ToText()->Value());
        }

        auto first_child = node->FirstChild();
        while (first_child) {
            auto child_node = std::make_unique<XmlTreeNode>(xml_file_brain, first_child);
            addChildNode(std::move(child_node));
            first_child = first_child->NextSibling();
        }

    }
    void XmlTreeNode::dropEvent(Wt::WDropEvent event)
    {
        std::cout << "Drop event on XML tree node: " << event.mimeType() << "\n";
    }

    // derived element node
    XmlElemNode::XmlElemNode(std::shared_ptr<XmlFileBrain> xml_file_brain, tinyxml2::XMLElement* node)
    : Wt::WContainerWidget(), node_(node), xml_file_brain_(xml_file_brain)
    {
        // acceptDrops("file", "Wt-item");
        setHtmlTagName(node->Name());
        setStyleClass(node_->Attribute("class"));

        auto first_child = node->FirstChild();
        while (first_child) {
            if (first_child->ToElement()) {
                auto child_node = std::make_unique<XmlElemNode>(xml_file_brain, first_child->ToElement());
                addWidget(std::move(child_node));
            } else if (first_child->ToText()) {
                auto text_node = std::make_unique<XmlTextNode>(xml_file_brain, first_child->ToText());
                addWidget(std::move(text_node));
            }
            first_child = first_child->NextSibling();
        }
    }

    void XmlElemNode::dropEvent(Wt::WDropEvent event)
    {
        std::cout << "Drop event on XML element node: " << event.mimeType() << "\n";
    }

    // derived text node
    XmlTextNode::XmlTextNode(std::shared_ptr<XmlFileBrain> xml_file_brain, tinyxml2::XMLText* node)
    : Wt::WText(node->Value()), node_(node), xml_file_brain_(xml_file_brain)
    {
    }


    XmlFileUi::XmlFileUi()
    {
        auto grid_layout = std::make_unique<Wt::WGridLayout>();
        grid_layout_ = grid_layout.get();
        setLayout(std::move(grid_layout));        
    }

    void XmlFileUi::setFile(std::string file_path)
    {
        xml_file_brain_ = std::make_shared<XmlFileBrain>(file_path);
        
    }


}