#include "101-Stylus/001-XmlFilesManager/Preview/AttributeControl.h"



namespace Stylus 
{

    AttributeControl::AttributeControl(std::shared_ptr<XMLFileBrain> file_brain)
        : file_brain_(file_brain)
    {
        setStyleClass("flex flex-col text-sm");
        setMinimumSize(Wt::WLength(10, Wt::LengthUnit::Pixel), Wt::WLength(100, Wt::LengthUnit::ViewportHeight));
        setMaximumSize(Wt::WLength(10, Wt::LengthUnit::Pixel), Wt::WLength(100, Wt::LengthUnit::ViewportHeight));
        setStyleClass("overflow-y-auto scrollbar-stylus border-l border-solid");
    }

}