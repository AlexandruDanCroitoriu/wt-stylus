#pragma once
#include <Wt/WContainerWidget.h>
#include "101-Stylus/StylusState.h"

namespace Stylus {
class ImagesManager : public Wt::WContainerWidget
{
public:
    ImagesManager(std::shared_ptr<StylusState> state);
private:
    std::shared_ptr<StylusState> state_;
};
}