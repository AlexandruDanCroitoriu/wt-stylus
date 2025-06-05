#include "101-Stylus/005-Settings/Settings.h"


namespace Stylus {

Settings::Settings(std::shared_ptr<StylusState> state)
    : Wt::WContainerWidget(),
      state_(state)
{
    setStyleClass("h-[100vh] stylus-background");
    
}

}