#include "101-Stylus/001-XmlFilesManager/Preview/ControlCenter.h"



namespace Stylus 
{

    ControlCenter::ControlCenter(std::shared_ptr<XMLFileBrain> file_brain)
        : file_brain_(file_brain)
    {
        setStyleClass("flex flex-col text-sm");
        setMinimumSize(Wt::WLength(300, Wt::LengthUnit::Pixel), Wt::WLength(100, Wt::LengthUnit::ViewportHeight));
        setMaximumSize(Wt::WLength(300, Wt::LengthUnit::Pixel), Wt::WLength(100, Wt::LengthUnit::ViewportHeight));
        setStyleClass("stylus-background z-[100] overflow-x-visible");


        auto box1 = addWidget(std::make_unique<Wt::WContainerWidget>());
        box1->setStyleClass("p-4 border border-solid bg-red-200 rounded-md");

        auto box2 = addWidget(std::make_unique<Wt::WContainerWidget>());
        box2->setStyleClass("w-[20px] h-screen border border-solid bg-green-200 absolute top-0 right-[-20px]");   
    }

}