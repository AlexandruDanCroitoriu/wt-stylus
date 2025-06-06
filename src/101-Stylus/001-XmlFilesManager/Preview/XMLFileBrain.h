#pragma once
#include "101-Stylus/000-Utils/tinyxml2/tinyxml2.h"
#include "101-Stylus/000-Utils/StylusState.h"

#include <Wt/WString.h>
#include <Wt/WSignal.h>


namespace Stylus
{

    class XMLFileBrain
    {
        public:
            XMLFileBrain(std::shared_ptr<StylusState> state, std::string file_path);
            
            std::shared_ptr<tinyxml2::XMLDocument> doc_;
            std::string file_path_;
            
            void setFile(std::string file_path);

            // std::string trimWitespace(std::string str);
            // std::string trimAllWitespace(std::string str);
            // bool isCondNode(tinyxml2::XMLElement* node);
            /* 
                organizeXmlNode is used to split condition ${} brackets into separate text nodes,
                and in the future tu propagate changes to the xml tree
            */
            // void organizeXmlNode(tinyxml2::XMLElement* node);
            // the selected node and if to scroll the selected into view
            Wt::Signal<tinyxml2::XMLElement*, bool> xml_node_selected_;
            Wt::Signal<> file_saved_;
            
            tinyxml2::XMLElement* selected_node_;
            std::shared_ptr<StylusState> state_;
        private:
            
            
    };
}