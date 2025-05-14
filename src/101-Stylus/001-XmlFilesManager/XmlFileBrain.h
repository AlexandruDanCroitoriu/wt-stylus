#pragma once
#include "101-Stylus/000-Utils/tinyxml2/tinyxml2.h"
#include "101-Stylus/StylusState.h"

#include <Wt/WString.h>
#include <Wt/WTreeNode.h>
#include <Wt/WIconPair.h>
#include <Wt/WContainerWidget.h>
#include <Wt/WGridLayout.h>
#include <Wt/WTree.h>
#include <Wt/WText.h>

namespace Stylus
{
    class XmlFileBrain
    {
        public:
            XmlFileBrain(std::shared_ptr<StylusState> state);
            
            void setFile(std::string file_path);

            tinyxml2::XMLDocument doc_;
            std::string file_path_;

            // the selected node and if to scroll the selected into view
            Wt::Signal<tinyxml2::XMLNode*, bool> xml_node_selected_;
            tinyxml2::XMLNode* selected_node_;
        private:
            std::shared_ptr<StylusState> state_;

    };

    
    class XmlTreeNode : public Wt::WTreeNode
    {
        public:
            XmlTreeNode(std::shared_ptr<XmlFileBrain> xml_file_brain, tinyxml2::XMLElement* node);
            void dropEvent(Wt::WDropEvent event) override;
            
            Wt::WContainerWidget* label_wrapper_;

            tinyxml2::XMLElement* node_;
            
            Wt::Signal<Wt::WString>& width_changed() { return width_changed_; }
    
            protected:
            // Custom implementation
            void layoutSizeChanged(int width, int height) override;
        
        private:
            std::shared_ptr<XmlFileBrain> xml_file_brain_;
            Wt::Signal<Wt::WString> width_changed_;
         
    };
    

    class XmlElemNode : public Wt::WContainerWidget
    {
        public:
            XmlElemNode(std::shared_ptr<XmlFileBrain> xml_file_brain, tinyxml2::XMLElement* node, bool scroll_into_view = false);
            void dropEvent(Wt::WDropEvent event) override;
            tinyxml2::XMLElement* node_;

        private:
            std::shared_ptr<XmlFileBrain> xml_file_brain_;
    };

    
    class XmlFileUi : public Wt::WContainerWidget
    {
        public:
            XmlFileUi(std::shared_ptr<StylusState> state);
            void setFile(std::string file_path);
            void resetUI(bool scroll_into_view = false);
            Wt::WGridLayout* grid_layout_;
            std::shared_ptr<XmlFileBrain> xml_file_brain_;
        private:
            void recursiveAddChild(tinyxml2::XMLNode* node, Wt::WTreeNode* parent_node, bool scroll_into_view = false);
            Wt::WTree* tree_;
            Wt::WContainerWidget* preview_wrapper_;
            std::shared_ptr<StylusState> state_;
    };  

}

