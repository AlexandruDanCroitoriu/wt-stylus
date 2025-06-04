#include "101-Stylus/000-Utils/FilesManager.h"
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
#include <Wt/WIconPair.h>
#include <Wt/WLineEdit.h>
#include <Wt/WMessageBox.h>
#include <Wt/WDialog.h>
#include <Wt/WIcon.h>

namespace Stylus
{

FilesManagerSidebar::FilesManagerSidebar()
{
    setStyleClass("flex flex-col max-h-screen stylus-background select-none");
    setLayoutSizeAware(true);
    // tree header
    // setMinimumSize(Wt::WLength(240, Wt::LengthUnit::Pixel), Wt::WLength(100, Wt::LengthUnit::ViewportHeight));
    setMinimumSize(Wt::WLength(240, Wt::LengthUnit::Pixel), Wt::WLength::Auto);
    setMaximumSize(Wt::WLength(1000, Wt::LengthUnit::Pixel), Wt::WLength(100, Wt::LengthUnit::ViewportHeight));

    contents_ = addWidget(std::make_unique<Wt::WContainerWidget>());
    contents_->setStyleClass("w-full flex-[1] overflow-y-auto overflow-x-hidden flex flex-col stylus-background");

    footer_ = addWidget(std::make_unique<Wt::WContainerWidget>());
    footer_->setStyleClass("flex items-center justify-between p-[3px] border-t border-solid");
    footer_->hide();
}

void FilesManagerSidebar::layoutSizeChanged(int width, int height)
{
    if(width >= 240 && width <= 1000) {
        width_changed_.emit(std::to_string(width));
    }
}


TreeNode::TreeNode(std::string name, TreeNodeType type, std::string path)
    : Wt::WTreeNode(name),
    type_(type),
    path_(path)
{
    // setStyleClass("relative");
    label_wrapper_ = labelArea();
    label_wrapper_->setStyleClass("flex items-center cursor-pointer mr-[3px] ");


    if(type_ == TreeNodeType::Folder) 
    {
        setSelectable(false);
        // if(name.compare(path_) != 0)
        // {
            // std::cout << "\n\nFolder name: " << name << " path: " << path_ << std::endl;
            acceptDrops("file", "Wt-item");
        // }
    }else if (type_ == TreeNodeType::File) 
    {
        setSelectable(true);
    }
    setParentWidget(parentNode());

    selected().connect(this, [=](bool selected)
    {
        if(selected)
        {
            auto drag_handle = label_wrapper_->addWidget(std::make_unique<Wt::WTemplate>(tr("stylus-svg-drag-handle")));
            drag_handle->setStyleClass("w-4 h-4 flex items-center justify-center ml-auto"); 
            drag_handle->clicked().preventPropagation();
            drag_handle->setDraggable("file", this, false, this);
        }
    });

    label_wrapper_->mouseWentUp().connect(this, [=](const Wt::WMouseEvent& event)
    {
        if (event.button() == Wt::MouseButton::Right) {
            showPopup(event);
        }
    });

    std::unique_ptr<Wt::WIconPair> icon;
    if(type_ == TreeNodeType::Folder)
    {
        setLabelIcon(std::make_unique<Wt::WIconPair>("./static/stylus/yellow-folder-closed.png", "./static/stylus/yellow-folder-open.png", false));
    }else if(type_ == TreeNodeType::File)
    {
        setLabelIcon(std::make_unique<Wt::WIconPair>("static/stylus/document.png", "static/stylus/document.png", false));
    }
}

void TreeNode::dropEvent(Wt::WDropEvent event)
{
    TreeNode* source_node = static_cast<TreeNode*>(event.source());

    if(source_node->parentNode() != this && label()->text().toUTF8().compare(path_) != 0)
    {
        Wt::WTreeNode* new_node;
        std::unique_ptr<Wt::WTreeNode> removed_node = source_node->parentNode()->removeChildNode(source_node);
        new_node = addChildNode(std::move(removed_node));

        // check if there is a file with the same name
        std::string new_file_name = source_node->label()->text().toUTF8();
        std::string new_file_path = path_ + new_file_name;
        if (std::filesystem::exists(new_file_path)) {
            auto message_box = addChild(std::make_unique<Wt::WMessageBox>(
                "File already exists", 
                R"(
                    <div class='flex-1 text-center font-bold text-2xl'>)" + new_file_name + R"(</div>
                )",
                Wt::Icon::Warning, Wt::StandardButton::None));
            message_box->setOffsets(100, Wt::Side::Top);
            message_box->setModal(true);

            message_box->setStyleClass("stylus-background");
            message_box->titleBar()->setStyleClass("flex items-center justify-center p-[8px] cursor-pointer border-b border-solid text-xl font-bold");
            message_box->contents()->setStyleClass("flex flex-col");
            auto content = message_box->contents()->addWidget(std::make_unique<Wt::WContainerWidget>());
            auto footer = message_box->contents()->addWidget(std::make_unique<Wt::WContainerWidget>());
            content->setStyleClass("p-[8px]");
            footer->setStyleClass("flex items-center justify-between p-[8px]");
            auto error_label = content->addWidget(std::make_unique<Wt::WText>(""));
            error_label->setStyleClass("w-full text-[#B22222] text-md font-semibold");
            auto label = content->addWidget(std::make_unique<Wt::WLabel>("File with the same name already exists. Do you want to replace it?"));
            auto confirm_btn = footer->addWidget(std::make_unique<Wt::WPushButton>("Confirm"));
            confirm_btn->setStyleClass("btn-default");
            auto cancel_btn = footer->addWidget(std::make_unique<Wt::WPushButton>("Cancel"));
            cancel_btn->setStyleClass("btn-red");
            cancel_btn->clicked().connect(this, [=](){ message_box->reject(); });
            confirm_btn->clicked().connect(this, [=](){ 
                message_box->accept();
            });
            message_box->finished().connect(this, [=](){
                if (message_box->result() == Wt::DialogCode::Accepted) {
                    std::filesystem::path old_path(source_node->path_ + source_node->label()->text().toUTF8());
                    std::filesystem::path new_path(path_ + this->label()->text().toUTF8() + "/" + source_node->label()->text().toUTF8());
                    std::filesystem::rename(old_path, new_path);
                    folders_changed_.emit(this->label()->text().toUTF8() + "/" + source_node->label()->text().toUTF8());
                }else {
                    source_node->selected().emit(true);
                }
                removeChild(message_box);
            });
            message_box->show();
            return;
        }


        // move file to this folder
        std::filesystem::path old_path(source_node->path_ + source_node->label()->text().toUTF8());
        std::filesystem::path new_path(path_ + label()->text().toUTF8() + "/" + source_node->label()->text().toUTF8());
        std::filesystem::rename(old_path, new_path);
        folders_changed_.emit(label()->text().toUTF8() + "/" + source_node->label()->text().toUTF8());
    }else {
        source_node->selected().emit(true);
    }
    
}

