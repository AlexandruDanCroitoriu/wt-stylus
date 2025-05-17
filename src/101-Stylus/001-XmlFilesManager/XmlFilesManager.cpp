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
        selected_file_brain_ = xml_file_brains_[state_->xml_node_->Attribute("selected-file-path")];

        
        tree_wrapper_ = grid_layout_->addWidget(std::make_unique<GridItemWrapper>(), 0, 2);
        elem_wrapper_ = grid_layout_->addWidget(std::make_unique<GridItemWrapper>(), 0, 3);
        control_center_ = grid_layout_->addWidget(std::make_unique<ControlCenter>(selected_file_brain_), 0, 4, Wt::AlignmentFlag::Right);
        grid_layout_->setColumnStretch(3, 1);
        
        grid_layout_->setColumnResizable(1, true, Wt::WLength(state_->xml_node_->IntAttribute("editor-width"), Wt::LengthUnit::Pixel));
        grid_layout_->setColumnResizable(2, true, Wt::WLength(state_->xml_node_->IntAttribute("preview-widget-sidebar-width"), Wt::LengthUnit::Pixel));
        grid_layout_->setColumnResizable(3, true, Wt::WLength(Wt::WLength::Auto));
        tree_wrapper_->setStyleClass("overflow-y-auto stylus-background h-screen");
        elem_wrapper_->setStyleClass("overflow-y-auto stylus-background h-screen");
        
        setPreviewWidgets();

        if(state_->xml_node_->BoolAttribute("editor-hidden"))
        {
            editor_->hide();
        }
        if(state_->xml_node_->BoolAttribute("preview-tree-hidden"))
        {
            tree_wrapper_->hide();
        }
        if(state_->xml_node_->BoolAttribute("preview-elem-hidden"))
        {
            elem_wrapper_->hide();
        }
        if(state_->xml_node_->BoolAttribute("control-center-hidden"))
        {
            control_center_->hide();
        }
        

     
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
            setPreviewWidgets(true);
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
        tree_wrapper_->width_changed_.connect(this, [=](int width)
        {
            if(width != state_->xml_node_->IntAttribute("preview-widget-sidebar-width"))
            {
                state_->xml_node_->SetAttribute("preview-widget-sidebar-width", width);
                state_->doc_->SaveFile(state_->state_file_path_.c_str());
            }
        }); 
        folders_changed_.connect(this, [=]()
        {
           setXmlFileBrains();
        });

    }

    void XmlFilesManager::setPreviewWidgets(bool scroll_into_view)
    {
        tree_wrapper_->clear();
        elem_wrapper_->clear();
        
        if(selected_file_path_ == "")
        {
            tree_wrapper_->addWidget(std::make_unique<Wt::WText>("No file selected"));
            elem_wrapper_->addWidget(std::make_unique<Wt::WText>("No file selected"));
            return;
        }
        auto xml_tree_preview_ = tree_wrapper_->addWidget(std::make_unique<XMLTreeNode>(selected_file_brain_, selected_file_brain_->doc_->RootElement(), scroll_into_view));
        auto xml_elem_preview_ = elem_wrapper_->addWidget(std::make_unique<XMLElemNode>(selected_file_brain_, selected_file_brain_->doc_->RootElement(), scroll_into_view));
        
        Wt::WStringStream contextJS;
        contextJS << WT_CLASS << ".$('" << id() << "').oncontextmenu = "
                    << "function() { event.cancelBubble = true; event.returnValue = false; return false; };";
        Wt::WApplication::instance()->doJavaScript(contextJS.str());

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
                    setPreviewWidgets(scroll_into_view);
                });

            }
        }
    }


}