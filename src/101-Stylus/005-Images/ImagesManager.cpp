#include "101-Stylus/005-Images/ImagesManager.h"


namespace Stylus {

ImagesManager::ImagesManager(std::shared_ptr<StylusState> state)
    : Wt::WContainerWidget(),
      state_(state)
{
    setStyleClass("h-[100vh] stylus-background");
    
}

}