void TreeNode::showPopup(const Wt::WMouseEvent& event)
{
    if (!popup_) {
        popup_ = std::make_unique<Wt::WPopupMenu>();

        if(type_ == TreeNodeType::Folder)
        {
            if(label()->text().toUTF8().compare(path_) == 0)
            {
                popup_->addItem("Create New Folder")->clicked().connect(this, [=](){ createNewFolderDialog(); });
            }else {
                popup_->addItem("Create New File")->clicked().connect(this, [=](){ createNewFileDialog(); });
                popup_->addItem("Rename Folder")->clicked().connect(this, [=]() { createRenameFolderDialog(); });
                popup_->addSeparator();
                popup_->addItem("Delete Folder")->clicked().connect(this, [=]() { createRemoveFolderMessageBox(); });
            }
        }
        else if(type_ == TreeNodeType::File)
        {
            popup_->addItem("copy import to clipboard")->clicked().connect(this, [=]() { copyFilePathToClipboard(); });
            popup_->addItem("Rename File")->clicked().connect(this, [=]() { createRenameFileDialog(); });
            popup_->addSeparator();
            popup_->addItem("Delete File")->clicked().connect(this, [=]() { deleteFileMessageBox(); });
        }

    }

    if (popup_->isHidden())
        popup_->popup(event);
    else
        popup_->hide();
}


