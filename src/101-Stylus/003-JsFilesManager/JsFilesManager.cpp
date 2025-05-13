#include "101-Stylus/003-JsFilesManager/JsFilesManager.h"
#include <Wt/WApplication.h>
#include <Wt/WRandom.h>
#include <fstream>

namespace Stylus
{

    JsFilesManager::JsFilesManager(std::shared_ptr<StylusState> state)
        : state_(state),
        FilesManager(state, state->js_editor_data_, state->js_node_->IntAttribute("sidebar-width"), state->js_node_->Attribute("selected-file-path"))
    {        

        // file_saved().connect(this, [=](Wt::WString file_path)
        // {
        //     Wt::WApplication::instance()->require(file_path.toUTF8() + "?v=" + Wt::WRandom::generateId());
        // });

        file_selected().connect(this, [=]()
        {
            if(std::fstream(data_.root_folder_path_ + selected_file_path_).good() == false)
            {
                state_->js_node_->SetAttribute("selected-file-path", "");
            }else {
                state_->js_node_->SetAttribute("selected-file-path", selected_file_path_.c_str());
            }
            state_->doc_.SaveFile(state_->state_file_path_.c_str());
        });

        sidebar_->width_changed().connect(this, [=](Wt::WString width)
        {
            state_->js_node_->SetAttribute("sidebar-width", std::stoi(width.toUTF8()));
            state_->doc_.SaveFile(state_->state_file_path_.c_str());
        });
    }
}