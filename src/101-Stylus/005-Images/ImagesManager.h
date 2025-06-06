#pragma once
#include "101-Stylus/000-Utils/StylusPanelWrapper.h"
#include "101-Stylus/000-Utils/StylusState.h"

namespace Stylus {
class ImagesManager : public StylusPanelWrapper
{
public:
    ImagesManager(std::shared_ptr<StylusState> state);
private:
};
}