void TreeNode::createNewFolderDialog()
{
    auto dialog = Wt::WApplication::instance()->root()->addChild(std::make_unique<Wt::WDialog>("Create new folder"));

    dialog->setModal(true);
    dialog->rejectWhenEscapePressed();
    dialog->setOffsets(100, Wt::Side::Top);

    dialog->setStyleClass("stylus-background");
    dialog->titleBar()->setStyleClass("flex items-center justify-center p-[8px] cursor-pointer border-b border-solid text-xl font-bold");
    dialog->contents()->setStyleClass("flex flex-col");

    auto content = dialog->contents()->addWidget(std::make_unique<Wt::WContainerWidget>());
    auto footer = dialog->contents()->addWidget(std::make_unique<Wt::WContainerWidget>());
    
    content->setStyleClass("p-[8px]");
    footer->setStyleClass("flex items-center justify-between p-[8px]");
    
    auto input_wrapper = content->addWidget(std::make_unique<Wt::WContainerWidget>());
    input_wrapper->setStyleClass("flex flex-col items-center justify-center");
    auto error_label = content->addWidget(std::make_unique<Wt::WText>(""));
    error_label->setStyleClass("w-full text-[#B22222] text-md font-semibold");
    
    auto label = input_wrapper->addWidget(std::make_unique<Wt::WLabel>("Name"));
    auto new_folder_name_input = input_wrapper->addWidget(std::make_unique<Wt::WLineEdit>());
    new_folder_name_input->setStyleClass("w-full min-w-[200px] placeholder:text-slate-400 text-sm border rounded-md px-3 py-2 transition duration-300 ease focus:outline-none shadow-sm");
    label->setBuddy(new_folder_name_input);
    
    auto confirm_btn = footer->addWidget(std::make_unique<Wt::WPushButton>("Confirm"));
    confirm_btn->setStyleClass("btn-default");
    auto cancel_btn = footer->addWidget(std::make_unique<Wt::WPushButton>("Cancel"));
    cancel_btn->setStyleClass("btn-red");

    cancel_btn->clicked().connect(this, [=](){ dialog->reject(); });
    new_folder_name_input->enterPressed().connect(this, [=](){ confirm_btn->clicked().emit(Wt::WMouseEvent()); });
    confirm_btn->clicked().connect(this, [=](){ 
        // check if the folder already exists
        std::string new_folder_name = new_folder_name_input->text().toUTF8();
        std::string pattern = R"(^[a-z0-9-_]+$)";
        if(std::regex_match(new_folder_name, std::regex(pattern)) == false) {
            error_label->setText("Match reges:" + pattern);
            return;
        }
        std::filesystem::path new_path(path_ + new_folder_name);
        if (std::filesystem::exists(new_path)) {
            error_label->setText("Folder with the same name already exists.");
        }else {
            dialog->accept();                
        }
    });
    dialog->finished().connect(this, [=](){
        if (dialog->result() == Wt::DialogCode::Accepted) {
            std::string new_folder_name = new_folder_name_input->text().toUTF8();
            std::filesystem::path new_path(path_ + new_folder_name);
            std::filesystem::create_directory(new_path);
            folders_changed_.emit("");
        }
        removeChild(dialog);
    });
    dialog->show(); 
}
void TreeNode::createRenameFolderDialog()
{
    auto dialog = Wt::WApplication::instance()->root()->addChild(std::make_unique<Wt::WDialog>("Create new folder"));

    dialog->setModal(true);
    dialog->rejectWhenEscapePressed();
    dialog->setOffsets(100, Wt::Side::Top);

    dialog->setStyleClass("stylus-background");
    dialog->titleBar()->setStyleClass("flex items-center justify-center p-[8px] cursor-pointer border-b border-solid text-xl font-bold");
    dialog->contents()->setStyleClass("flex flex-col");

    auto content = dialog->contents()->addWidget(std::make_unique<Wt::WContainerWidget>());
    auto footer = dialog->contents()->addWidget(std::make_unique<Wt::WContainerWidget>());
    
    content->setStyleClass("p-[8px]");
    footer->setStyleClass("flex items-center justify-between p-[8px]");
    
    auto input_wrapper = content->addWidget(std::make_unique<Wt::WContainerWidget>());
    input_wrapper->setStyleClass("flex flex-col items-center justify-center");
    auto error_label = content->addWidget(std::make_unique<Wt::WText>(""));
    error_label->setStyleClass("w-full text-[#B22222] text-md font-semibold");
    
    auto label = input_wrapper->addWidget(std::make_unique<Wt::WLabel>("current name: " + this->label()->text()));
    auto new_folder_name_input = input_wrapper->addWidget(std::make_unique<Wt::WLineEdit>(this->label()->text()));
    new_folder_name_input->setStyleClass("w-full min-w-[200px] placeholder:text-slate-400 text-sm border rounded-md px-3 py-2 transition duration-300 ease focus:outline-none shadow-sm");
    label->setBuddy(new_folder_name_input);
    
    auto confirm_btn = footer->addWidget(std::make_unique<Wt::WPushButton>("Confirm"));
    confirm_btn->setStyleClass("btn-default");
    auto cancel_btn = footer->addWidget(std::make_unique<Wt::WPushButton>("Cancel"));
    cancel_btn->setStyleClass("btn-red");

    cancel_btn->clicked().connect(this, [=](){ dialog->reject(); });
    new_folder_name_input->enterPressed().connect(this, [=](){ confirm_btn->clicked().emit(Wt::WMouseEvent()); });
    confirm_btn->clicked().connect(this, [=](){ 
        // check if the folder already exists
        std::string new_folder_name = new_folder_name_input->text().toUTF8();
        std::string pattern = R"(^[a-z0-9-_]+$)";
        if(std::regex_match(new_folder_name, std::regex(pattern)) == false) {
            error_label->setText("Match reges:" + pattern);
            return;
        }
        std::filesystem::path new_path(path_ + new_folder_name);
        if (std::filesystem::exists(new_path)) {
            error_label->setText("Folder with the same name already exists.");
        }else {
            dialog->accept();                
        }
    });
    dialog->finished().connect(this, [=](){
        if (dialog->result() == Wt::DialogCode::Accepted) {
            std::string new_folder_name = new_folder_name_input->text().toUTF8();
            std::filesystem::path old_path(path_ + this->label()->text().toUTF8());
            std::filesystem::path new_path(path_ + new_folder_name);
            std::filesystem::rename(old_path, new_path);
            this->label()->setText(new_folder_name);
            folders_changed_.emit("");
        }
        removeChild(dialog);
    });
    dialog->show(); 

}
void TreeNode::createRemoveFolderMessageBox()
{
    auto message_box = addChild(std::make_unique<Wt::WMessageBox>(
        "Rename folder ?", 
        R"(
            <div class='flex-1 text-center font-bold text-2xl'>)" + label()->text() + R"(</div>
        )",
        Wt::Icon::Warning, Wt::StandardButton::None));
    message_box->setOffsets(100, Wt::Side::Top);
    message_box->setModal(true);

    message_box->setStyleClass("stylus-background");
    message_box->titleBar()->setStyleClass("flex items-center justify-center p-[8px] cursor-pointer border-b border-solid text-xl font-bold");
    message_box->contents()->addStyleClass("flex items-center");
    message_box->footer()->setStyleClass("flex items-center justify-between p-[8px]");

    auto delete_btn = message_box->addButton("Delete", Wt::StandardButton::Yes);
    auto cancel_btn = message_box->addButton("Cancel", Wt::StandardButton::No);
    delete_btn->setStyleClass("btn-red");
    cancel_btn->setStyleClass("btn-default");
    
    message_box->buttonClicked().connect([=] {
        if (message_box->buttonResult() == Wt::StandardButton::Yes)
            {
            std::filesystem::path file_path = path_ + label()->text().toUTF8();

            // delete folder and all its contents
            std::filesystem::remove_all(file_path);
            folders_changed_.emit("");
            
        }
        removeChild(message_box);
    });
    
    message_box->show(); 
}

