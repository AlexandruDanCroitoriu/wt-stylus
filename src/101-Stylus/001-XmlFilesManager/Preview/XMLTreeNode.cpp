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

        label_wrapper_->setStyleClass("flex space-x-2 truncate rounded-md");
        content_wrapper_->setStyleClass("flex flex-col ml-[10px]");

        label_wrapper_->addWidget(std::make_unique<Wt::WText>(node->Name()))->setStyleClass("font-medium pl-[5px]");

        if(node_ == file_brain_->selected_node_)
        {
            label_wrapper_->addStyleClass("selected-xml-node");
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
                
                if(first_child->PreviousSiblingElement() || first_child->NextSiblingElement()){
                    auto text_node = content_wrapper_->addWidget(std::make_unique<Wt::WText>(first_child->ToText()->Value()));
                    text_node->setStyleClass("truncate italic font-light text-[#ff0000] text-bold");
                }else {
                    auto text_node = label_wrapper_->addWidget(std::make_unique<Wt::WText>(first_child->ToText()->Value()));
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

    
    XMLTreeView::XMLTreeView(std::shared_ptr<XMLFileBrain> file_brain)
        : file_brain_(file_brain)
    {
        setLayoutSizeAware(true);
        setMinimumSize(Wt::WLength(200, Wt::LengthUnit::Pixel), Wt::WLength(100, Wt::LengthUnit::ViewportHeight));
        setMaximumSize(Wt::WLength(1000, Wt::LengthUnit::Pixel), Wt::WLength(100, Wt::LengthUnit::ViewportHeight));
        setStyleClass("overflow-y-auto scrollbar-stylus");

        Wt::WStringStream contextJS;
        contextJS << WT_CLASS << ".$('" << id() << "').oncontextmenu = "
                    << "function() { event.cancelBubble = true; event.returnValue = false; return false; };";
        Wt::WApplication::instance()->doJavaScript(contextJS.str());

        file_brain_->xml_node_selected_.connect(this, [=](tinyxml2::XMLNode* node, bool scroll_into_view)
        {
            file_brain_->selected_node_ = node;
            resetUi(scroll_into_view);
        });

        if(file_brain_->selected_node_){
            resetUi();
        }
    }

    void XMLTreeView::resetUi(bool scroll_into_view)
    {
        clear();
        if(file_brain_->doc_->ErrorID() != tinyxml2::XML_SUCCESS)
        {
            std::cout << "\n\n ELEMENT file brain has errors: " << file_brain_->doc_->ErrorID() << "\n\n";
            addWidget(std::make_unique<Wt::WText>("Error loading XML file"));
        }else {
            addWidget(std::make_unique<XMLTreeNode>(file_brain_, file_brain_->doc_->RootElement(), scroll_into_view))->addStyleClass("stylus-list-tree");
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
