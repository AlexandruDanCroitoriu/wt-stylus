#include "101-Stylus/001-XmlFilesManager/Preview/ControlCenter.h"



namespace Stylus 
{

    ControlCenter::ControlCenter(std::shared_ptr<XMLFileBrain> file_brain)
        : file_brain_(file_brain)
    {
        setStyleClass("flex flex-col text-sm");
        setMinimumSize(Wt::WLength(300, Wt::LengthUnit::Pixel), Wt::WLength(100, Wt::LengthUnit::ViewportHeight));
        setMaximumSize(Wt::WLength(300, Wt::LengthUnit::Pixel), Wt::WLength(100, Wt::LengthUnit::ViewportHeight));
        setStyleClass("overflow-y-auto stylus-background border-l border-solid border-[var(--stylus-text)/50]");
    }

}