void TreeNode::createNewFileDialog()
{
    auto dialog = Wt::WApplication::instance()->root()->addChild(std::make_unique<Wt::WDialog>("Create new file in folder " + label()->text()));
    dialog->setOffsets(100, Wt::Side::Top);
    dialog->setModal(true);
    dialog->rejectWhenEscapePressed();
    dialog->setStyleClass("stylus-background");
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
        // check if the file name already exists
        std::string new_file_name = new_file_name_input->text().toUTF8();
        std::string pattern = R"(^[a-z-.]+$)";
        if(std::regex_match(new_file_name, std::regex(pattern)) == false) {
            error_label->setText("Match reges:" + pattern);
            return;
        }
        std::filesystem::path new_path(path_ + this->label()->text().toUTF8() + "/" +  new_file_name);
        if (std::filesystem::exists(new_path)) {
            error_label->setText("file with the same name already exists.");
        }else {
            dialog->accept();                
        }
    });
    dialog->finished().connect(this, [=](){
        if (dialog->result() == Wt::DialogCode::Accepted) {
            std::string new_file_name = new_file_name_input->text().toUTF8();
            std::filesystem::path new_path(path_ + this->label()->text().toUTF8() + "/" + new_file_name);
            std::ofstream new_file(new_path);
            folders_changed_.emit(this->label()->text().toUTF8() + "/" + new_file_name);
        }
        removeChild(dialog);
    });

    dialog->show();
}
void TreeNode::createRenameFileDialog()
{
    auto dialog = Wt::WApplication::instance()->root()->addChild(std::make_unique<Wt::WDialog>("Rename File: " + label()->text()));

    dialog->setModal(true);
    dialog->rejectWhenEscapePressed();
    dialog->setOffsets(100, Wt::Side::Top);

    dialog->setStyleClass("stylus-background");
    dialog->titleBar()->setStyleClass("flex items-center justify-center p-[8px] cursor-pointer border-b border-solid text-xl font-bold");
    dialog->contents()->setStyleClass("flex flex-col");

    auto content = dialog->contents()->addWidget(std::make_unique<Wt::WContainerWidget>());
    auto footer = dialog->contents()->addWidget(std::make_unique<Wt::WContainerWidget>());
    
    content->setStyleClass("p-[8px]");
    footer->setStyleClass("flex items-center justify-between p-[8px]");
    
    auto input_wrapper = content->addWidget(std::make_unique<Wt::WContainerWidget>());
    input_wrapper->setStyleClass("flex flex-col items-center justify-center");
    auto error_label = content->addWidget(std::make_unique<Wt::WText>(""));
    error_label->setStyleClass("w-full text-[#B22222] text-md font-semibold");
    
    auto new_file_name_input = input_wrapper->addWidget(std::make_unique<Wt::WLineEdit>(this->label()->text()));
    new_file_name_input->setStyleClass("w-full min-w-[200px] placeholder:text-slate-400 text-sm border rounded-md px-3 py-2 transition duration-300 ease focus:outline-none shadow-sm");
    
    auto confirm_btn = footer->addWidget(std::make_unique<Wt::WPushButton>("Confirm"));
    confirm_btn->setStyleClass("btn-default");
    auto cancel_btn = footer->addWidget(std::make_unique<Wt::WPushButton>("Cancel"));
    cancel_btn->setStyleClass("btn-red");

    cancel_btn->clicked().connect(this, [=](){ dialog->reject(); });
    new_file_name_input->enterPressed().connect(this, [=](){ confirm_btn->clicked().emit(Wt::WMouseEvent()); });
    confirm_btn->clicked().connect(this, [=](){ 
        // check if the file name already exists
        std::string new_file_name = new_file_name_input->text().toUTF8();
        std::string pattern = R"(^[a-z-.]+$)";
        if(std::regex_match(new_file_name, std::regex(pattern)) == false) {
            error_label->setText("Match reges:" + pattern);
            return;
        }
        std::filesystem::path new_path(path_ + new_file_name);
        if (std::filesystem::exists(new_path)) {
            error_label->setText("file with the same name already exists.");
        }else {
            dialog->accept();                
        }
    });
    dialog->finished().connect(this, [=](){
        if (dialog->result() == Wt::DialogCode::Accepted) {
            std::string new_file_name = new_file_name_input->text().toUTF8();
            std::filesystem::path old_path(path_ + this->label()->text().toUTF8());
            std::filesystem::path new_path(path_ + new_file_name);
            std::filesystem::rename(old_path, new_path);
            folders_changed_.emit(parentNode()->label()->text().toUTF8() + "/" + new_file_name);
        }
        removeChild(dialog);
    });
    dialog->show(); 

}
void TreeNode::deleteFileMessageBox()
{
    auto message_box = addChild(std::make_unique<Wt::WMessageBox>(
        "Delete file: " + label()->text() + " ?", "",
        Wt::Icon::None, Wt::StandardButton::None));
    message_box->setOffsets(100, Wt::Side::Top);
    message_box->setModal(true);

    message_box->setStyleClass("stylus-background");
    message_box->titleBar()->setStyleClass("flex items-center justify-center p-[8px] cursor-pointer border-b border-solid text-xl font-bold");
    message_box->contents()->addStyleClass("flex items-center");
    message_box->footer()->setStyleClass("flex items-center justify-between p-[8px]");

    auto delete_btn = message_box->addButton("Delete", Wt::StandardButton::Yes);
    auto cancel_btn = message_box->addButton("Cancel", Wt::StandardButton::No);
    delete_btn->setStyleClass("btn-red");
    cancel_btn->setStyleClass("btn-default");
    
    message_box->buttonClicked().connect([=] {
        if (message_box->buttonResult() == Wt::StandardButton::Yes)
            {
            std::filesystem::path file_path = path_ + label()->text().toUTF8();
            // delete file
            if (std::filesystem::remove(file_path)) {
                folders_changed_.emit("");
            } else {
                Wt::WApplication::instance()->log("ERROR") << "\n\nError deleting file.\n\n";                    
            }
        }
        removeChild(message_box); 
    });
    
    message_box->show(); 
}

