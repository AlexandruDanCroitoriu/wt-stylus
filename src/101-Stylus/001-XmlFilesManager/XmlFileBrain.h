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
            XmlFileBrain(std::string file_path);
            
        private:
            tinyxml2::XMLDocument doc_;
            std::string file_path_;
    };

    
    class XmlTreeNode : public Wt::WTreeNode
    {
        public:
            XmlTreeNode(std::shared_ptr<XmlFileBrain> xml_file_brain, tinyxml2::XMLNode* node);
            void dropEvent(Wt::WDropEvent event) override;
            
            
            Wt::WContainerWidget* label_wrapper_;
        private:
            std::shared_ptr<XmlFileBrain> xml_file_brain_;
            tinyxml2::XMLNode* node_;
         
    };
    

    class XmlElemNode : public Wt::WContainerWidget
    {
        public:
            XmlElemNode(std::shared_ptr<XmlFileBrain> xml_file_brain, tinyxml2::XMLElement* node);
            void dropEvent(Wt::WDropEvent event) override;


        private:
            std::shared_ptr<XmlFileBrain> xml_file_brain_;
            tinyxml2::XMLElement* node_;
    };

    
    class XmlTextNode : public Wt::WText
    {
        public:
            XmlTextNode(std::shared_ptr<XmlFileBrain> xml_file_brain, tinyxml2::XMLText* node);

        private:
            std::shared_ptr<XmlFileBrain> xml_file_brain_;
            tinyxml2::XMLText* node_;
    };


    class XmlFileUi : public Wt::WContainerWidget
    {
        public:
            XmlFileUi();
            void setFile(std::string file_path);
            Wt::WGridLayout* grid_layout_;
        private:
            std::shared_ptr<XmlFileBrain> xml_file_brain_;
    };  

}

