#include "101-Stylus/001-XmlFilesManager/Preview/XMLElemNode.h"

namespace Stylus
{

    XMLElemNode::XMLElemNode(std::shared_ptr<XmlFileBrain> file_brain, tinyxml2::XMLElement* node, bool scroll_into_view)
    : Wt::WContainerWidget(), node_(node), file_brain_(file_brain)
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
        if(node_ == file_brain_->selected_node_)
        {
            addStyleClass("selected-xml-node");
            if(scroll_into_view){

                doJavaScript(jsRef() + ".scrollIntoView({ behavior: 'smooth', block: 'center' });");
            }
            
        }

        clicked().connect(this, [=]()
        {
            file_brain_->xml_node_selected_.emit(node_, false);
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
                auto child_node = std::make_unique<XMLElemNode>(file_brain, first_child->ToElement(), scroll_into_view);
                addWidget(std::move(child_node));
            } else if (first_child->ToText()) {
                addWidget(std::make_unique<Wt::WText>(first_child->ToText()->Value()));
            }
            first_child = first_child->NextSibling();
        }
    }

    
    XMLElemView::XMLElemView(std::shared_ptr<StylusState> state)
        : file_brain_(std::make_shared<XmlFileBrain>(state)),
        state_(state)
    {
        
    }

    void XMLElemView::setXmlFileBrain(std::string file_path)
    {
        file_brain_->setFile(file_path);
    }

    void XMLElemView::resetUi(bool scroll_into_view)
    {
        clear();
        addWidget(std::make_unique<XMLElemNode>(file_brain_, file_brain_->doc_.RootElement(), scroll_into_view));
    }

    void XMLElemView::dropEvent(Wt::WDropEvent event)
    {
        std::cout << "Drop event on XML element node: " << event.mimeType() << "\n";
    }

}
