#include "101-Stylus/001-XmlFilesManager/Preview/XMLElemNode.h"
#include <Wt/WText.h>
#include <Wt/WApplication.h>

namespace Stylus
{

    XMLElemNode::XMLElemNode(std::shared_ptr<XMLFileBrain> file_brain, tinyxml2::XMLElement* node, bool scroll_into_view)
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
                // doJavaScript(jsRef() + ".scrollIntoView({ behavior: 'smooth', block: 'center' });");
                doJavaScript(jsRef() + ".scrollIntoView({ block: 'center' });");
            }
            
        }

        clicked().connect(this, [=]()
        {
            file_brain_->xml_node_selected_.emit(node_, false);
        });
        mouseWentOver().preventPropagation();
        mouseWentOver().connect(this, [=]()
        {
            toggleStyleClass("selected-xml-node-hover", true, true);
        });
        mouseWentOut().connect(this, [=]()
        {
            toggleStyleClass("selected-xml-node-hover", false, true);
        });
        auto first_child = node->FirstChild();
        while (first_child) {
            if (first_child->ToElement()) {
                auto child_node = std::make_unique<XMLElemNode>(file_brain, first_child->ToElement(), scroll_into_view);
                addWidget(std::move(child_node));
            } else if (first_child->ToText()) {
                auto text_node = addWidget(std::make_unique<Wt::WText>(first_child->ToText()->Value()));


                if(first_child->PreviousSiblingElement() || first_child->NextSiblingElement()){

                    text_node->setStyleClass("font-bold text-[#ff0000] outline-2 outline-[#ff0000] rounded-md p-[2px] hover:bg-[#ff0000]/30 cursor-pointer");
                    text_node->clicked().preventPropagation();
                    text_node->clicked().connect(this, [=](const Wt::WMouseEvent& event)
                    {
                        auto new_node = file_brain_->doc_->NewElement("span");
                        auto parent_node = first_child->Parent();
                        auto prev_node = first_child->PreviousSibling();

                        std::string text = first_child->ToText()->Value();
                        // remove whitespace from start and end of the string
                        text.erase(0, text.find_first_not_of(" \t\n\r\f\v"));
                        text.erase(text.find_last_not_of(" \t\n\r\f\v") + 1);
                        if(prev_node && parent_node)
                        {
                            parent_node->InsertAfterChild(prev_node, new_node);
                        }else {
                            parent_node->InsertFirstChild(new_node);
                        }
                        new_node->SetText(text.c_str());
                        file_brain_->doc_->DeleteNode(first_child);
                        file_brain_->doc_->SaveFile(file_brain_->file_path_.c_str());
                        file_brain_->file_saved_.emit();
                        file_brain_->xml_node_selected_.emit(new_node, true);
                    });
                }
            }
            first_child = first_child->NextSibling();
        }
    }

    
    XMLElemView::XMLElemView(std::shared_ptr<XMLFileBrain> file_brain)
        : file_brain_(file_brain)
    {
        setLayoutSizeAware(true);
        setMinimumSize(Wt::WLength(200, Wt::LengthUnit::Pixel), Wt::WLength(100, Wt::LengthUnit::ViewportHeight));
        setMaximumSize(Wt::WLength::Auto, Wt::WLength(100, Wt::LengthUnit::ViewportHeight));
        setStyleClass("overflow-auto stylus-background");

        
        file_brain_->xml_node_selected_.connect(this, [=](tinyxml2::XMLNode* node, bool scroll_into_view)
        {
            file_brain_->selected_node_ = node;
            resetUi(scroll_into_view);
        });
        resetUi();
    }

    void XMLElemView::resetUi(bool scroll_into_view)
    {
        clear();
        if(file_brain_->doc_->ErrorID() != tinyxml2::XML_SUCCESS)
        {
            std::cout << "\n\n ELEMENT file brain has errors: " << file_brain_->doc_->ErrorID() << "\n\n";
            addWidget(std::make_unique<Wt::WText>("Error loading XML file"));
        }else {
            addWidget(std::make_unique<XMLElemNode>(file_brain_, file_brain_->doc_->RootElement(), scroll_into_view));
        }
    }

    void XMLElemView::dropEvent(Wt::WDropEvent event)
    {
        std::cout << "Drop event on XML element node: " << event.mimeType() << "\n";
    }

    void XMLElemView::layoutSizeChanged(int width, int height)
    {
        std::cout << "XMLElemView size changed: " << width << "x" << height << "\n";
    }

}
