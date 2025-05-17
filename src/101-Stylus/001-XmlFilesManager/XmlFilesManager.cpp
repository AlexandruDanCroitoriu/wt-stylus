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

#include "101-Stylus/001-XmlFilesManager/Preview/ControlCenter.h"

namespace Stylus
{

    XmlFilesManager::XmlFilesManager(std::shared_ptr<StylusState> state)
        : state_(state),
        FilesManager(state, state->xml_editor_data_, state->xml_node_->IntAttribute("sidebar-width"), state->xml_node_->Attribute("selected-file-path"))
    {
        setXmlFileBrains();

        
        // file_brain_ = std::make_shared<XMLFileBrain>(state_);
        // file_brain_->setFile(data_.root_folder_path_ + state->xml_node_->Attribute("selected-file-path"));
        // auto temp_wrapper = grid_layout_->addWidget(std::make_unique<Wt::WContainerWidget>(), 0, 2);
        tree_wrapper_ = grid_layout_->addWidget(std::make_unique<Wt::WContainerWidget>(), 0, 2);
        elem_wrapper_ = grid_layout_->addWidget(std::make_unique<Wt::WContainerWidget>(), 0, 3);
        auto control_center = grid_layout_->addWidget(std::make_unique<ControlCenter>(selected_file_brain_), 0, 4);
        
        grid_layout_->setColumnResizable(1, true, Wt::WLength(state_->xml_node_->IntAttribute("editor-width"), Wt::LengthUnit::Pixel));
        grid_layout_->setColumnResizable(2, true, Wt::WLength(state_->xml_node_->IntAttribute("preview-widget-sidebar-width"), Wt::LengthUnit::Pixel));

        selected_file_brain_ = xml_file_brains_[state_->xml_node_->Attribute("selected-file-path")];

        setPreviewWidgets();
     
        file_selected().connect(this, [=]()
        {
            std::string file_path = data_.root_folder_path_ + selected_file_path_;
            state_->xml_node_->SetAttribute("selected-file-path", selected_file_path_.c_str());
            state_->doc_->SaveFile(state_->state_file_path_.c_str());
            // selected_file_brain_->setFile(file_path);
            selected_file_brain_ = xml_file_brains_[selected_file_path_];
            setPreviewWidgets();
        });
        file_saved().connect(this, [=](Wt::WString file_path)
        {
            std::cout << "\n\nFile saved:sasdsad " << file_path.toUTF8() << "\n\n";
            selected_file_brain_->setFile(data_.root_folder_path_ +  file_path.toUTF8());
            setPreviewWidgets();
        });
        
        sidebar_->width_changed().connect(this, [=](Wt::WString width)
        {
            std::cout << "sidebar width changed: " << width.toUTF8() << std::endl;
            if(std::stoi(width.toUTF8()) != state_->xml_node_->IntAttribute("sidebar-width"))
            {
                state_->xml_node_->SetAttribute("sidebar-width", std::stoi(width.toUTF8()));
                state_->doc_->SaveFile(state_->state_file_path_.c_str());
            }
        });

        editor_->width_changed().connect(this, [=](Wt::WString width)
        {
            if(std::stoi(width.toUTF8()) != state_->xml_node_->IntAttribute("editor-width"))
            {
                state_->xml_node_->SetAttribute("editor-width", std::stoi(width.toUTF8()));
                state_->doc_->SaveFile(state_->state_file_path_.c_str());
            }
        });

        folders_changed_.connect(this, [=]()
        {
           setXmlFileBrains();
        });

    }

    void XmlFilesManager::setPreviewWidgets()
    {
        tree_wrapper_->clear();
        elem_wrapper_->clear();
        
        xml_tree_preview_ = tree_wrapper_->addWidget(std::make_unique<XMLTreeView>(selected_file_brain_));
        xml_elem_preview_ = elem_wrapper_->addWidget(std::make_unique<XMLElemView>(selected_file_brain_));

        xml_tree_preview_->width_changed_.connect(this, [=](int width)
        {
            if(width != state_->xml_node_->IntAttribute("preview-widget-sidebar-width"))
            {
                state_->xml_node_->SetAttribute("preview-widget-sidebar-width", width);
                state_->doc_->SaveFile(state_->state_file_path_.c_str());
            }
        }); 
    }


    void XmlFilesManager::setXmlFileBrains()
    {
        xml_file_brains_.clear();
        std::string file_path;
        std::shared_ptr<XMLFileBrain> file_brain;
        for(auto folder : folders_)
        {
            for(const auto &file : folder.second)
            {
                file_path = folder.first + "/" + file;
                file_brain = std::make_shared<XMLFileBrain>(state_, data_.root_folder_path_ + file_path);
                // file_brain->setFile(data_.root_folder_path_ + file_path);
                xml_file_brains_[folder.first + "/" + file] = file_brain;
                file_brain->file_saved_.connect(this, [=]()
                {
                    reuploadFile(); // reffers to monaco editor
                    selected_file_brain_->xml_node_selected_.emit(selected_file_brain_->selected_node_, true);
                });

                file_brain->xml_node_selected_.connect(this, [=](tinyxml2::XMLNode* node, bool scroll_into_view)
                {
                    selected_file_brain_ = file_brain;
                    selected_file_brain_->selected_node_ = node;
                    setPreviewWidgets();
                });

            }
        }
    }


}