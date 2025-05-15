#include "101-Stylus/001-XmlFilesManager/Preview/XMLTreeNode.h"
#include <Wt/WText.h>

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

        label_wrapper_->setStyleClass("flex space-x-2 truncate");
        content_wrapper_->setStyleClass("flex flex-col ml-[10px]");

        label_wrapper_->addWidget(std::make_unique<Wt::WText>(node->Name()))->setStyleClass("uppercase font-medium");

        if(node_ == file_brain_->selected_node_)
        {
            label_wrapper_->addStyleClass("selected-xml-node");
            if(scroll_into_view){
                label_wrapper_->doJavaScript(jsRef() + ".scrollIntoView({ behavior: 'smooth', block: 'center' });");
            }
        }

        clicked().connect(this, [=]()
        {
            file_brain_->xml_node_selected_.emit(node_, false);
        });
        mouseWentOver().preventPropagation();
        mouseWentOver().connect(this, [=]()
        {
            label_wrapper_->toggleStyleClass("preview-xml-node-hover", true, true);
        });
        mouseWentOut().connect(this, [=]()
        {
            label_wrapper_->toggleStyleClass("preview-xml-node-hover", false, true);
        });
        auto first_child = node->FirstChild();
        while (first_child) {
            if (first_child->ToElement()) {
                auto child_node = std::make_unique<XMLTreeNode>(file_brain, first_child->ToElement(), scroll_into_view);
                content_wrapper_->addWidget(std::move(child_node));
            } else if (first_child->ToText()) {
                label_wrapper_->addWidget(std::make_unique<Wt::WText>(first_child->ToText()->Value()))->setStyleClass("truncate italic font-light");
            }
            first_child = first_child->NextSibling();
        }
    }

    
    XMLTreeView::XMLTreeView(std::shared_ptr<XMLFileBrain> file_brain)
        : file_brain_(file_brain)
    {
        file_brain_->xml_node_selected_.connect(this, [=](tinyxml2::XMLNode* node, bool scroll_into_view)
        {
            file_brain_->selected_node_ = node;
            resetUi(scroll_into_view);
        });

        if(file_brain_->selected_node_){
            resetUi();
        }
        setLayoutSizeAware(true);
        setMinimumSize(Wt::WLength(200, Wt::LengthUnit::Pixel), Wt::WLength(100, Wt::LengthUnit::ViewportHeight));
        setMaximumSize(Wt::WLength(1000, Wt::LengthUnit::Pixel), Wt::WLength(100, Wt::LengthUnit::ViewportHeight));
        setStyleClass("overflow-y-auto scrollbar-stylus");
    }

    void XMLTreeView::resetUi(bool scroll_into_view)
    {
         clear();
        if(file_brain_->doc_.ErrorID() != tinyxml2::XML_SUCCESS)
        {
            std::cout << "\n\n ELEMENT file brain has errors: " << file_brain_->doc_.ErrorID() << "\n\n";
            addWidget(std::make_unique<Wt::WText>("Error loading XML file"));
        }else {
            addWidget(std::make_unique<XMLTreeNode>(file_brain_, file_brain_->doc_.RootElement(), scroll_into_view))->addStyleClass("stylus-list-tree");
        }
    }

    void XMLTreeView::dropEvent(Wt::WDropEvent event)
    {
        std::cout << "Drop event on XML element node: " << event.mimeType() << "\n";
    }

    void XMLTreeView::layoutSizeChanged(int width, int height)
    {
        if(width >= 200 && width <= 1000)
        {
            width_changed_.emit(width);
        }
    }


}
