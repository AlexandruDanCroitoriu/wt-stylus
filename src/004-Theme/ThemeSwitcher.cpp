#include "004-Theme/ThemeSwitcher.h"
#include "004-Theme/Theme.h"
#include "001-App/App.h"

ThemeSwitcher::ThemeSwitcher(Session& session)
    : Button("UI", "text-md font-bold z-20 !rounded-full w-10", PenguinUiWidgetTheme::BtnPrimary),
    session_(session)
{
    clicked().connect(this, &ThemeSwitcher::showPopup);
}


void ThemeSwitcher::showPopup(const Wt::WMouseEvent& event)
{
    std::cout << "\n\n\n\n ------------------ThemeSwitcher::showPopup called\n\n";
    if(!popup_menu_) {
        popup_menu_ = std::make_unique<Wt::WPopupMenu>();
        popup_menu_->setHideOnSelect(false);
        popup_menu_->setStyleClass("border divide-y divide-outline border-outline bg-surface dark:divide-outline-dark dark:border-outline-dark dark:bg-surface-dark rounded-radius shadow-2xl");

        popup_menu_->addItem(std::move(getMenuItem("arctic")))->addStyleClass("!rounded-t-radius");
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
        popup_menu_->addItem(std::move(getMenuItem("industrial")))->addStyleClass("!rounded-b-radius");
        
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
        popup_menu_->popup(this, Wt::Orientation::Horizontal);
    else
        popup_menu_->hide();
}

void ThemeSwitcher::hidePopup()
{
    if(popup_menu_ && popup_menu_->isVisible()) 
        popup_menu_->hide();
}

std::unique_ptr<Wt::WMenuItem> ThemeSwitcher::getMenuItem(const std::string& theme_name)
{
    auto menu_item = std::make_unique<Wt::WMenuItem>(theme_name);
    // menu_item->setAttributeValue("data-theme", theme_name);
    menu_item->addStyleClass("flex items-center gap-2 px-2 py-1.5 text-sm font-medium text-on-surface underline-offset-2 hover:bg-primary/5 hover:text-on-surface-strong focus-visible:underline focus:outline-hidden dark:text-on-surface-dark dark:hover:bg-primary-dark/5 dark:hover:text-on-surface-dark-strong");
    // dynamic_cast<Theme*>(wApp->theme().get())->applyTheme(menu_item.get(), PenguinUiWidgetTheme::BtnDefault);
    // dynamic_cast<Theme*>(wApp->theme().get())->applyTheme(menu_item.get(), PenguinUiWidgetTheme::BtnPrimary);
    menu_item->clicked().connect([=]() {
        wApp->setHtmlAttribute("data-theme", theme_name);
    });
    return menu_item;
}
