#pragma once
#include "101-Stylus/000-Utils/FilesManager.h"
#include "101-Stylus/StylusState.h"
#include <Wt/WSignal.h>
#include "101-Stylus/001-XmlFilesManager/Preview/XMLFileBrain.h"
#include "101-Stylus/001-XmlFilesManager/Preview/XMLElemNode.h"
#include "101-Stylus/001-XmlFilesManager/Preview/XMLTreeNode.h"
#include <map>

namespace Stylus
{

    class XmlFilesManager : public FilesManager
    {
    public:
        XmlFilesManager(std::shared_ptr<StylusState> state);

        std::shared_ptr<XMLFileBrain> selected_file_brain_;
        std::map<std::string, std::shared_ptr<XMLFileBrain>> xml_file_brains_;

        void setPreviewWidgets();

    private:
        std::shared_ptr<StylusState> state_;
        void setXmlFileBrains();
        
        Wt::WContainerWidget* tree_wrapper_;
        Wt::WContainerWidget* elem_wrapper_;

        XMLTreeView* xml_tree_preview_;
        XMLElemView* xml_elem_preview_;
        
    };
}