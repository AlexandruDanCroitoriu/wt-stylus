#include "101-Stylus/004-TailwindConfigManager/TailwindConfigManager.h"
#include <Wt/WApplication.h>
#include <Wt/WText.h>
#include <Wt/WPushButton.h>
#include <Wt/WLineEdit.h>
#include <Wt/WRandom.h>
#include <boost/regex.hpp>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <Wt/WTemplate.h>
#include <Wt/WDialog.h>
#include <Wt/WLabel.h>
#include <regex>
#include <Wt/WMessageBox.h>

#include <Wt/WApplication.h>
#include <Wt/WServer.h>
#include <Wt/WIOService.h>

namespace Stylus
{

    TailwindConfigManager::TailwindConfigManager(std::shared_ptr<StylusState> state)
    : state_(state),
    default_config_file_name_("0.css")
    {

        // auto sidebar = addWidget(std::make_unique<WContainerWidget>());
        // auto editors_wrapper = addWidget(std::make_unique<WContainerWidget>());
        auto grid_layout = std::make_unique<Wt::WGridLayout>();

        auto sidebar = grid_layout->addWidget(std::make_unique<Wt::WContainerWidget>(), 0, 0, 1, 2, Wt::AlignmentFlag::Top);
        config_editor_ = grid_layout->addWidget(std::make_unique<MonacoEditor>("css"), 1, 0);
        output_editor_ = grid_layout->addWidget(std::make_unique<MonacoEditor>("css"), 1, 1);

        config_editor_->setMinimumSize(Wt::WLength::Auto, Wt::WLength(100, Wt::LengthUnit::Percentage));
        output_editor_->setMinimumSize(Wt::WLength::Auto, Wt::WLength(100, Wt::LengthUnit::Percentage));

        grid_layout->setColumnResizable(0, true, Wt::WLength(state_->tailwind_config_node_->IntAttribute("editor-width"), Wt::LengthUnit::Pixel));
        grid_layout->setContentsMargins(0, 0, 0, 0);

        grid_layout->setRowStretch(1, 1);

        grid_layout_ = grid_layout.get();

        config_files_ = getConfigFiles();
        
        output_editor_->setEditorReadOnly(true);
        setStyleClass("flex flex-col h-[100vh] stylus-background");
        sidebar->setStyleClass("flex items-center space-x-[10px] stylus-background border-b border-solid");
        // config_editor_->addStyleClass("h-full");
        config_files_combobox_ = sidebar->addWidget(std::make_unique<Wt::WComboBox>());
        config_files_combobox_->setStyleClass("max-w-[240px] m-[4px] bg-[#f9fafb] border border-[#d1d5db] text-[#111827] text-sm rounded-lg focus:ring-[#3b82f6] focus:border-[#3b82f6] block w-full p-[6px] dark:bg-[#374151] dark:border-[#4b5563] dark:placeholder-[#9ca3af] dark:text-[#FFF] dark:focus:ring-[#3b82f6] dark:focus:border-[#3b82f6] disabled:bg-[#374151] disabled:text-[#9ca3af] disabled:border-[#4b5563] disabled:cursor-not-allowed");
        config_files_combobox_->keyWentDown().connect(this, [=](Wt::WKeyEvent event)
        {
            Wt::WApplication::instance()->globalKeyWentDown().emit(event); // Emit the global key event
        });

        std::string btn_styles = "cursor-pointer rounded-[6px] w-[30px] h-[30px] p-[4px] disabled:cursor-not-allowed disabled:bg-[#EC133B]/50 hover:disabled:!bg-[#EC133B]/50";
        auto add_file_btn = sidebar->addWidget(std::make_unique<Wt::WPushButton>(Wt::WString::tr("stylus-svg-add-file"), Wt::TextFormat::XHTML));
        add_file_btn->setAttributeValue("tabindex", "-1");
        add_file_btn->setStyleClass(btn_styles);
        
        delete_file_btn_ = sidebar->addWidget(std::make_unique<Wt::WPushButton>(Wt::WString::tr("stylus-svg-trash"), Wt::TextFormat::XHTML));
        delete_file_btn_->setAttributeValue("tabindex", "-1");
        delete_file_btn_->setStyleClass(btn_styles);

        auto save_file_btn = sidebar->addWidget(std::make_unique<Wt::WPushButton>(Wt::WString::tr("stylus-svg-green-checked"), Wt::TextFormat::XHTML));
        save_file_btn->setAttributeValue("tabindex", "-1");
        save_file_btn->setStyleClass(btn_styles);
        save_file_btn->setDisabled(true);
        save_file_btn->setHidden(true);

        config_editor_->width_changed().connect(this, [=](Wt::WString width)
        {
            state_->tailwind_config_node_->SetAttribute("editor-width", std::stoi(width.toUTF8()));
            state_->doc_->SaveFile(state_->state_file_path_.c_str());
        });

        config_editor_->avalable_save().connect(this, [=]()
        {
            if(config_editor_->unsavedChanges())
            {
                config_files_combobox_->setDisabled(true);
                save_file_btn->setDisabled(false);
                save_file_btn->setHidden(false);
                add_file_btn->setDisabled(true);
                delete_file_btn_->setDisabled(true);
            }
            else
            {
                config_files_combobox_->setDisabled(false);
                save_file_btn->setDisabled(true);
                save_file_btn->setHidden(true);
                delete_file_btn_->setDisabled(true);
                add_file_btn->setDisabled(false);
            }
        });

        config_editor_->save_file_signal().connect(this, [=](std::string file_path)
        {
            std::string file_name = config_files_combobox_->currentText().toUTF8();
            if(file_name == "")
            {
                file_name = default_config_file_name_;
            }
            std::ofstream file(state_->tailwind_config_editor_data_.root_folder_path_ + file_name);
            if (file.is_open())
            {
                file << config_editor_->getUnsavedText();
                file.close();
                config_editor_->textSaved();
                config_files_combobox_->setDisabled(false);
                generateCssFile();
            }
        });

        save_file_btn->clicked().connect(this, [=]()
        {
            config_editor_->save_file_signal().emit(config_editor_->getUnsavedText());
        });
        
        delete_file_btn_->clicked().connect(this, [=]()
        {
            if(config_files_combobox_->currentText().toUTF8().compare(default_config_file_name_) == 0)
            {
                delete_file_btn_->setDisabled(true);
                return;
            }
            auto message_box = addChild(std::make_unique<Wt::WMessageBox>("Are you sure you want to delete the file ?", 
                "<div class='flex-[1] text-center font-bold text-2xl'>" + config_files_combobox_->currentText().toUTF8() + "</div>",
                Wt::Icon::Warning, Wt::StandardButton::None)
            );
            message_box->setOffsets(100, Wt::Side::Top);
            message_box->setModal(true);
        
            message_box->setStyleClass("");
            message_box->titleBar()->setStyleClass("flex items-center justify-center p-[8px] cursor-pointer border-b border-solid text-[18px] font-bold");
            message_box->contents()->addStyleClass("flex items-center");
            message_box->footer()->setStyleClass("flex items-center justify-between p-[8px]");
            auto delete_btn = message_box->addButton("Delete", Wt::StandardButton::Yes);
            auto cancel_btn = message_box->addButton("Cancel", Wt::StandardButton::No);
            delete_btn->setStyleClass("btn-red");
            cancel_btn->setStyleClass("btn-default");
            
            message_box->buttonClicked().connect([=] {
                if (message_box->buttonResult() == Wt::StandardButton::Yes)
                    {
                    std::filesystem::path file_path =  state_->tailwind_config_editor_data_.root_folder_path_ + config_files_combobox_->currentText().toUTF8();
    
                    // delete file
                    if (std::filesystem::remove(file_path)) {
                        getConfigFiles();
                        config_files_combobox_->clear();
                        for(const auto &file : config_files_)
                        {
                            config_files_combobox_->addItem(file);
                        }
                        config_files_combobox_->setCurrentIndex(config_files_combobox_->findText(default_config_file_name_));
                    } else {
                        Wt::WApplication::instance()->log("ERROR") << "\n\nError deleting file.\n\n";                    
                    }
                }
                removeChild(message_box);
            });
            
            message_box->show(); 
        });

        add_file_btn->clicked().connect(this, [=]()
                                        {
            auto dialog = Wt::WApplication::instance()->root()->addChild(std::make_unique<Wt::WDialog>("Create new config file"));
            dialog->setOffsets(100, Wt::Side::Top);
            dialog->setModal(true);
            dialog->rejectWhenEscapePressed();
            dialog->setStyleClass("z-[2]");
            dialog->titleBar()->setStyleClass("flex items-center justify-center p-[8px] cursor-pointer border-b border-solid text-xl font-bold");
            dialog->contents()->setStyleClass("flex flex-col");

            auto content = dialog->contents()->addWidget(std::make_unique<Wt::WContainerWidget>());
            auto footer = dialog->contents()->addWidget(std::make_unique<Wt::WContainerWidget>());
            
            content->setStyleClass("p-[8px]");
            footer->setStyleClass("flex items-center justify-between p-[8px]");
                
            auto input_wrapper = content->addWidget(std::make_unique<Wt::WContainerWidget>());
            input_wrapper->setStyleClass("flex flex-col items-center justify-center");
            auto error_label = content->addWidget(std::make_unique<Wt::WText>(""));
            error_label->setStyleClass("w-full text-md font-semibold");
            
            auto label = input_wrapper->addWidget(std::make_unique<Wt::WLabel>("Name"));
            auto new_file_name_input = input_wrapper->addWidget(std::make_unique<Wt::WLineEdit>());
            new_file_name_input->setStyleClass("w-full min-w-[200px] text-sm border rounded-md px-3 py-2 transition duration-300 ease focus:outline-none shadow-sm focus:shadow");
            label->setBuddy(new_file_name_input);
            
            auto confirm_btn = footer->addWidget(std::make_unique<Wt::WPushButton>("Confirm"));
            confirm_btn->setStyleClass("btn-default");
            auto cancel_btn = footer->addWidget(std::make_unique<Wt::WPushButton>("Cancel"));
            cancel_btn->setStyleClass("btn-red");

            cancel_btn->clicked().connect(this, [=](){ dialog->reject(); });
            new_file_name_input->enterPressed().connect(this, [=](){ confirm_btn->clicked().emit(Wt::WMouseEvent()); });

            confirm_btn->clicked().connect(this, [=](){ 
                // check if the folder already exists
                std::string new_file_name = new_file_name_input->text().toUTF8();
                std::string pattern = R"(^[a-z-]+$)";
                if(std::regex_match(new_file_name, std::regex(pattern)) == false) {
                    error_label->setText("Match reges:" + pattern);
                    return;
                }
                std::filesystem::path new_path(state_->tailwind_config_editor_data_.root_folder_path_ + new_file_name + "." + state_->tailwind_config_editor_data_.extension_);
                if (std::filesystem::exists(new_path)) {
                    error_label->setText("file with the same name already exists.");
                }else {
                    dialog->accept();                
                }
            });
            dialog->finished().connect(this, [=](){
                if (dialog->result() == Wt::DialogCode::Accepted) {
                    std::string new_file_name = new_file_name_input->text().toUTF8();
                    std::filesystem::path new_path(state_->tailwind_config_editor_data_.root_folder_path_ + new_file_name + "." +  state_->tailwind_config_editor_data_.extension_);
                    std::ofstream new_file(new_path);
                    getConfigFiles();

                    config_files_combobox_->clear();
                    for(const auto &file : config_files_)
                    {
                        config_files_combobox_->addItem(file);
                    }
                    config_files_combobox_->setCurrentIndex(config_files_combobox_->findText(new_file_name + "." + state_->tailwind_config_editor_data_.extension_));
                }
                removeChild(dialog);
            });

            dialog->show();
        });

        
        config_files_combobox_->activated().connect(this, [=]()
        {
            std::string file_name = config_files_combobox_->currentText().toUTF8();
            if(file_name == "")
            {
                file_name = default_config_file_name_;
            }
            
            auto resource_url = state_->tailwind_config_editor_data_.root_resource_url_ + file_name;
            auto file_path = state_->tailwind_config_editor_data_.root_folder_path_ + file_name;
            config_editor_->setEditorText(resource_url, state_->getFileText(file_path));
            generateCssFile();

            if(file_name == default_config_file_name_)
            {
                config_editor_->setEditorReadOnly(true);
                delete_file_btn_->setDisabled(true);
            }
            else
            {
                config_editor_->setEditorReadOnly(false);
                delete_file_btn_->setDisabled(false);
            }
            state_->tailwind_config_node_->SetAttribute("selected-file-name", file_name.c_str());
            state_->doc_->SaveFile(state_->state_file_path_.c_str());
            save_file_btn->setDisabled(true);
            save_file_btn->setHidden(true);
        });

        std::string selected_file_name = state_->tailwind_config_node_->Attribute("selected-file-name");
        if(selected_file_name == "")
        {
            selected_file_name = default_config_file_name_;
        }
        for(const auto &file : config_files_)
        {
            config_files_combobox_->addItem(file);
            if(selected_file_name.compare(file) == 0){
                config_files_combobox_->setCurrentIndex(config_files_combobox_->findText(selected_file_name));
                config_editor_->setEditorText(state_->tailwind_config_editor_data_.root_resource_url_ + selected_file_name, state_->getFileText(state_->tailwind_config_editor_data_.root_folder_path_ + selected_file_name));
                output_editor_->setEditorText("static/tailwind.css", state_->getFileText("../static/tailwind.css"));
            }
        }
        
        
        setLayout(std::move(grid_layout));
        
        prev_css_file_ = Wt::WApplication::instance()->docRoot() + "/../static/tailwind.css?v=" + Wt::WRandom::generateId();
        Wt::WApplication::instance()->useStyleSheet(prev_css_file_.toUTF8());
    }
    