void TreeNode::copyFilePathToClipboard()
{
    Wt::WApplication::instance()->doJavaScript(
        "navigator.clipboard.writeText(" + getNodeImportString() + ");",
        "copyFilePathToClipboard"
    );
}
std::string TreeNode::getNodeImportString()
{
    std::string path = path_ + label()->text().toUTF8();
    std::string return_function = "";

    if(type_ == TreeNodeType::Folder) {
    }
    else if(type_ == TreeNodeType::File) {
        if(path.substr(path.find_last_of(".") + 1).compare("xml") == 0) {
            path = path.substr(0, path.find_last_of("."));
            return_function += "'messageResourceBundle().use(\"" + path + "\");'";
        }else if(path.substr(path.find_last_of(".") + 1).compare("css") == 0) {
            return_function += "'useStyleSheet(\"" + path + "\");'";
        }else if(path.substr(path.find_last_of(".") + 1).compare("js") == 0) {
            return_function += "'require(\"" + path + "\");'";
        }
    }
    return return_function;
}


FilesManager::FilesManager(std::shared_ptr<StylusState> state, StylusEditorManagementData data, int sidebar_width, std::string selected_file_path)
    : state_(state),
    data_(data),
    selected_file_path_(selected_file_path)
{
    auto grid_layout = std::make_unique<Wt::WGridLayout>();

    sidebar_ = grid_layout->addWidget(std::make_unique<FilesManagerSidebar>(), 0, 0);
    sidebar_->addStyleClass("z-[100]");

    if(data.extension_.compare("js") == 0) {
        editor_ = grid_layout->addWidget(std::make_unique<MonacoEditor>("javascript"), 0, 1, 0, 0);
    }else{
        editor_ = grid_layout->addWidget(std::make_unique<MonacoEditor>(data.extension_), 0, 1, 0, 0);
    }
    editor_->setStyleClass("z-[99] h-screen");

    grid_layout->setColumnResizable(0, true, Wt::WLength(sidebar_width, Wt::LengthUnit::Pixel));
    grid_layout->setContentsMargins(0, 0, 0, 0);
    
    grid_layout_ = grid_layout.get();
    setLayout(std::move(grid_layout));
 
    getFolders();
    tree_ = sidebar_->contents_->addWidget(std::make_unique<Wt::WTree>());

    editor_->avalable_save().connect(this, [=]()
                                        {
        TreeNode* selected_node;
        if(selected_file_path_.compare("") == 0) {
            return;
        }

        if(!std::fstream(data_.root_folder_path_ +  selected_file_path_).good()) {
            std::cout << "\n\n avalable save but file not found: " << data_.root_folder_path_ + selected_file_path_ << "\n\n";
            return;
        }

        auto selected_nodes = tree_->selectedNodes();

        for(auto node : selected_nodes) {
            if(node->label()->text().toUTF8().compare(selected_file_path_.substr(selected_file_path_.find_last_of("/")+1)) == 0) {
                selected_node = dynamic_cast<TreeNode*>(node);
                break;
            }
        }
        if(selected_node == nullptr) {
            std::cout << "\n\n No node found for the selected file.\n\n";
            return;
        }
        if(editor_->unsavedChanges()) {
            selected_node->toggleStyleClass("unsaved-changes", true, true);
        }
        else {
            selected_node->toggleStyleClass("unsaved-changes", false, true);
        }
    });

    editor_->save_file_signal().connect(this, [=](std::string text)
                                        {
        if(selected_file_path_.compare("") == 0) {
            std::cout << "\n\n No file selected to save.\n\n";
            return;
        }
        std::ofstream file(data_.root_folder_path_ + selected_file_path_);
        if (!file.is_open()) {
            std::cout << "\n\n Failed to open file: " << selected_file_path_ << "\n\n";
            return;
        }
        file << text;
        file.close();
        editor_->textSaved(); // this is fo monaco to set the current text and emit avalable_save_ signal to fase
        
        file_saved_.emit(selected_file_path_);
    });
  
    file_selected_.connect(this, [=]() {
        reuploadFile();
    });
    
    file_selected_.emit();
}

