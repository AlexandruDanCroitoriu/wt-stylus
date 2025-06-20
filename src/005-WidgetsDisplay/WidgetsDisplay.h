#pragma once
#include <Wt/WContainerWidget.h>


enum ButtonSize
{
    XS = 0,
    SM = 1,
    MD = 2,
    LG = 3,
    XL = 4,
};
class WidgetsDisplay : public Wt::WContainerWidget
{
    public:
    WidgetsDisplay();
    
    void createButtons();
    
    private:
    void setCopyToClipboardAction(Wt::WInteractWidget *widget, const std::string &text);
    ButtonSize selected_size_ = ButtonSize::XS;

};