    std::vector<std::string> TailwindConfigManager::getConfigFiles()
    {
        std::vector<std::string> config_files;
        for (const auto &entry : std::filesystem::directory_iterator(state_->tailwind_config_editor_data_.root_folder_path_))
        {
            if (entry.is_regular_file())
            {
                config_files.push_back(entry.path().filename().string());
            }
        }
        config_files_ = config_files;
        return config_files;
    }


    void TailwindConfigManager::generateCssFile()
    {

        // std::ofstream file("../static/stylus-resources/tailwind4/input.css");
        std::ofstream file(state_->tailwind_input_file_path_);
        if (!file.is_open())
        {
            std::cerr << "\n\n Error opening file for writing: " << state_->tailwind_input_file_path_ << "\n\n";
            return;
        }   
        file << "/* Import TailwindCSS base styles */\n";
        file << "@import \"tailwindcss\";\n\n";
        file << "/* Import custom CSS files for additional styles */\n";
        
        file << "\n";
        file << "/* Source additional templates and styles */\n";
        file << "@source \"../xml/\";\n";
        file << "@source \"../../../src/\";\n\n";

       
        file << "/* Import custom CSS files */\n";
        for (const auto &css_folder : std::filesystem::directory_iterator(state_->css_editor_data_.root_folder_path_))
        {
            if (css_folder.is_directory())
            {
                std::vector<std::string> files;
                for (const auto &css_file : std::filesystem::directory_iterator(css_folder.path().string()))
                {
                    if (css_file.is_regular_file())
                    {
                    file << "@import \"./css/" << css_folder.path().filename().string() << "/" << css_file.path().filename().string() << "\";\n";
                    }
                }
            }
        }
 
        file << "/* Define custom theme */\n";
        file << state_->getFileText(state_->tailwind_config_editor_data_.root_folder_path_ + config_files_combobox_->currentText().toUTF8()) << "\n\n";
        file.close();

        auto session_id = Wt::WApplication::instance()->sessionId();
        Wt::WServer::instance()->ioService().post([this, session_id](){
            std::system("cd ../static/stylus-resources/tailwind4 && npm run build");
            Wt::WServer::instance()->post(session_id, [this]() {
                current_css_file_ = Wt::WApplication::instance()->docRoot() + "/../static/tailwind.css?v=" + Wt::WRandom::generateId();
                Wt::WApplication::instance()->removeStyleSheet(prev_css_file_.toUTF8());
                Wt::WApplication::instance()->useStyleSheet(current_css_file_.toUTF8());
                prev_css_file_ = current_css_file_;
                output_editor_->setEditorText("static/tailwind.css", state_->getFileText("../static/tailwind.css"));
            }); 
        });
    }


}