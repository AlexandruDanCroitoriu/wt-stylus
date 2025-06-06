#pragma once
#include <Wt/WContainerWidget.h>
#include "101-Stylus/000-Utils/StylusState.h"

namespace Stylus {

    class StylusPanel : public Wt::WContainerWidget
    {
    public:
        StylusPanel(std::shared_ptr<StylusState> state);

    private:
        std::shared_ptr<StylusState> state_;
        
    };
}
