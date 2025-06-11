#pragma once
#include "101-Stylus/000-Utils/StylusPanelWrapper.h"
#include "101-Stylus/000-Utils/StylusState.h"

namespace Stylus {
class Settings : public StylusPanelWrapper
{
public:
    Settings(std::shared_ptr<StylusState> state);
private:
};
}