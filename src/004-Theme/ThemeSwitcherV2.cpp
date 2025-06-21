#include "004-Theme/ThemeSwitcherV2.h"
#include "004-Theme/Theme.h"
#include "001-App/App.h"

ThemeSwitcherV2::ThemeSwitcherV2(Session& session)
    : Button("UI", "text-md font-bold z-20 !rounded-full w-10", PenguinUiWidgetTheme::BtnPrimary),
    session_(session)
{
    clicked().connect(this, &ThemeSwitcherV2::showPopup);
}


void ThemeSwitcherV2::showPopup(const Wt::WMouseEvent& event)
{
    std::cout << "\n\n\n\n ------------------ThemeSwitcherV2::showPopup called\n\n";
    if(!popup_menu_) {
        popup_menu_ = std::make_unique<Wt::WPopupMenu>();
        popup_menu_->setHideOnSelect(false);
        popup_menu_->setStyleClass("flex flex-col bg-transparent");

        popup_menu_->addItem(std::move(getMenuItem("arctic")));
        popup_menu_->addItem(std::move(getMenuItem("minimal")));
        popup_menu_->addItem(std::move(getMenuItem("modern")));
        popup_menu_->addItem(std::move(getMenuItem("high-contrast")));
        popup_menu_->addItem(std::move(getMenuItem("neo-brutalism")));
        popup_menu_->addItem(std::move(getMenuItem("halloween")));
        popup_menu_->addItem(std::move(getMenuItem("zombie")));
        popup_menu_->addItem(std::move(getMenuItem("pastel")));
        popup_menu_->addItem(std::move(getMenuItem("90s")));
        popup_menu_->addItem(std::move(getMenuItem("christmas")));
        popup_menu_->addItem(std::move(getMenuItem("prototype")));
        popup_menu_->addItem(std::move(getMenuItem("news")));
        popup_menu_->addItem(std::move(getMenuItem("industrial")));
        
        // insertItem(ThemeConfig::Arctic, "arctic");
        // insertItem(ThemeConfig::Minimal, "minimal");
        // insertItem(ThemeConfig::Modern, "modern");
        // insertItem(ThemeConfig::HighContrast, "high-contrast");
        // insertItem(ThemeConfig::NeoBrutalism, "neo-brutalism");
        // insertItem(ThemeConfig::Halloween, "halloween");
        // insertItem(ThemeConfig::Zombie, "zombie");
        // insertItem(ThemeConfig::Pastel, "pastel");
        // insertItem(ThemeConfig::Nineties, "90s");
        // insertItem(ThemeConfig::Christmas, "christmas");
        // insertItem(ThemeConfig::Prototype, "prototype");
        // insertItem(ThemeConfig::News, "news");
        // insertItem(ThemeConfig::Industrial, "industrial");

    }

 
    if (popup_menu_->isHidden())
        // popup_menu_->popup(event);
        popup_menu_->popup(this, Wt::Orientation::Vertical);
    else
        popup_menu_->hide();
}

void ThemeSwitcherV2::hidePopup()
{
    if(popup_menu_ && popup_menu_->isVisible()) 
        popup_menu_->hide();
}

std::unique_ptr<Wt::WMenuItem> ThemeSwitcherV2::getMenuItem(const std::string& theme_name)
{
    auto menu_item = std::make_unique<Wt::WMenuItem>(theme_name);
    menu_item->setAttributeValue("data-theme", theme_name);
    menu_item->addStyleClass("capitalize px-3 py-1.5 text-base font-bold !rounded-none ");
    // dynamic_cast<Theme*>(wApp->theme().get())->applyTheme(menu_item.get(), PenguinUiWidgetTheme::BtnDefault);
    dynamic_cast<Theme*>(wApp->theme().get())->applyTheme(menu_item.get(), PenguinUiWidgetTheme::BtnPrimary);

    menu_item->clicked().connect([=]() {
        wApp->setHtmlAttribute("data-theme", theme_name);
    });
    return menu_item;
}
