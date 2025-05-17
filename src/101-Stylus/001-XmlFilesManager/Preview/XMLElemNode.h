#pragma once
#include "101-Stylus/000-Utils/tinyxml2/tinyxml2.h"
#include "101-Stylus/001-XmlFilesManager/Preview/XMLFileBrain.h"

#include <Wt/WContainerWidget.h>

namespace Stylus 
{

    
    class XMLElemNode : public Wt::WContainerWidget
    {
    public:
        XMLElemNode(std::shared_ptr<XMLFileBrain> file_brain, tinyxml2::XMLElement* node, bool scroll_into_view = false);
        tinyxml2::XMLElement* node_;
        
    private:
        std::shared_ptr<XMLFileBrain> file_brain_;
    };

}