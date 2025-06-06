#pragma once
#include <Wt/WContainerWidget.h>
#include "101-Stylus/000-Utils/StylusState.h"

namespace Stylus {
class Settings : public Wt::WContainerWidget
{
public:
    Settings(std::shared_ptr<StylusState> state);
private:
    std::shared_ptr<StylusState> state_;
};
}