#include "004-Theme/DarkModeToggleV2.h"
#include "001-App/App.h"
#include <Wt/WLabel.h>


DarkModeToggleV2::DarkModeToggleV2()
    : Wt::WCheckBox("")
{
    setStyleClass("dark-mode-checkbox-toggle");
    setChecked(wApp->htmlClass().find("dark") == std::string::npos ? false : true);
    
    // label()->setStyleClass("font-bold");
    
    changed().connect(this, [=](){
        dynamic_cast<App*>(wApp)->dark_mode_changed_.emit(isChecked());
        if (isChecked()) {
            wApp->setHtmlClass("dark");
        } else {
            wApp->setHtmlClass("");
        }
    });

    keyWentDown().connect([=](Wt::WKeyEvent e)
    { 
        wApp->globalKeyWentDown().emit(e); // Emit the global key event
    });
    changed().connect(this, [=](){
        dynamic_cast<App*>(wApp)->dark_mode_changed_.emit(isChecked());
    });
    
    dynamic_cast<App*>(wApp)->dark_mode_changed_.connect(this, [=](bool dark){
        setChecked(dark);
    });
}