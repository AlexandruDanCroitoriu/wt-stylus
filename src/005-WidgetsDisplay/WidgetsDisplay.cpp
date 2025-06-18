#include "005-WidgetsDisplay/WidgetsDisplay.h"
#include <Wt/WApplication.h>
#include <Wt/WPushButton.h>
#include "004-Theme/Theme.h"
#include "000-Components/Button.h"
#include <Wt/WTemplate.h>
#include <Wt/WTable.h>
#include <Wt/WTableCell.h>
#include <Wt/WContainerWidget.h>
#include <Wt/WComboBox.h>

WidgetsDisplay::WidgetsDisplay()
{

}


void WidgetsDisplay::createButtons()
{
    auto table_wrapper = addNew<Wt::WContainerWidget>();
    table_wrapper->setStyleClass("overflow-x-auto my-4 max-w-full w-fit border border-outline dark:border-outline-dark rounded-radius");


    auto table_header_wrapper = table_wrapper->addNew<Wt::WContainerWidget>();
    table_header_wrapper->addStyleClass("flex items-center p-2 space-x-2");
    
    auto size_combobox = table_header_wrapper->addNew<Wt::WComboBox>();
    size_combobox->addStyleClass("w-24");
    size_combobox->insertItem(ButtonSize::XS, "xs");
    size_combobox->insertItem(ButtonSize::SM, "sm");
    size_combobox->insertItem(ButtonSize::MD, "md");
    size_combobox->insertItem(ButtonSize::LG, "lg");
    size_combobox->insertItem(ButtonSize::XL, "xl");

    table_header_wrapper->addNew<Wt::WText>("Button examples")->setStyleClass("ml-2 text-lg font-semibold text-on-surface-strong dark:text-on-surface-dark-strong");

    auto table = table_wrapper->addNew<Wt::WTable>();
    
    table->setStyleClass("text-center text-on-surface dark:text-on-surface-dark ");
    table->rowAt(0)->setStyleClass("border-t border-outline dark:border-outline-dark bg-surface-alt text-on-surface-strong dark:bg-surface-dark-alt dark:text-on-surface-dark-strong");
    table->columnAt(0)->setStyleClass(" bg-surface-alt text-on-surface-strong dark:bg-surface-dark-alt dark:text-on-surface-dark-strong");

    table->elementAt(0, 0)->addNew<Wt::WText>("buttons")->setStyleClass("text-sm font-semibold p-1 capitalize");

    table->elementAt(0, 1)->addStyleClass("border-b border-outline dark:border-outline-dark text-sm font-semibold p-1 capitalize");
    table->elementAt(0, 2)->addStyleClass("border-b border-outline dark:border-outline-dark text-sm font-semibold p-1 capitalize");
    table->elementAt(0, 3)->addStyleClass("border-b border-outline dark:border-outline-dark text-sm font-semibold p-1 capitalize");
    table->elementAt(0, 4)->addStyleClass("border-b border-outline dark:border-outline-dark text-sm font-semibold p-1 capitalize");
    table->elementAt(0, 5)->addStyleClass("border-b border-outline dark:border-outline-dark text-sm font-semibold p-1 capitalize");
    table->elementAt(0, 6)->addStyleClass("border-b border-outline dark:border-outline-dark text-sm font-semibold p-1 capitalize");
    table->elementAt(0, 7)->addStyleClass("border-b border-outline dark:border-outline-dark text-sm font-semibold p-1 capitalize");
    table->elementAt(0, 8)->addStyleClass("border-b border-outline dark:border-outline-dark text-sm font-semibold p-1 capitalize");

    table->elementAt(1, 0)->addStyleClass("border-r border-outline dark:border-outline-dark text-sm font-semibold p-1 capitalize text-on-surface-strong dark:text-on-surface-dark");
    table->elementAt(2, 0)->addStyleClass("border-r border-outline dark:border-outline-dark text-sm font-semibold p-1 capitalize text-on-surface-strong dark:text-on-surface-dark");
    table->elementAt(3, 0)->addStyleClass("border-r border-outline dark:border-outline-dark text-sm font-semibold p-1 capitalize text-on-surface-strong dark:text-on-surface-dark");
    table->elementAt(4, 0)->addStyleClass("border-r border-outline dark:border-outline-dark text-sm font-semibold p-1 capitalize text-on-surface-strong dark:text-on-surface-dark");
    table->elementAt(5, 0)->addStyleClass("border-r border-outline dark:border-outline-dark text-sm font-semibold p-1 capitalize text-on-surface-strong dark:text-on-surface-dark");
    table->elementAt(6, 0)->addStyleClass("border-r border-outline dark:border-outline-dark text-sm font-semibold p-1 capitalize text-on-surface-strong dark:text-on-surface-dark");

    table->elementAt(0, 1)->addNew<Wt::WText>("primary");
    table->elementAt(0, 2)->addNew<Wt::WText>("seccondary");
    table->elementAt(0, 3)->addNew<Wt::WText>("success");
    table->elementAt(0, 4)->addNew<Wt::WText>("danger");
    table->elementAt(0, 5)->addNew<Wt::WText>("warning");
    table->elementAt(0, 6)->addNew<Wt::WText>("info");
    table->elementAt(0, 7)->addNew<Wt::WText>("alternate");
    table->elementAt(0, 8)->addNew<Wt::WText>("inverse");

    table->elementAt(1, 0)->addNew<Wt::WText>("default");
    table->elementAt(2, 0)->addNew<Wt::WText>("outline");
    table->elementAt(3, 0)->addNew<Wt::WText>("ghost");
    table->elementAt(4, 0)->addNew<Wt::WText>("with icon");
    table->elementAt(5, 0)->addNew<Wt::WText>("action");
    table->elementAt(6, 0)->addNew<Wt::WText>("loader");



    table->elementAt(1, 1)->addNew<Button>("primary", "mx-auto my-1", PenguinUiWidgetTheme::BtnPrimary);
    table->elementAt(1, 2)->addNew<Button>("seccondary", "mx-auto my-1", PenguinUiWidgetTheme::BtnSecondary);
    table->elementAt(1, 3)->addNew<Button>("success", "mx-auto my-1", PenguinUiWidgetTheme::BtnSuccess);
    table->elementAt(1, 4)->addNew<Button>("danger", "mx-auto my-1", PenguinUiWidgetTheme::BtnDanger);
    table->elementAt(1, 5)->addNew<Button>("warning", "mx-auto my-1", PenguinUiWidgetTheme::BtnWarning);
    table->elementAt(1, 6)->addNew<Button>("info", "mx-auto my-1", PenguinUiWidgetTheme::BtnInfo);
    table->elementAt(1, 7)->addNew<Button>("alternate", "mx-auto my-1", PenguinUiWidgetTheme::BtnAlternate);
    table->elementAt(1, 8)->addNew<Button>("inverse", "mx-auto my-1", PenguinUiWidgetTheme::BtnInverse);

    table->elementAt(2, 1)->addNew<Button>("primary", "mx-auto my-1", PenguinUiWidgetTheme::BtnPrimaryOutline);
    table->elementAt(2, 2)->addNew<Button>("seccondary", "mx-auto my-1", PenguinUiWidgetTheme::BtnSecondaryOutline);
    table->elementAt(2, 3)->addNew<Button>("success", "mx-auto my-1", PenguinUiWidgetTheme::BtnSuccessOutline);
    table->elementAt(2, 4)->addNew<Button>("danger", "mx-auto my-1", PenguinUiWidgetTheme::BtnDangerOutline);
    table->elementAt(2, 5)->addNew<Button>("warning", "mx-auto my-1", PenguinUiWidgetTheme::BtnWarningOutline);
    table->elementAt(2, 6)->addNew<Button>("info", "mx-auto my-1", PenguinUiWidgetTheme::BtnInfoOutline);
    table->elementAt(2, 7)->addNew<Button>("alternate", "mx-auto my-1", PenguinUiWidgetTheme::BtnAlternateOutline);
    table->elementAt(2, 8)->addNew<Button>("inverse", "mx-auto my-1", PenguinUiWidgetTheme::BtnInverseOutline);

    table->elementAt(3, 1)->addNew<Button>("primary", "mx-auto my-1", PenguinUiWidgetTheme::BtnPrimaryGhost);
    table->elementAt(3, 2)->addNew<Button>("seccondary", "mx-auto my-1", PenguinUiWidgetTheme::BtnSecondaryGhost);
    table->elementAt(3, 3)->addNew<Button>("success", "mx-auto my-1", PenguinUiWidgetTheme::BtnSuccessGhost);
    table->elementAt(3, 4)->addNew<Button>("danger", "mx-auto my-1", PenguinUiWidgetTheme::BtnDangerGhost);
    table->elementAt(3, 5)->addNew<Button>("warning", "mx-auto my-1", PenguinUiWidgetTheme::BtnWarningGhost);
    table->elementAt(3, 6)->addNew<Button>("info", "mx-auto my-1", PenguinUiWidgetTheme::BtnInfoGhost);
    table->elementAt(3, 7)->addNew<Button>("alternate", "mx-auto my-1", PenguinUiWidgetTheme::BtnAlternateGhost);
    table->elementAt(3, 8)->addNew<Button>("inverse", "mx-auto my-1", PenguinUiWidgetTheme::BtnInverseGhost);

    table->elementAt(4, 1)->addNew<Button>(std::string(Wt::WString::tr("penguin-ui-svg:plus-primary").toUTF8() + "with icon"), "mx-auto my-1", PenguinUiWidgetTheme::BtnPrimaryWithIcon);
    table->elementAt(4, 2)->addNew<Button>(std::string(Wt::WString::tr("penguin-ui-svg:plus-seccondary").toUTF8() + "with icon"), "mx-auto my-1", PenguinUiWidgetTheme::BtnSecondaryWithIcon);
    table->elementAt(4, 3)->addNew<Button>(std::string(Wt::WString::tr("penguin-ui-svg:plus-success").toUTF8() + "with icon"), "mx-auto my-1", PenguinUiWidgetTheme::BtnSuccessWithIcon);
    table->elementAt(4, 4)->addNew<Button>(std::string(Wt::WString::tr("penguin-ui-svg:plus-danger").toUTF8() + "with icon"), "mx-auto my-1", PenguinUiWidgetTheme::BtnDangerWithIcon);
    table->elementAt(4, 5)->addNew<Button>(std::string(Wt::WString::tr("penguin-ui-svg:plus-warning").toUTF8() + "with icon"), "mx-auto my-1", PenguinUiWidgetTheme::BtnWarningWithIcon);
    table->elementAt(4, 6)->addNew<Button>(std::string(Wt::WString::tr("penguin-ui-svg:plus-info").toUTF8() + "with icon"), "mx-auto my-1", PenguinUiWidgetTheme::BtnInfoWithIcon);
    table->elementAt(4, 7)->addNew<Button>(std::string(Wt::WString::tr("penguin-ui-svg:plus-alternate").toUTF8() + "with icon"), "mx-auto my-1", PenguinUiWidgetTheme::BtnAlternateWithIcon);
    table->elementAt(4, 8)->addNew<Button>(std::string(Wt::WString::tr("penguin-ui-svg:plus-inverse").toUTF8() + "with icon"), "mx-auto my-1", PenguinUiWidgetTheme::BtnInverseWithIcon);

    table->elementAt(5, 1)->addNew<Button>(std::string(Wt::WString::tr("penguin-ui-svg:plus-primary").toUTF8()), "mx-auto my-1", PenguinUiWidgetTheme::BtnPrimaryAction);
    table->elementAt(5, 2)->addNew<Button>(std::string(Wt::WString::tr("penguin-ui-svg:plus-seccondary").toUTF8()), "mx-auto my-1", PenguinUiWidgetTheme::BtnSecondaryAction);
    table->elementAt(5, 3)->addNew<Button>(std::string(Wt::WString::tr("penguin-ui-svg:plus-success").toUTF8()), "mx-auto my-1", PenguinUiWidgetTheme::BtnSuccessAction);
    table->elementAt(5, 4)->addNew<Button>(std::string(Wt::WString::tr("penguin-ui-svg:plus-danger").toUTF8()), "mx-auto my-1", PenguinUiWidgetTheme::BtnDangerAction);
    table->elementAt(5, 5)->addNew<Button>(std::string(Wt::WString::tr("penguin-ui-svg:plus-warning").toUTF8()), "mx-auto my-1", PenguinUiWidgetTheme::BtnWarningAction);
    table->elementAt(5, 6)->addNew<Button>(std::string(Wt::WString::tr("penguin-ui-svg:plus-info").toUTF8()), "mx-auto my-1", PenguinUiWidgetTheme::BtnInfoAction);
    table->elementAt(5, 7)->addNew<Button>(std::string(Wt::WString::tr("penguin-ui-svg:plus-alternate").toUTF8()), "mx-auto my-1", PenguinUiWidgetTheme::BtnAlternateAction);
    table->elementAt(5, 8)->addNew<Button>(std::string(Wt::WString::tr("penguin-ui-svg:plus-inverse").toUTF8()), "mx-auto my-1", PenguinUiWidgetTheme::BtnInverseAction);

    table->elementAt(6, 1)->addNew<Button>(std::string(Wt::WString::tr("penguin-ui-svg:loader-primary").toUTF8() + "loader"), "mx-auto my-1", PenguinUiWidgetTheme::BtnPrimaryLoader);
    table->elementAt(6, 2)->addNew<Button>(std::string(Wt::WString::tr("penguin-ui-svg:loader-seccondary").toUTF8() + "loader"), "mx-auto my-1", PenguinUiWidgetTheme::BtnSecondaryLoader);
    table->elementAt(6, 3)->addNew<Button>(std::string(Wt::WString::tr("penguin-ui-svg:loader-success").toUTF8() + "loader"), "mx-auto my-1", PenguinUiWidgetTheme::BtnSuccessLoader);
    table->elementAt(6, 4)->addNew<Button>(std::string(Wt::WString::tr("penguin-ui-svg:loader-danger").toUTF8() + "loader"), "mx-auto my-1", PenguinUiWidgetTheme::BtnDangerLoader);
    table->elementAt(6, 5)->addNew<Button>(std::string(Wt::WString::tr("penguin-ui-svg:loader-warning").toUTF8() + "loader"), "mx-auto my-1", PenguinUiWidgetTheme::BtnWarningLoader);
    table->elementAt(6, 6)->addNew<Button>(std::string(Wt::WString::tr("penguin-ui-svg:loader-info").toUTF8() + "loader"), "mx-auto my-1", PenguinUiWidgetTheme::BtnInfoLoader);
    table->elementAt(6, 7)->addNew<Button>(std::string(Wt::WString::tr("penguin-ui-svg:loader-alternate").toUTF8() + "loader"), "mx-auto my-1", PenguinUiWidgetTheme::BtnAlternateLoader);
    table->elementAt(6, 8)->addNew<Button>(std::string(Wt::WString::tr("penguin-ui-svg:loader-inverse").toUTF8() + "loader"), "mx-auto my-1", PenguinUiWidgetTheme::BtnInverseLoader);

    size_combobox->activated().connect([=](int index) {
        if(index == selected_size_ && table->hasStyleClass("text-" + size_combobox->itemText(index).toUTF8())) {
            return;
        }
        table->removeStyleClass("text-" + size_combobox->itemText(selected_size_).toUTF8(), true);
        table->addStyleClass("text-" + size_combobox->itemText(index).toUTF8());
        selected_size_ = ButtonSize(index);
    });

    // Set default size
    size_combobox->activated().emit(selected_size_);

}
