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
#include <Wt/WStringStream.h>
#include <regex>

namespace Stylus
{

    XmlFilesManager::XmlFilesManager(std::shared_ptr<StylusState> state)
        : state_(state),
        FilesManager(state, state->xml_editor_data_, state->xml_node_->IntAttribute("sidebar-width"), state->xml_node_->Attribute("selected-file-path"))
    {
        std::cout << "\n\nXmlFilesManager initialized successfully.\n\n";
        setXmlFileBrains();
        tree_wrapper_ = grid_layout_->addWidget(std::make_unique<GridItemWrapper>(), 0, 2);
        elem_wrapper_ = grid_layout_->addWidget(std::make_unique<GridItemWrapper>(), 0, 3);
        control_center_ = grid_layout_->addWidget(std::make_unique<ControlCenter>(), 0, 4, Wt::AlignmentFlag::Right);
        template_var_control_center_ = grid_layout_->addWidget(std::make_unique<TemplateVarControlCenter>(), 0, 5, Wt::AlignmentFlag::Right);
        grid_layout_->setColumnStretch(3, 1);

        editor_->addStyleClass("w-full min-w-full");
        editor_->setMinimumSize(Wt::WLength::Auto, Wt::WLength(100, Wt::LengthUnit::ViewportHeight));

        grid_layout_->setColumnResizable(1, true, Wt::WLength(state_->xml_node_->IntAttribute("editor-width"), Wt::LengthUnit::Pixel));
        grid_layout_->setColumnResizable(2, true, Wt::WLength(state_->xml_node_->IntAttribute("preview-widget-sidebar-width"), Wt::LengthUnit::Pixel));
        
        tree_wrapper_->setStyleClass("overflow-y-auto stylus-scrollbar h-screen select-none");
        elem_wrapper_->setStyleClass("overflow-y-auto stylus-scrollbar h-screen select-none m-1 bg-surface dark:bg-surface-dark text-on-surface dark:text-on-surface-dark");
        
        // Wt::WStringStream contextJS;
        // contextJS << WT_CLASS << ".$('" << id() << "').oncontextmenu = "
        //             << "function() { event.cancelBubble = true; event.returnValue = false; return false; };";
        // Wt::WApplication::instance()->doJavaScript(contextJS.str());

        selected_file_brain_ = state_->xml_file_brains_[state_->xml_node_->Attribute("selected-file-path")];
        if(selected_file_brain_ && selected_file_brain_->selected_node_)
        {
            state_->organizeXmlNode(selected_file_brain_->doc_->RootElement(), selected_file_brain_->file_path_);
            selected_file_brain_->doc_->SaveFile(std::string("../test.xml").c_str(), true);
        }
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
        if(state_->xml_node_->BoolAttribute("template-var-control-center-hidden"))
        {
            template_var_control_center_->hide();
        }
     
        file_selected().connect(this, [=]()
        {
            std::string file_path = data_.root_folder_path_ + selected_file_path_;
            state_->xml_node_->SetAttribute("selected-file-path", selected_file_path_.c_str());
            state_->doc_->SaveFile(state_->state_file_path_.c_str());
            selected_file_brain_ = state_->xml_file_brains_[selected_file_path_];
            if(selected_file_brain_->doc_->RootElement())
            {
                state_->organizeXmlNode(selected_file_brain_->doc_->RootElement(), selected_file_brain_->file_path_);
                selected_file_brain_->doc_->SaveFile(std::string("../test.xml").c_str());
            }
            setPreviewWidgets();
        });
        
        file_saved().connect(this, [=](Wt::WString file_path)
        {
            std::cout << "\n\nFile saved:sasdsad " << file_path.toUTF8() << "\n\n";
            selected_file_brain_->setFile(data_.root_folder_path_ +  file_path.toUTF8());
            if(selected_file_brain_->doc_->RootElement())
            {
                state_->organizeXmlNode(selected_file_brain_->doc_->RootElement(), selected_file_brain_->file_path_);
                selected_file_brain_->doc_->SaveFile(std::string("../test.xml").c_str(), true);
            }
            setPreviewWidgets();
        });
        
        sidebar_->width_changed().connect(this, [=](Wt::WString width)
        {
            std::cout << "\n\nsidebar width changed: " << width.toUTF8() << std::endl;
            if(std::stoi(width.toUTF8()) != state_->xml_node_->IntAttribute("sidebar-width"))
            {
                state_->xml_node_->SetAttribute("sidebar-width", std::stoi(width.toUTF8()));
                state_->doc_->SaveFile(state_->state_file_path_.c_str());
            }
        });

        editor_->width_changed().connect(this, [=](Wt::WString width)
        {
            std::cout << "\n\nEditor width changed: " << width.toUTF8() << std::endl;
            if(std::stoi(width.toUTF8()) != state_->xml_node_->IntAttribute("editor-width"))
            {
                state_->xml_node_->SetAttribute("editor-width", std::stoi(width.toUTF8()));
                state_->doc_->SaveFile(state_->state_file_path_.c_str());
            }
        });
        tree_wrapper_->width_changed_.connect(this, [=](int width)
        {
            std::cout << "\n\nTree width changed: " << width << std::endl;
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
        if(selected_file_path_.compare("") == 0 || !selected_file_brain_ || !selected_file_brain_->doc_ || selected_file_brain_->doc_->Error() != tinyxml2::XML_SUCCESS)
        {
            std::string xml_error;
            if(selected_file_brain_ && selected_file_brain_->doc_) {
                xml_error = selected_file_brain_->doc_->ErrorStr();
                std::cout << "\n\nXML Error: " << xml_error << std::endl;
            } else {
                xml_error = "No file selected or file could not be loaded.";
            }
            
            // place words split by whitespace in vector
            std::regex re("\\s+");
            std::sregex_token_iterator it(xml_error.begin(), xml_error.end(), re, -1);
            std::vector<std::string> words(it, {});
            Wt::WStringStream error_stream;
            error_stream << "<div class='flex flex-col space-y-2'>";
            for(auto word : words)
            {
                error_stream << "<div class=''>" << word << "</div>";
            }
            error_stream << "</div>";
            xml_error = error_stream.str();
            

            tree_wrapper_->addWidget(std::make_unique<Wt::WText>(xml_error));
            elem_wrapper_->addWidget(std::make_unique<Wt::WText>(xml_error));
            control_center_->disableAll();
            template_var_control_center_->disableAll();
            return;
        }
        auto xml_tree_preview_ = tree_wrapper_->addWidget(std::make_unique<XMLTreeNode>(selected_file_brain_, selected_file_brain_->doc_->RootElement(), scroll_into_view));
        auto xml_elem_preview_ = elem_wrapper_->addWidget(std::make_unique<XMLElemNode>(selected_file_brain_, selected_file_brain_->doc_->RootElement(), scroll_into_view));
        
        control_center_->setFileBrain(selected_file_brain_);
        template_var_control_center_->setFileBrain(selected_file_brain_);

    }


    void XmlFilesManager::setXmlFileBrains()
    {
        state_->xml_file_brains_.clear();
        std::string file_path;
        std::shared_ptr<XMLFileBrain> file_brain;
        for(auto folder : folders_)
        {
            // std::cout << "\nFolder: " << folder.first << std::endl;
            for(const auto &file : folder.second)
            {
                // std::cout << "File: " << file << std::endl;
                file_path = folder.first + "/" + file;
                file_brain = std::make_shared<XMLFileBrain>(state_, data_.root_folder_path_ + file_path);
                state_->xml_file_brains_[folder.first + "/" + file] = file_brain;
                // std::cout << "\n\nFile path: " << folder.first + "/" + file << std::endl;
                file_brain->file_saved_.connect(this, [=]()
                {
                    reuploadFile(); // reffers to monaco editor
                    // state_->file_saved_.emit();
                    selected_file_brain_->xml_node_selected_.emit(selected_file_brain_->selected_node_, true);
                });

                file_brain->xml_node_selected_.connect(this, [=](tinyxml2::XMLNode* node, bool scroll_into_view)
                {
                    selected_file_brain_ = file_brain;
                    selected_file_brain_->selected_node_ = node->ToElement();
                    setPreviewWidgets(scroll_into_view);
                });
                if(selected_file_path_.compare(file_path) == 0)
                {
                    selected_file_brain_ = file_brain;
                }

            }
        }
    }


}