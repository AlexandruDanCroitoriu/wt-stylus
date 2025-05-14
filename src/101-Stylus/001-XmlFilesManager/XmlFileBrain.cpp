#include "101-Stylus/001-XmlFilesManager/XmlFileBrain.h"
#include <iostream>
#include <Wt/WTemplate.h>

namespace Stylus {

    XmlFileBrain::XmlFileBrain(std::shared_ptr<StylusState> state)
    : state_(state)
    {
    }

    void XmlFileBrain::setFile(std::string file_path)
    {
        file_path_ = file_path;
        // Load the XML file
        tinyxml2::XMLError eResult = doc_.LoadFile(file_path.c_str());
        if (eResult != tinyxml2::XML_SUCCESS) {
            std::cout << "\n\nError loading XML file: " << doc_.ErrorIDToName(eResult) << "\n\n";
        }
        selected_node_ = doc_.RootElement();

    }


    // derived tree node
    XmlTreeNode::XmlTreeNode(std::shared_ptr<XmlFileBrain> xml_file_brain, tinyxml2::XMLElement* node)
    : Wt::WTreeNode(""), node_(node), xml_file_brain_(xml_file_brain)
    {
        label_wrapper_ = labelArea();
        // acceptDrops("file", "Wt-item");
        expand();
       
        label()->setText(node->Name());
        label()->setStyleClass("font-bold uppercase");
        if(node->FirstChild() && node->FirstChild()->ToText()){
            auto text_wid = labelArea()->addWidget(std::make_unique<Wt::WText>(node->FirstChild()->ToText()->Value()));
            text_wid->setStyleClass("italic ml-3");
        }
        
    }
    void XmlTreeNode::dropEvent(Wt::WDropEvent event)
    {
        std::cout << "Drop event on XML tree node: " << event.mimeType() << "\n";
    }
    
    void XmlTreeNode::layoutSizeChanged(int width, int height)
    {
        if(width >= 200 && width <= 1000) {
            width_changed_.emit(std::to_string(width));
        }
    }


    // element node
    XmlElemNode::XmlElemNode(std::shared_ptr<XmlFileBrain> xml_file_brain, tinyxml2::XMLElement* node, bool scroll_into_view)
    : Wt::WContainerWidget(), node_(node), xml_file_brain_(xml_file_brain)
    {
        // acceptDrops("file", "Wt-item");
        // recive error if given a messages tag name to an element
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
            if(scroll_into_view){

                doJavaScript(jsRef() + ".scrollIntoView({ behavior: 'smooth', block: 'center' });");
            }
            
        }

        clicked().connect(this, [=]()
        {
            xml_file_brain_->xml_node_selected_.emit(node_, false);
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
                auto child_node = std::make_unique<XmlElemNode>(xml_file_brain, first_child->ToElement(), scroll_into_view);
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


    XmlFileUi::XmlFileUi(std::shared_ptr<StylusState> state)
    : xml_file_brain_(std::make_shared<XmlFileBrain>(state)),
        state_(state)
    {
        keyWentUp().preventDefaultAction();
        keyWentDown().preventDefaultAction();
        auto grid_layout = std::make_unique<Wt::WGridLayout>();
        grid_layout_ = grid_layout.get();
        grid_layout_->setContentsMargins(0, 0, 0, 0);

        auto tree_wrapper = grid_layout_->addWidget(std::make_unique<Wt::WContainerWidget>(), 0, 0);
        tree_ = tree_wrapper->addWidget(std::make_unique<Wt::WTree>());
        preview_wrapper_ = grid_layout_->addWidget(std::make_unique<Wt::WContainerWidget>(), 0, 1);
        
        xml_file_brain_->xml_node_selected_.connect(this, [=](tinyxml2::XMLNode* node, bool scroll_into_view)
        {
            xml_file_brain_->selected_node_ = node;
            resetUI(scroll_into_view);
        });
        
        setMinimumSize(Wt::WLength::Auto, Wt::WLength(100, Wt::LengthUnit::ViewportHeight));
        tree_->setMinimumSize(Wt::WLength(200, Wt::LengthUnit::Pixel), Wt::WLength(100, Wt::LengthUnit::Percentage));
        tree_->setMaximumSize(Wt::WLength(1000, Wt::LengthUnit::Pixel), Wt::WLength(100, Wt::LengthUnit::Percentage));
        // setStyleClass("w-full h-full");
        preview_wrapper_->setStyleClass("overflow-auto h-screen w-full scrollbar-stylus p-12");
        tree_wrapper->setStyleClass("overflow-y-auto overflow-x-none h-screen scrollbar-stylus");

        grid_layout_->setColumnResizable(0, true, Wt::WLength(state_->xml_node_->Int64Attribute("preview-widget-sidebar-width"), Wt::LengthUnit::Pixel));

        setLayout(std::move(grid_layout)); 
    }

    void XmlFileUi::setFile(std::string file_path)
    {
        xml_file_brain_->setFile(file_path);
        resetUI();
    }

    void XmlFileUi::resetUI(bool scroll_into_view)
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
        if(xml_file_brain_->selected_node_ == xml_file_brain_->doc_.RootElement())
        {
            tree_->select(tree_root_node);
        }
        tree_root_node->selected().connect(this, [=](bool selected)
            {
                if(selected)
                {
                    xml_file_brain_->xml_node_selected_.emit(xml_file_brain_->doc_.RootElement(), false);
                }
            }
        );
        tree_root_node->width_changed().connect(this, [=](Wt::WString width)
        {
            state_->xml_node_->SetAttribute("preview-widget-sidebar-width", width.toUTF8().c_str());
            state_->doc_.SaveFile(state_->state_file_path_.c_str());
        });
        recursiveAddChild(xml_file_brain_->doc_.RootElement()->FirstChild()->ToElement(), tree_root_node, scroll_into_view);

        preview_wrapper_->addWidget(std::make_unique<XmlElemNode>(xml_file_brain_, xml_file_brain_->doc_.RootElement(), scroll_into_view));    
    }

    void XmlFileUi::recursiveAddChild(tinyxml2::XMLNode* node, Wt::WTreeNode* parent_node, bool scroll_into_view)
    {
        while (node) {
            if(node->ToElement()){
                
                auto child_tree_node = std::make_unique<XmlTreeNode>(xml_file_brain_, node->ToElement());
                auto tree_child_node = parent_node->addChildNode(std::move(child_tree_node));
                if(node == xml_file_brain_->selected_node_)
                {
                    tree_->select(tree_child_node);
                    if(scroll_into_view){
                        std::cout << "\n\nscrolling tree into view\n\n";
                        auto tree_node = dynamic_cast<XmlTreeNode*>(tree_child_node);
                        tree_node->doJavaScript(tree_node->label_wrapper_->jsRef() + ".scrollIntoView({ behavior: 'auto', block: 'center' });");
                    }
                }   
                
                
                
                tree_child_node->selected().connect(this, [=](bool selected)
                {
                    if(selected)
                    {
                        xml_file_brain_->xml_node_selected_.emit(node, false);
                    }
                });
                recursiveAddChild(node->FirstChild(), tree_child_node, scroll_into_view);
            }

            node = node->NextSibling();
        }
    }


}