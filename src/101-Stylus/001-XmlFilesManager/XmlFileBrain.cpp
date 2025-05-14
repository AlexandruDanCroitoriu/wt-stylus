#include "101-Stylus/001-XmlFilesManager/XmlFileBrain.h"
#include <iostream>
#include <Wt/WTemplate.h>

namespace Stylus {

    XmlFileBrain::XmlFileBrain()
    {

        // xml_node_selected_.connect([=](tinyxml2::XMLNode* node)
        // {
        //     selected_node_ = node;
        // });

    }

    void XmlFileBrain::setFile(std::string file_path)
    {
        file_path_ = file_path;
        // Load the XML file
        tinyxml2::XMLError eResult = doc_.LoadFile(file_path.c_str());
        if (eResult != tinyxml2::XML_SUCCESS) {
            std::cout << "\n\nError loading XML file: " << doc_.ErrorIDToName(eResult) << "\n\n";
        }

    }


    // derived tree node
    XmlTreeNode::XmlTreeNode(std::shared_ptr<XmlFileBrain> xml_file_brain, tinyxml2::XMLNode* node)
    : Wt::WTreeNode(""), node_(node), xml_file_brain_(xml_file_brain)
    {
        label_wrapper_ = labelArea();
        // setSelectable(false);
        // acceptDrops("file", "Wt-item");
        // expand();
        

        // if(node == xml_file_brain_->selected_node_)
        // {
        //     addStyleClass("selected-xml-node");
            
        //     // impl()->bindString("selected", "selected-xml-node");
        //     // std::cout << "\n\nSelected node: alkjsdhfkjahgsfjkhdg \n\n";
        // }
       

        if(node->ToElement() != nullptr)
        {
            label()->setText(node->ToElement()->Name());
        }else if (node->ToText() != nullptr)
        {
            label()->setText(node->ToText()->Value());
        }

        // auto first_child = node->FirstChild();
        // while (first_child) {
        //     auto child_node = std::make_unique<XmlTreeNode>(xml_file_brain, first_child);
        //     addChildNode(std::move(child_node));
        //     first_child = first_child->NextSibling();
        // }

    }
    void XmlTreeNode::dropEvent(Wt::WDropEvent event)
    {
        std::cout << "Drop event on XML tree node: " << event.mimeType() << "\n";
    }

