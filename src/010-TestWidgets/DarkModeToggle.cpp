#include "010-TestWidgets/DarkModeToggle.h"
#include <Wt/WApplication.h>
#include <Wt/WLabel.h>
#include "001-App/App.h"

DarkModeToggle::DarkModeToggle(bool dark)
{
    setHtmlTagName("label");
    setStyleClass("inline-flex cursor-pointer items-center m-1");
    checkBox_ = addWidget(std::make_unique<Wt::WCheckBox>());
    setAttributeValue("for", checkBox_->id());
    
    auto display = addWidget(std::make_unique<Wt::WContainerWidget>());

    checkBox_->setStyleClass("peer sr-only");
    display->setStyleClass("peer text-md relative h-6 w-11 rounded-full bg-gray-200 peer-checked:bg-blue-600 peer-focus:ring-4 peer-focus:ring-blue-300 peer-focus:outline-none after:absolute after:start-[2px] after:top-[2px] after:h-5 after:w-5 after:rounded-full after:border after:border-gray-300 after:bg-white after:leading-[16px] after:transition-all after:content-['☀'] peer-checked:after:translate-x-full peer-checked:after:border-white peer-checked:after:content-['🌙'] rtl:peer-checked:after:-translate-x-full dark:border-gray-600 dark:bg-gray-700 dark:peer-checked:bg-blue-600 dark:peer-focus:ring-blue-800");
    
    if(Wt::WApplication::instance()->htmlClass().find("dark") != std::string::npos){
        checkBox_->setChecked(true);
    }

    checkBox_->changed().connect(this, [=](){
        dynamic_cast<App*>(Wt::WApplication::instance())->dark_mode_changed_.emit(checkBox_->isChecked());
    });
    keyWentDown().connect([=](Wt::WKeyEvent e)
    { 
        Wt::WApplication::instance()->globalKeyWentDown().emit(e); // Emit the global key event
    });

    dynamic_cast<App*>(Wt::WApplication::instance())->dark_mode_changed_.connect(this, [=](bool dark){
        setDarkMode(dark);
        dark_mode_changed_.emit(dark);
    });



}

void DarkModeToggle::setDarkMode(bool dark)
{
    checkBox_->setChecked(dark);
    if(dark){
        Wt::WApplication::instance()->setHtmlClass("dark");
    }
    else{
        Wt::WApplication::instance()->setHtmlClass("");
    } 
}