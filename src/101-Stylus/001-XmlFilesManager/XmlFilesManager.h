#pragma once
#include "101-Stylus/000-Utils/FilesManager.h"
#include "101-Stylus/000-Utils/StylusState.h"
#include <Wt/WSignal.h>
#include "101-Stylus/001-XmlFilesManager/Preview/XMLFileBrain.h"
#include "101-Stylus/001-XmlFilesManager/Preview/XMLElemNode.h"
#include "101-Stylus/001-XmlFilesManager/Preview/XMLTreeNode.h"
#include "101-Stylus/001-XmlFilesManager/Preview/ControlCenter.h"
#include <map>

namespace Stylus
{

    class GridItemWrapper : public Wt::WContainerWidget
    {
        public:
            GridItemWrapper() { setLayoutSizeAware(true); };
            Wt::Signal<int> width_changed_;
        protected:
            void layoutSizeChanged(int width, int height) override
            {
                if(width > 1)
                {
                    width_changed_.emit(width);
                }
            }
        private:
    };


    class XmlFilesManager : public FilesManager
    {
    public:
        XmlFilesManager(std::shared_ptr<StylusState> state);

        std::shared_ptr<XMLFileBrain> selected_file_brain_;
        std::map<std::string, std::shared_ptr<XMLFileBrain>> xml_file_brains_;

        void setPreviewWidgets(bool scroll_into_view = false);
        
        GridItemWrapper* tree_wrapper_;
        GridItemWrapper* elem_wrapper_;
        ControlCenter* control_center_;
    private:
        std::shared_ptr<StylusState> state_;
        void setXmlFileBrains();
        

    };
}