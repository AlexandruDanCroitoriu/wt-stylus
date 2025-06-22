#include "004-Theme/DarkModeToggle.h"
#include "001-App/App.h"
#include <Wt/WLabel.h>


DarkModeToggle::DarkModeToggle(Session& session)
    : Wt::WCheckBox(""),
    session_(session)
{
    setStyleClass("dark-mode-checkbox-toggle");
    setChecked(wApp->htmlClass().find("dark") == std::string::npos ? false : true);
    
    // label()->setStyleClass("font-bold");
    
    changed().connect(this, [=](){
        dynamic_cast<App*>(wApp)->dark_mode_changed_.emit(isChecked());
        if(session_.login().loggedIn()){
            Wt::Dbo::Transaction transaction(session_);
            auto user = session_.user(session_.login().user());
            if (user) {
                user.modify()->ui_dark_mode_ = isChecked();
            }
            transaction.commit();
            std::cout << "Transaction committed for dark mode change." << std::endl;
        }
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
        auto app = dynamic_cast<App*>(wApp);    
        dynamic_cast<App*>(wApp)->dark_mode_changed_.emit(isChecked());
    });
    
    dynamic_cast<App*>(wApp)->dark_mode_changed_.connect(this, [=](bool dark){
        setChecked(dark);
    });
}