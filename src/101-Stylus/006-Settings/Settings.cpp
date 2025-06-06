#include "101-Stylus/006-Settings/Settings.h"
#include <Wt/WApplication.h>
#include <Wt/WCheckBox.h>
#include <Wt/WLineEdit.h>

namespace Stylus {

Settings::Settings(std::shared_ptr<StylusState> state)
    : StylusPanelWrapper(state)
{
    addStyleClass("container mx-auto px-[16px] border border-solid");
    
    if(state_->settings_node_->BoolAttribute("use-tailwind-cdn")) {
        Wt::WApplication::instance()->require("https://cdn.jsdelivr.net/npm/@tailwindcss/browser@4");
    }
    
    auto checkbox = addWidget(std::make_unique<Wt::WCheckBox>("Use Tailwind CDN"));
    checkbox->setChecked(state_->settings_node_->BoolAttribute("use-tailwind-cdn"));
    checkbox->setStyleClass("mb-[16px] mt-[8px] text-[#111827] dark:text-[#FFF] text-sm");
    checkbox->changed().connect([=]()
    {
        state_->settings_node_->SetAttribute("use-tailwind-cdn", checkbox->isChecked() ? "true" : "false");
        state_->doc_->SaveFile(state_->state_file_path_.c_str());
        if(checkbox->isChecked())
        {
            Wt::WApplication::instance()->require("https://cdn.jsdelivr.net/npm/@tailwindcss/browser@4");
        }
    });


}

}