void FilesManager::reuploadFile() 
{
      std::string file_path = data_.root_folder_path_ + selected_file_path_;
        if(std::fstream(file_path).good() == false) {
            editor_->setEditorText("static/stylus-resources/empty-file", state_->getFileText("../static/stylus-resources/empty-file"));
            editor_->setEditorReadOnly(true);
        }else {
            editor_->setEditorReadOnly(false);
            editor_->setEditorText(data_.root_resource_url_ + selected_file_path_, state_->getFileText(file_path));
        }
        setTreeFolderWidgets();

}



void FilesManager::setTreeFolderWidgets()
{
    auto node = std::make_unique<TreeNode>(data_.root_folder_path_, TreeNodeType::Folder, data_.root_folder_path_);
    auto root_folder = node.get();
    tree_->setTreeRoot(std::move(node));
    tree_->setSelectionMode(Wt::SelectionMode::Single);
    tree_->treeRoot()->label()->setTextFormat(Wt::TextFormat::Plain);
    tree_->treeRoot()->expand();
    // tree_->treeRoot()->setLoadPolicy(Wt::ContentLoading::NextLevel);


    for(auto folder : folders_)
    {
        TreeNode *folder_tree_node = dynamic_cast<TreeNode*>(tree_->treeRoot()->addChildNode(std::make_unique<TreeNode>(folder.first, TreeNodeType::Folder, data_.root_folder_path_)));

        for(const auto &file : folder.second)
        {
            auto file_tree_node = dynamic_cast<TreeNode*>(folder_tree_node->addChildNode(std::make_unique<TreeNode>(file, TreeNodeType::File, data_.root_folder_path_ + folder.first + "/"))); 

            if(selected_file_path_.compare(folder.first + "/" + file) == 0)
            {
                if(editor_->unsavedChanges()) {
                    file_tree_node->addStyleClass("unsaved-changes");
                }
                tree_->select(file_tree_node);
            }
      
            file_tree_node->selected().connect(this, [=](bool selected)
            {
                if(selected){
                    selected_file_path_ = file_tree_node->parentNode()->label()->text().toUTF8() + "/" + file_tree_node->label()->text().toUTF8();
                    file_selected_.emit();
                }
            });

            file_tree_node->folders_changed_.connect(this, [=](std::string selected_file_path)
            {
                if(selected_file_path.compare("") != 0) {
                    selected_file_path_ = selected_file_path;
                }
                getFolders();
                file_selected_.emit();
            });

        }

        if(folder.second.size() > 0)
        {
            folder_tree_node->expand();
        }

        folder_tree_node->folders_changed_.connect(this, [=](std::string selected_file_path)
        {
            if(selected_file_path.compare("") != 0) {
                selected_file_path_ = selected_file_path;
            }
            getFolders();
            file_selected_.emit();
        });
    }

    root_folder->folders_changed_.connect(this, [=](std::string selected_file_path)
    {
        if(selected_file_path.compare("") != 0) {
            selected_file_path_ = selected_file_path;
        }
        getFolders();
        file_selected_.emit();
    });
}

std::vector<std::pair<std::string, std::vector<std::string>>> FilesManager::getFolders()
{
    std::vector<std::pair<std::string, std::vector<std::string>>> return_folders;
    std::vector<std::string> folders;

    for (const auto &entry : std::filesystem::directory_iterator(data_.root_folder_path_))
    {
        if (entry.is_directory())
        {
            folders.push_back(entry.path().filename().string());
        }
    }
    for (const auto &folder : folders)
    {
        std::vector<std::string> files;
        for (const auto &entry : std::filesystem::directory_iterator(data_.root_folder_path_ + folder))
        {
            if (entry.is_regular_file())
            {
                files.push_back(entry.path().filename().string());
            }
        }
        return_folders.push_back(std::make_pair(folder, files));
    }
    // sort
    std::sort(return_folders.begin(), return_folders.end(), [](const auto &a, const auto &b)
                { return a.first < b.first; });
    for (auto &folder : return_folders)
    {
        std::sort(folder.second.begin(), folder.second.end());
    }
    folders_ = return_folders;
    folders_changed_.emit();
    return return_folders;
}

}