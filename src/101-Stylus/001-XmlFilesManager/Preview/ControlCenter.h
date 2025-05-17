#pragma once
#include "101-Stylus/000-Utils/tinyxml2/tinyxml2.h"
#include "101-Stylus/001-XmlFilesManager/Preview/XMLFileBrain.h"

#include <Wt/WContainerWidget.h>

namespace Stylus 
{

    
    class ControlCenter : public Wt::WContainerWidget
    {
        public:
            ControlCenter(std::shared_ptr<XMLFileBrain> file_brain);

        private:
            std::shared_ptr<XMLFileBrain> file_brain_;
    };
}