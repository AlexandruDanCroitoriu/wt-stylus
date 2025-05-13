#include "101-Stylus/001-XmlFilesManager/XmlFilesManager.h"
#include <filesystem>
#include <Wt/WPushButton.h>
#include <Wt/WMenu.h>
#include <Wt/WMenuItem.h>
#include <Wt/WPanel.h>
#include <Wt/WTemplate.h>
#include <fstream>
#include <Wt/WMessageBox.h>
#include <Wt/WLineEdit.h>
#include <Wt/WLabel.h>
#include <Wt/WDialog.h>
#include <regex>
#include <Wt/WRandom.h>
#include <Wt/WApplication.h>
#include <Wt/WText.h>
#include <Wt/WContainerWidget.h>
#include <Wt/WLayout.h>
#include "001-App/App.h"

namespace Stylus
{

    XmlFilesManager::XmlFilesManager(std::shared_ptr<StylusState> state)
        : state_(state),
        FilesManager(state, state->xml_editor_data_, state->xml_node_->IntAttribute("sidebar-width"), state->xml_node_->Attribute("selected-file-path"))
    {
        sidebar_->footer_->show();
        if(!state_->xml_node_->BoolAttribute("editor-open"))
        {
            editor_->hide();
        }

        
        // auto temp_wrapper = addWidget(std::make_unique<Wt::WContainerWidget>());
        auto temp_wrapper = grid_layout_->addWidget(std::make_unique<Wt::WContainerWidget>(), 0, 2);
        grid_layout_->setColumnResizable(1, true, Wt::WLength(state_->xml_node_->IntAttribute("editor-width"), Wt::LengthUnit::Pixel));
        temp_wrapper->setStyleClass("p-[8px] stylus-background overflow-y-auto h-screen"); 


        auto temp_view = temp_wrapper->addWidget(std::make_unique<Wt::WTemplate>());
        temp_view->setTemplateText(editor_->getUnsavedText(), Wt::TextFormat::UnsafeXHTML);
        dark_mode_toggle_ = sidebar_->footer_->addWidget(std::make_unique<DarkModeToggle>());
      
        auto editor_checkbox = sidebar_->footer_->addWidget(std::make_unique<Wt::WCheckBox>("Editor"));
        editor_checkbox->setChecked(state_->xml_node_->BoolAttribute("editor-open"));
        editor_checkbox->keyWentDown().connect(this, [=](Wt::WKeyEvent e) { 
            Wt::WApplication::instance()->globalKeyWentDown().emit(e); // Emit the global key event
        });


        editor_checkbox->changed().connect(this, [=]()
        {
            if (editor_checkbox->isChecked())
            {
                grid_layout_->itemAt(1)->widget()->animateShow(Wt::WAnimation(Wt::AnimationEffect::SlideInFromLeft, Wt::TimingFunction::EaseInOut, 500));
                state_->xml_node_->SetAttribute("editor-open", "true");
            }
            else
            {
                grid_layout_->itemAt(1)->widget()->animateHide(Wt::WAnimation(Wt::AnimationEffect::SlideInFromLeft, Wt::TimingFunction::EaseInOut, 500));
                state_->xml_node_->SetAttribute("editor-open", "false");
            }
            state_->doc_.SaveFile(state_->state_file_path_.c_str());
        });

        file_selected().connect(this, [=]()
        {
            std::string file_path = data_.root_folder_path_ + selected_file_path_;
            state_->xml_node_->SetAttribute("selected-file-path", selected_file_path_.c_str());
            temp_view->setTemplateText(editor_->getUnsavedText(), Wt::TextFormat::UnsafeXHTML);
            state_->doc_.SaveFile(state_->state_file_path_.c_str());
        });
        file_saved().connect(this, [=](Wt::WString file_path)
        {
            temp_view->setTemplateText(editor_->getUnsavedText(), Wt::TextFormat::UnsafeXHTML);
        });
        
        sidebar_->width_changed().connect(this, [=](Wt::WString width)
        {
            if(std::stoi(width.toUTF8()) != state_->xml_node_->IntAttribute("sidebar-width"))
            {
                state_->xml_node_->SetAttribute("sidebar-width", std::stoi(width.toUTF8()));
                state_->doc_.SaveFile(state_->state_file_path_.c_str());
            }
        });

        editor_->width_changed().connect(this, [=](Wt::WString width)
        {
            if(std::stoi(width.toUTF8()) != state_->xml_node_->IntAttribute("editor-width"))
            {
                state_->xml_node_->SetAttribute("editor-width", std::stoi(width.toUTF8()));
                state_->doc_.SaveFile(state_->state_file_path_.c_str());
            }
        });

        dark_mode_toggle_->dark_mode_changed_.connect(this, [=](bool dark)
        {
            editor_->setDarkTheme(dark);
            state_->stylus_node_->SetAttribute("dark-mode", dark ? "true" : "false");
            state_->doc_.SaveFile(state_->state_file_path_.c_str());
        });

        if(state_->stylus_node_->BoolAttribute("dark-mode")){
            dynamic_cast<App*>(Wt::WApplication::instance())->dark_mode_changed_.emit(true);
            editor_->setDarkTheme(true);
        }


    }

}