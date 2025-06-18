#include "004-Theme/ThemeSwitcher.h"
#include <Wt/WApplication.h>
#include "004-Theme/Theme.h"


ThemeSwitcher::ThemeSwitcher()
    : Wt::WComboBox()
{ 
    // setThemeStyleEnabled(true);
    wApp->theme()->apply(this, this, PenguinUiWidgetTheme::WComboBox);
    insertItem(ThemeConfig::Arctic, "arctic");
    insertItem(ThemeConfig::Minimal, "minimal");
    insertItem(ThemeConfig::Modern, "modern");
    insertItem(ThemeConfig::HighContrast, "high-contrast");
    insertItem(ThemeConfig::NeoBrutalism, "neo-brutalism");
    insertItem(ThemeConfig::Halloween, "halloween");
    insertItem(ThemeConfig::Zombie, "zombie");
    insertItem(ThemeConfig::Pastel, "pastel");
    insertItem(ThemeConfig::Nineties, "90s");
    insertItem(ThemeConfig::Christmas, "christmas");
    insertItem(ThemeConfig::Prototype, "prototype");
    insertItem(ThemeConfig::News, "news");
    insertItem(ThemeConfig::Industrial, "industrial");

    setCurrentIndex(static_cast<int>(dynamic_cast<Theme*>(wApp->theme().get())->current_theme_));

    activated().connect(this, [=](int index) {
        auto theme = itemText(index).toUTF8();
        wApp->setHtmlAttribute("data-theme", theme);
    });        

    setStyleClass("");

    keyWentDown().connect(this, [=](const Wt::WKeyEvent& event) {
        wApp->globalKeyWentDown().emit(event);
    });

    activated().emit(currentIndex()); // Emit the current index to apply the theme immediately

}


// flex items-center rounded-radius gap-2 bg-primary/10 px-2 py-1.5 text-sm font-medium text-on-surface-strong underline-offset-2 focus-visible:underline focus:outline-hidden dark:bg-primary-dark/10 dark:text-on-surface-dark-strong

// fixed left-0 z-20 flex h-svh w-60 shrink-0 flex-col border-r border-outline bg-surface-alt p-4 transition-transform duration-300 md:w-64 md:translate-x-0 md:relative dark:border-outline-dark dark:bg-surface-dark-alt -translate-x-60

// border-outline bg-surface-alt dark:border-outline-dark dark:bg-surface-dark-alt