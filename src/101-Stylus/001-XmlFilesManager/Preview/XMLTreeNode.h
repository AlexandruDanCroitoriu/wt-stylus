#pragma once
#include "101-Stylus/000-Utils/tinyxml2/tinyxml2.h"
#include "101-Stylus/001-XmlFilesManager/Preview/XMLFileBrain.h"

#include <Wt/WContainerWidget.h>
#include <Wt/WTemplate.h>
#include <Wt/WPopupMenu.h>

namespace Stylus 
{

    
    class XMLTreeNode : public Wt::WContainerWidget
{
    public:
        XMLTreeNode(std::shared_ptr<XMLFileBrain> file_brain, tinyxml2::XMLElement* node, bool scroll_into_view = false);
        tinyxml2::XMLElement* node_;
        void showPopup(const Wt::WMouseEvent& event);
        
        private:
        std::shared_ptr<XMLFileBrain> file_brain_;

        Wt::WContainerWidget* label_wrapper_;
        Wt::WContainerWidget* content_wrapper_;
        std::unique_ptr<Wt::WPopupMenu> popup_;

    };

}