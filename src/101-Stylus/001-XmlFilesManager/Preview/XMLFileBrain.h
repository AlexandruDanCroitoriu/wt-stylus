#pragma once
#include "101-Stylus/000-Utils/tinyxml2/tinyxml2.h"
#include "101-Stylus/StylusState.h"

#include <Wt/WString.h>
#include <Wt/WSignal.h>


namespace Stylus
{

    class XMLFileBrain
    {
        public:
            XMLFileBrain(std::shared_ptr<StylusState> state, std::string file_path);
            
            tinyxml2::XMLDocument doc_;
            std::string file_path_;
            
            // the selected node and if to scroll the selected into view
            Wt::Signal<tinyxml2::XMLNode*, bool> xml_node_selected_;
            tinyxml2::XMLNode* selected_node_;
        private:
            std::shared_ptr<StylusState> state_;
            
    };
}