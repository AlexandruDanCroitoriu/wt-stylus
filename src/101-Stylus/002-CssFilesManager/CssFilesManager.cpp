#include "101-Stylus/002-CssFilesManager/CssFilesManager.h"
#include <Wt/WApplication.h>
#include <Wt/WRandom.h>
#include <fstream>

namespace Stylus
{

    CssFilesManager::CssFilesManager(std::shared_ptr<StylusState> state)
        : state_(state),
        FilesManager(state, state->css_editor_data_, state->css_node_->IntAttribute("sidebar-width"), state->css_node_->Attribute("selected-file-path"))
    {
        

        sidebar_->width_changed().connect(this, [=](Wt::WString width)
        {
            state_->css_node_->SetAttribute("sidebar-width", std::stoi(width.toUTF8()));
            state_->doc_.SaveFile(state_->state_file_path_.c_str());
        });

        file_selected().connect(this, [=]()
        {
            if(std::fstream(data_.root_folder_path_ + selected_file_path_).good() == false)
            {
                state_->css_node_->SetAttribute("selected-file-path", "");
            }
            else {
                state_->css_node_->SetAttribute("selected-file-path", selected_file_path_.c_str());
            }
            state_->doc_.SaveFile(state_->state_file_path_.c_str());
        });
        
    }

}