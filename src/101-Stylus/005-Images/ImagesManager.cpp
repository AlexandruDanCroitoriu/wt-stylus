#include "101-Stylus/005-Images/ImagesManager.h"


namespace Stylus {

ImagesManager::ImagesManager(std::shared_ptr<StylusState> state)
    : StylusPanelWrapper(state)
{
    addStyleClass("");
    
}

}