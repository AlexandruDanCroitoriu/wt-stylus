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
#include "101-Stylus/001-XmlFilesManager/XmlFileBrain.h"
#include "101-Stylus/001-XmlFilesManager/Preview/XMLElemNode.h"
#include "101-Stylus/001-XmlFilesManager/Preview/XMLTreeNode.h"

namespace Stylus
{

    XmlFilesManager::XmlFilesManager(std::shared_ptr<StylusState> state)
        : state_(state),
        FilesManager(state, state->xml_editor_data_, state->xml_node_->IntAttribute("sidebar-width"), state->xml_node_->Attribute("selected-file-path"))
    {
        sidebar_->footer_->show();
        
        file_brain_ = std::make_shared<XMLFileBrain>(state_);
        file_brain_->setFile(data_.root_folder_path_ + state->xml_node_->Attribute("selected-file-path"));
        // auto temp_wrapper = grid_layout_->addWidget(std::make_unique<Wt::WContainerWidget>(), 0, 2);
        auto xml_tree_preview = grid_layout_->addWidget(std::make_unique<XMLTreeView>(file_brain_), 0, 2);
        auto xml_elem_preview = grid_layout_->addWidget(std::make_unique<XMLElemView>(file_brain_), 0, 3);
        grid_layout_->setColumnResizable(1, true, Wt::WLength(state_->xml_node_->IntAttribute("editor-width"), Wt::LengthUnit::Pixel));
        grid_layout_->setColumnResizable(2, true, Wt::WLength(state_->xml_node_->IntAttribute("preview-widget-sidebar-width"), Wt::LengthUnit::Pixel));
        // temp_wrapper->setStyleClass("p-[8px] stylus-background overflow-y-auto h-screen w-full flex"); 
        // temp_wrapper->setStyleClass("stylus-background overflow-auto h-screen w-full"); 


        // auto temp_view = temp_wrapper->addWidget(std::make_unique<Wt::WTemplate>());
        // file_preview_ = temp_wrapper->addWidget(std::make_unique<XmlFileUi>(state_));
        
 
        dark_mode_toggle_ = sidebar_->footer_->addWidget(std::make_unique<DarkModeToggle>());
        
        // auto preview_checkbox = sidebar_->footer_->addWidget(std::make_unique<Wt::WCheckBox>("Change View"));
        // preview_checkbox->setChecked(state_->xml_node_->BoolAttribute("editor-open"));
        // preview_checkbox->keyWentDown().connect(this, [=](Wt::WKeyEvent e) { 
        //     Wt::WApplication::instance()->globalKeyWentDown().emit(e); // Emit the global key event
        // });

        // if(std::string(state_->xml_node_->Attribute("preview-type")).compare("template") == 0) {
        //     file_preview_->hide();
        //     temp_view->show();
        //     preview_checkbox->setChecked(false);
        // }else if(std::string(state_->xml_node_->Attribute("preview-type")).compare("widgets") == 0) {
        //     file_preview_->show();
        //     temp_view->hide();
        //     preview_checkbox->setChecked(true);
        // }
        // temp_view->setTemplateText(editor_->getUnsavedText(), Wt::TextFormat::UnsafeXHTML);
        // file_preview_->setFile(data_.root_folder_path_ + selected_file_path_);
        

        // preview_checkbox->changed().connect(this, [=]()
        // {
        //     if (preview_checkbox->isChecked())
        //     {
        //         state_->xml_node_->SetAttribute("preview-type", "widgets");
        //         file_preview_->show();
        //         temp_view->hide();
        //     }
        //     else
        //     {
        //         state_->xml_node_->SetAttribute("preview-type", "template");
        //         file_preview_->hide();
        //         temp_view->show();
        //     }
        //     state_->doc_.SaveFile(state_->state_file_path_.c_str());
        // });

        xml_tree_preview->width_changed_.connect(this, [=](int width)
        {
            if(width != state_->xml_node_->IntAttribute("preview-widget-sidebar-width"))
            {
                state_->xml_node_->SetAttribute("preview-widget-sidebar-width", width);
                state_->doc_.SaveFile(state_->state_file_path_.c_str());
            }
        }); 
        file_selected().connect(this, [=]()
        {
            std::string file_path = data_.root_folder_path_ + selected_file_path_;
            state_->xml_node_->SetAttribute("selected-file-path", selected_file_path_.c_str());
            file_brain_->setFile(file_path);
            xml_elem_preview->resetUi();
            xml_tree_preview->resetUi();
            // temp_view->setTemplateText(editor_->getUnsavedText(), Wt::TextFormat::UnsafeXHTML);
            // file_preview_->setFile(file_path);
            state_->doc_.SaveFile(state_->state_file_path_.c_str());
        });
        file_saved().connect(this, [=](Wt::WString file_path)
        {
            std::cout << "\n\nFile saved:sasdsad " << file_path.toUTF8() << "\n\n";
            file_brain_->setFile(data_.root_folder_path_ +  file_path.toUTF8());
            xml_elem_preview->resetUi();
            xml_tree_preview->resetUi();
            // temp_view->setTemplateText(editor_->getUnsavedText(), Wt::TextFormat::UnsafeXHTML);
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