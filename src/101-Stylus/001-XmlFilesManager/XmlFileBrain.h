#pragma once
#include "101-Stylus/000-Utils/tinyxml2/tinyxml2.h"
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
            XmlFileBrain();
            
            void setFile(std::string file_path);

            tinyxml2::XMLDocument doc_;
            std::string file_path_;

            Wt::Signal<tinyxml2::XMLNode*> xml_node_selected_;
            tinyxml2::XMLNode* selected_node_;
        private:

    };

    
    class XmlTreeNode : public Wt::WTreeNode
    {
        public:
            XmlTreeNode(std::shared_ptr<XmlFileBrain> xml_file_brain, tinyxml2::XMLNode* node);
            void dropEvent(Wt::WDropEvent event) override;
            
            Wt::WContainerWidget* label_wrapper_;

            tinyxml2::XMLNode* node_;
        private:
            std::shared_ptr<XmlFileBrain> xml_file_brain_;
         
    };
    

    class XmlElemNode : public Wt::WContainerWidget
    {
        public:
            XmlElemNode(std::shared_ptr<XmlFileBrain> xml_file_brain, tinyxml2::XMLElement* node);
            void dropEvent(Wt::WDropEvent event) override;
            tinyxml2::XMLElement* node_;

        private:
            std::shared_ptr<XmlFileBrain> xml_file_brain_;
    };

    
    class XmlFileUi : public Wt::WContainerWidget
    {
        public:
            XmlFileUi();
            void setFile(std::string file_path);
            void resetUI();
            Wt::WGridLayout* grid_layout_;
        private:
            void recursiveAddChild(tinyxml2::XMLNode* node, Wt::WTreeNode* parent_node);
            Wt::WTree* tree_;
            Wt::WContainerWidget* preview_wrapper_;
            std::shared_ptr<XmlFileBrain> xml_file_brain_;
    };  

}

