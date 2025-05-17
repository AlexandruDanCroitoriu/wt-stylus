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


    class XMLElemView : public Wt::WContainerWidget
    {
        public:
            XMLElemView(std::shared_ptr<XMLFileBrain> file_brain);

            void resetUi(bool scroll_into_view = false);
            void dropEvent(Wt::WDropEvent event) override;

            std::shared_ptr<XMLFileBrain> file_brain_;
        protected:
            // Custom implementation
            void layoutSizeChanged(int width, int height) override;
        
        private:
    };

}