    // element node
    XmlElemNode::XmlElemNode(std::shared_ptr<XmlFileBrain> xml_file_brain, tinyxml2::XMLElement* node)
    : Wt::WContainerWidget(), node_(node), xml_file_brain_(xml_file_brain)
    {
        // acceptDrops("file", "Wt-item");
        if(std::string(node->Name()).compare("messages") != 0){
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
        if(node_ == xml_file_brain_->selected_node_)
        {
            addStyleClass("selected-xml-node"); 
        }

        clicked().connect(this, [=]()
        {
            xml_file_brain_->xml_node_selected_.emit(node_);
        });
        mouseWentOver().preventPropagation();
        mouseWentOver().connect(this, [=]()
        {
            toggleStyleClass("preview-xml-node-hover", true, true);
        });
        mouseWentOut().connect(this, [=]()
        {
            toggleStyleClass("preview-xml-node-hover", false, true);
        });
        auto first_child = node->FirstChild();
        while (first_child) {
            if (first_child->ToElement()) {
                auto child_node = std::make_unique<XmlElemNode>(xml_file_brain, first_child->ToElement());
                addWidget(std::move(child_node));
            } else if (first_child->ToText()) {
                addWidget(std::make_unique<Wt::WText>(first_child->ToText()->Value()));
            }
            first_child = first_child->NextSibling();
        }
    }

    void XmlElemNode::dropEvent(Wt::WDropEvent event)
    {
        std::cout << "Drop event on XML element node: " << event.mimeType() << "\n";
    }


    XmlFileUi::XmlFileUi()
    : xml_file_brain_(std::make_shared<XmlFileBrain>())
    {
        auto grid_layout = std::make_unique<Wt::WGridLayout>();
        grid_layout_ = grid_layout.get();
        grid_layout_->setContentsMargins(0, 0, 0, 0);

        auto tree_wrapper = grid_layout_->addWidget(std::make_unique<Wt::WContainerWidget>(), 0, 0);
        tree_ = tree_wrapper->addWidget(std::make_unique<Wt::WTree>());
        preview_wrapper_ = grid_layout_->addWidget(std::make_unique<Wt::WContainerWidget>(), 0, 1);
        
        // tree_->addStyleClass("overflow-y-auto h-screen w-full");
        preview_wrapper_->setStyleClass("overflow-auto h-screen w-full scrollbar-stylus");

        xml_file_brain_->xml_node_selected_.connect(this, [=](tinyxml2::XMLNode* node)
        {
            xml_file_brain_->selected_node_ = node;
            resetUI();
        });
        
        
        setMinimumSize(Wt::WLength::Auto, Wt::WLength(100, Wt::LengthUnit::ViewportHeight));
        tree_wrapper->setStyleClass("overflow-y-auto h-screen scrollbar-stylus");
        tree_->setMinimumSize(Wt::WLength(200, Wt::LengthUnit::Pixel), Wt::WLength(100, Wt::LengthUnit::Percentage));
        tree_->setMaximumSize(Wt::WLength(600, Wt::LengthUnit::Pixel), Wt::WLength(100, Wt::LengthUnit::Percentage));

        grid_layout_->setColumnResizable(0, true, Wt::WLength(200, Wt::LengthUnit::Pixel));
        
        setLayout(std::move(grid_layout)); 
        setStyleClass("w-full h-full");
    }

    void XmlFileUi::setFile(std::string file_path)
    {
        xml_file_brain_->setFile(file_path);
        resetUI();
    }

    void XmlFileUi::resetUI()
    {
        std::cout << "\n\nResetting UI...\n\n";
        preview_wrapper_->clear(); // Clear the preview wrapper

        if(xml_file_brain_->doc_.ErrorID() != tinyxml2::XML_SUCCESS)
        {
            std::cout << "\n\nError loading XML file: " << xml_file_brain_->doc_.ErrorIDToName(xml_file_brain_->doc_.ErrorID()) << "\n\n";
            return;
        }
        auto node = std::make_unique<XmlTreeNode>(xml_file_brain_, xml_file_brain_->doc_.RootElement());
        auto tree_root_node = node.get();
        tree_->setTreeRoot(std::move(node));
        tree_->setSelectionMode(Wt::SelectionMode::Single);
        tree_->treeRoot()->expand();
        // tree_->treeRoot()->label()->setTextFormat(Wt::TextFormat::Plain);
        tree_root_node->setStyleClass("overflow-x-hidden overflow-y-auto h-[1000VH]");

        recursiveAddChild(xml_file_brain_->doc_.RootElement()->FirstChild(), tree_root_node);
        // selected
        for(auto selected_node : tree_->selectedNodes())
        {
            auto selected_tree_node = dynamic_cast<XmlTreeNode*>(selected_node);
            // selected_tree_node->doJavaScript(selected_tree_node->label_wrapper_->jsRef() + ".scrollIntoView({behavior: 'smooth', block: 'center'});");
        }
        preview_wrapper_->addWidget(std::make_unique<XmlElemNode>(xml_file_brain_, xml_file_brain_->doc_.RootElement()));    
    


    }

    void XmlFileUi::recursiveAddChild(tinyxml2::XMLNode* node, Wt::WTreeNode* parent_node)
    {
        while (node) {
            auto child_tree_node = std::make_unique<XmlTreeNode>(xml_file_brain_, node);
            auto tree_child_node = parent_node->addChildNode(std::move(child_tree_node));
            tree_child_node->expand();
            if(node == xml_file_brain_->selected_node_)
            {
                tree_->select(tree_child_node);
                // scroll into view
            }

            tree_child_node->selected().connect(this, [=](bool selected)
            {
                if(selected)
                {
                    xml_file_brain_->xml_node_selected_.emit(node);
                }
            });

            recursiveAddChild(node->FirstChild(), tree_child_node);
            node = node->NextSibling();
        }
    }


}