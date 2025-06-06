#pragma once
#include "101-Stylus/000-Utils/FilesManager.h"
#include "101-Stylus/000-Utils/StylusState.h"

namespace Stylus
{

    class CssFilesManager : public FilesManager
    {
    public:
        CssFilesManager(std::shared_ptr<StylusState> state);

    private:
        std::shared_ptr<StylusState> state_;
    };
}