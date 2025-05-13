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
        
        // auto temp_wrapper = addWidget(std::make_unique<Wt::WContainerWidget>());
        auto temp_wrapper = grid_layout_->addWidget(std::make_unique<Wt::WContainerWidget>(), 0, 2);
        grid_layout_->setColumnResizable(1, true, Wt::WLength(state_->xml_node_->IntAttribute("editor-width"), Wt::LengthUnit::Pixel));
        temp_wrapper->setStyleClass("p-[8px] stylus-background overflow-y-auto h-screen w-full flex"); 


        auto temp_view = temp_wrapper->addWidget(std::make_unique<Wt::WTemplate>());
        temp_view->setTemplateText(editor_->getUnsavedText(), Wt::TextFormat::UnsafeXHTML);
        
        // auto xml_tree_wrapper = temp_wrapper->addWidget(std::make_unique<Wt::WContainerWidget>());
        // auto xml_elem_wrapper = temp_wrapper->addWidget(std::make_unique<Wt::WContainerWidget>());
        
        // xml_tree_wrapper->setStyleClass("bg-gray-100 dark:bg-gray-800 rounded-lg shadow-md p-4 m-2");
        // xml_elem_wrapper->setStyleClass("bg-gray-100 dark:bg-gray-800 rounded-lg shadow-md p-4 m-2");
        
        // xml_tree_wrapper->addWidget(std::make_unique<Wt::WText>("XML Tree"));
        // xml_elem_wrapper->addWidget(std::make_unique<Wt::WText>("XML Element"));

        dark_mode_toggle_ = sidebar_->footer_->addWidget(std::make_unique<DarkModeToggle>());

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
            std::cout << "sidebar width changed: " << width.toUTF8() << std::endl;
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