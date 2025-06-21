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
    table_wrapper->setStyleClass("relative overflow-x-auto my-4 max-w-full w-fit border border-outline dark:border-outline-dark rounded-radius");

    auto table_header_wrapper = table_wrapper->addNew<Wt::WContainerWidget>();
    table_header_wrapper->addStyleClass("flex items-center p-2 space-x-2 sticky left-0");
    
    auto size_combobox = table_header_wrapper->addNew<Wt::WComboBox>();
    size_combobox->addStyleClass("w-24");
    size_combobox->insertItem(ButtonSize::XS, "xs");
    size_combobox->insertItem(ButtonSize::SM, "sm");
    size_combobox->insertItem(ButtonSize::MD, "md");
    size_combobox->insertItem(ButtonSize::LG, "lg");
    size_combobox->insertItem(ButtonSize::XL, "xl");

    table_header_wrapper->addNew<Wt::WText>("Button examples")->setStyleClass("ml-2 text-lg font-semibold text-on-surface-strong dark:text-on-surface-dark-strong");

    auto table = table_wrapper->addNew<Wt::WTable>();
    
    table->setStyleClass("text-center text-on-surface dark:text-on-surface-dark border-outline dark:border-outline-dark ");
    table->rowAt(0)->setStyleClass("border-t border-outline dark:border-outline-dark bg-surface-alt text-on-surface-strong dark:bg-surface-dark-alt dark:text-on-surface-dark-strong font-semibold");
    table->columnAt(0)->setStyleClass(" bg-surface-alt dark:bg-surface-dark-alt");

    
    // set the header border and text styles
    table->elementAt(0, 0)->addNew<Wt::WText>("buttons")->setStyleClass("text-sm p-1 capitalize");

    table->elementAt(1, 0)->addStyleClass("border-r border-outline dark:border-outline-dark text-sm p-1 capitalize text-on-surface-strong dark:text-on-surface-dark-strong font-semibold");
    table->elementAt(2, 0)->addStyleClass("border-r border-outline dark:border-outline-dark text-sm p-1 capitalize text-on-surface-strong dark:text-on-surface-dark-strong font-semibold");
    table->elementAt(3, 0)->addStyleClass("border-r border-outline dark:border-outline-dark text-sm p-1 capitalize text-on-surface-strong dark:text-on-surface-dark-strong font-semibold");
    table->elementAt(4, 0)->addStyleClass("border-r border-outline dark:border-outline-dark text-sm p-1 capitalize text-on-surface-strong dark:text-on-surface-dark-strong font-semibold");
    table->elementAt(5, 0)->addStyleClass("border-r border-outline dark:border-outline-dark text-sm p-1 capitalize text-on-surface-strong dark:text-on-surface-dark-strong font-semibold");
    table->elementAt(6, 0)->addStyleClass("border-r border-outline dark:border-outline-dark text-sm p-1 capitalize text-on-surface-strong dark:text-on-surface-dark-strong font-semibold");
    table->elementAt(7, 0)->addStyleClass("border-r border-outline dark:border-outline-dark text-sm p-1 capitalize text-on-surface-strong dark:text-on-surface-dark-strong font-semibold");
    table->elementAt(8, 0)->addStyleClass("border-r border-outline dark:border-outline-dark text-sm p-1 capitalize text-on-surface-strong dark:text-on-surface-dark-strong font-semibold");

    table->elementAt(0, 1)->addStyleClass("border-b border-outline dark:border-outline-dark text-sm p-1 capitalize");
    table->elementAt(0, 2)->addStyleClass("border-b border-outline dark:border-outline-dark text-sm p-1 capitalize");
    table->elementAt(0, 3)->addStyleClass("border-b border-outline dark:border-outline-dark text-sm p-1 capitalize");
    table->elementAt(0, 4)->addStyleClass("border-b border-outline dark:border-outline-dark text-sm p-1 capitalize");
    table->elementAt(0, 5)->addStyleClass("border-b border-outline dark:border-outline-dark text-sm p-1 capitalize");
    table->elementAt(0, 6)->addStyleClass("border-b border-outline dark:border-outline-dark text-sm p-1 capitalize");

    table->elementAt(1, 0)->addNew<Wt::WText>("primary");
    table->elementAt(2, 0)->addNew<Wt::WText>("seccondary");
    table->elementAt(3, 0)->addNew<Wt::WText>("success");
    table->elementAt(4, 0)->addNew<Wt::WText>("danger");
    table->elementAt(5, 0)->addNew<Wt::WText>("warning");
    table->elementAt(6, 0)->addNew<Wt::WText>("info");
    table->elementAt(7, 0)->addNew<Wt::WText>("alternate");
    table->elementAt(8, 0)->addNew<Wt::WText>("inverse");

    table->elementAt(0, 1)->addNew<Wt::WText>("default");
    table->elementAt(0, 2)->addNew<Wt::WText>("outline");
    table->elementAt(0, 3)->addNew<Wt::WText>("ghost");
    table->elementAt(0, 4)->addNew<Wt::WText>("with icon");
    table->elementAt(0, 5)->addNew<Wt::WText>("action");
    table->elementAt(0, 6)->addNew<Wt::WText>("loader");



    // auto btn = 
    setCopyToClipboardAction(
        table->elementAt(1, 1)->addNew<Button>("default", "m-1.5", PenguinUiWidgetTheme::BtnPrimary),
        "->addNew<Button>(\"default\", \"m-1.5 text-"+size_combobox->itemText(selected_size_).toUTF8()+" \", PenguinUiWidgetTheme::BtnPrimary);"
    );
    setCopyToClipboardAction(
        table->elementAt(2, 1)->addNew<Button>("default", "m-1.5", PenguinUiWidgetTheme::BtnSecondary),
        "->addNew<Button>(\"default\", \"m-1.5 text-"+size_combobox->itemText(selected_size_).toUTF8()+" \", PenguinUiWidgetTheme::BtnSecondary);"
    );
    setCopyToClipboardAction(
        table->elementAt(3, 1)->addNew<Button>("default", "m-1.5", PenguinUiWidgetTheme::BtnSuccess),
        "->addNew<Button>(\"default\", \"m-1.5 text-"+size_combobox->itemText(selected_size_).toUTF8()+" \", PenguinUiWidgetTheme::BtnSuccess);"
    );
    setCopyToClipboardAction(
        table->elementAt(4, 1)->addNew<Button>("default", "m-1.5", PenguinUiWidgetTheme::BtnDanger),
        "->addNew<Button>(\"default\", \"m-1.5 text-"+size_combobox->itemText(selected_size_).toUTF8()+" \", PenguinUiWidgetTheme::BtnDanger);"
    );
    setCopyToClipboardAction(
        table->elementAt(5, 1)->addNew<Button>("default", "m-1.5", PenguinUiWidgetTheme::BtnWarning),
        "->addNew<Button>(\"default\", \"m-1.5 text-"+size_combobox->itemText(selected_size_).toUTF8()+" \", PenguinUiWidgetTheme::BtnWarning);"
    );
    setCopyToClipboardAction(
        table->elementAt(6, 1)->addNew<Button>("default", "m-1.5", PenguinUiWidgetTheme::BtnInfo),
        "->addNew<Button>(\"default\", \"m-1.5 text-"+size_combobox->itemText(selected_size_).toUTF8()+" \", PenguinUiWidgetTheme::BtnInfo);"
    );
    setCopyToClipboardAction(
        table->elementAt(7, 1)->addNew<Button>("default", "m-1.5", PenguinUiWidgetTheme::BtnAlternate),
        "->addNew<Button>(\"default\", \"m-1.5 text-"+size_combobox->itemText(selected_size_).toUTF8()+" \", PenguinUiWidgetTheme::BtnAlternate);"
    );
    setCopyToClipboardAction(
        table->elementAt(8, 1)->addNew<Button>("default", "m-1.5", PenguinUiWidgetTheme::BtnInverse),
        "->addNew<Button>(\"default\", \"m-1.5 text-"+size_combobox->itemText(selected_size_).toUTF8()+" \", PenguinUiWidgetTheme::BtnInverse);"
    );

    setCopyToClipboardAction(
        table->elementAt(1, 2)->addNew<Button>("outline", "m-1.5", PenguinUiWidgetTheme::BtnPrimaryOutline),
        "->addNew<Button>(\"outline\", \"m-1.5 text-"+size_combobox->itemText(selected_size_).toUTF8()+" \", PenguinUiWidgetTheme::BtnPrimaryOutline);"
    );
    setCopyToClipboardAction(
        table->elementAt(2, 2)->addNew<Button>("outline", "m-1.5", PenguinUiWidgetTheme::BtnSecondaryOutline),
        "->addNew<Button>(\"outline\", \"m-1.5 text-"+size_combobox->itemText(selected_size_).toUTF8()+" \", PenguinUiWidgetTheme::BtnSecondaryOutline);"
    );
    setCopyToClipboardAction(
        table->elementAt(3, 2)->addNew<Button>("outline", "m-1.5", PenguinUiWidgetTheme::BtnSuccessOutline),
        "->addNew<Button>(\"outline\", \"m-1.5 text-"+size_combobox->itemText(selected_size_).toUTF8()+" \", PenguinUiWidgetTheme::BtnSuccessOutline);"
    );
    setCopyToClipboardAction(
        table->elementAt(4, 2)->addNew<Button>("outline", "m-1.5", PenguinUiWidgetTheme::BtnDangerOutline),
        "->addNew<Button>(\"outline\", \"m-1.5 text-"+size_combobox->itemText(selected_size_).toUTF8()+" \", PenguinUiWidgetTheme::BtnDangerOutline);"
    );
    setCopyToClipboardAction(
        table->elementAt(5, 2)->addNew<Button>("outline", "m-1.5", PenguinUiWidgetTheme::BtnWarningOutline),
        "->addNew<Button>(\"outline\", \"m-1.5 text-"+size_combobox->itemText(selected_size_).toUTF8()+" \", PenguinUiWidgetTheme::BtnWarningOutline);"
    );
    setCopyToClipboardAction(
        table->elementAt(6, 2)->addNew<Button>("outline", "m-1.5", PenguinUiWidgetTheme::BtnInfoOutline),
        "->addNew<Button>(\"outline\", \"m-1.5 text-"+size_combobox->itemText(selected_size_).toUTF8()+" \", PenguinUiWidgetTheme::BtnInfoOutline);"
    );
    setCopyToClipboardAction(
        table->elementAt(7, 2)->addNew<Button>("outline", "m-1.5", PenguinUiWidgetTheme::BtnAlternateOutline),
        "->addNew<Button>(\"outline\", \"m-1.5 text-"+size_combobox->itemText(selected_size_).toUTF8()+" \", PenguinUiWidgetTheme::BtnAlternateOutline);"
    );
    setCopyToClipboardAction(
        table->elementAt(8, 2)->addNew<Button>("outline", "m-1.5", PenguinUiWidgetTheme::BtnInverseOutline),
        "->addNew<Button>(\"outline\", \"m-1.5 text-"+size_combobox->itemText(selected_size_).toUTF8()+" \", PenguinUiWidgetTheme::BtnInverseOutline);"
    );

    setCopyToClipboardAction(
        table->elementAt(1, 3)->addNew<Button>("ghost", "m-1.5", PenguinUiWidgetTheme::BtnPrimaryGhost),
        "->addNew<Button>(\"ghost\", \"m-1.5 text-"+size_combobox->itemText(selected_size_).toUTF8()+" \", PenguinUiWidgetTheme::BtnPrimaryGhost);"
    );
    setCopyToClipboardAction(
        table->elementAt(2, 3)->addNew<Button>("ghost", "m-1.5", PenguinUiWidgetTheme::BtnSecondaryGhost),
        "->addNew<Button>(\"ghost\", \"m-1.5 text-"+size_combobox->itemText(selected_size_).toUTF8()+" \", PenguinUiWidgetTheme::BtnSecondaryGhost);"
    );
    setCopyToClipboardAction(
        table->elementAt(3, 3)->addNew<Button>("ghost", "m-1.5", PenguinUiWidgetTheme::BtnSuccessGhost),
        "->addNew<Button>(\"ghost\", \"m-1.5 text-"+size_combobox->itemText(selected_size_).toUTF8()+" \", PenguinUiWidgetTheme::BtnSuccessGhost);"
    );
    setCopyToClipboardAction(
        table->elementAt(4, 3)->addNew<Button>("ghost", "m-1.5", PenguinUiWidgetTheme::BtnDangerGhost),
        "->addNew<Button>(\"ghost\", \"m-1.5 text-"+size_combobox->itemText(selected_size_).toUTF8()+" \", PenguinUiWidgetTheme::BtnDangerGhost);"
    );
    setCopyToClipboardAction(
        table->elementAt(5, 3)->addNew<Button>("ghost", "m-1.5", PenguinUiWidgetTheme::BtnWarningGhost),
        "->addNew<Button>(\"ghost\", \"m-1.5 text-"+size_combobox->itemText(selected_size_).toUTF8()+" \", PenguinUiWidgetTheme::BtnWarningGhost);"
    );
    setCopyToClipboardAction(
        table->elementAt(6, 3)->addNew<Button>("ghost", "m-1.5", PenguinUiWidgetTheme::BtnInfoGhost),
        "->addNew<Button>(\"ghost\", \"m-1.5 text-"+size_combobox->itemText(selected_size_).toUTF8()+" \", PenguinUiWidgetTheme::BtnInfoGhost);"
    );
    setCopyToClipboardAction(
        table->elementAt(7, 3)->addNew<Button>("ghost", "m-1.5", PenguinUiWidgetTheme::BtnAlternateGhost),
        "->addNew<Button>(\"ghost\", \"m-1.5 text-"+size_combobox->itemText(selected_size_).toUTF8()+" \", PenguinUiWidgetTheme::BtnAlternateGhost);"
    );
    setCopyToClipboardAction(
        table->elementAt(8, 3)->addNew<Button>("ghost", "m-1.5", PenguinUiWidgetTheme::BtnInverseGhost),
        "->addNew<Button>(\"ghost\", \"m-1.5 text-"+size_combobox->itemText(selected_size_).toUTF8()+" \", PenguinUiWidgetTheme::BtnInverseGhost);"
    );

    setCopyToClipboardAction(
        table->elementAt(1, 4)->addNew<Button>(std::string(Wt::WString::tr("penguin-ui-svg:plus-primary").toUTF8() + "icon"), "m-1.5", PenguinUiWidgetTheme::BtnPrimaryWithIcon),
        "->addNew<Button>(std::string(Wt::WString::tr(\"penguin-ui-svg:plus-primary\").toUTF8() + \"icon\"), \"m-1.5 text-"+size_combobox->itemText(selected_size_).toUTF8()+" \", PenguinUiWidgetTheme::BtnPrimaryWithIcon);"
    );
    setCopyToClipboardAction(
        table->elementAt(2, 4)->addNew<Button>(std::string(Wt::WString::tr("penguin-ui-svg:plus-seccondary").toUTF8() + "icon"), "m-1.5", PenguinUiWidgetTheme::BtnSecondaryWithIcon),
        "->addNew<Button>(std::string(Wt::WString::tr(\"penguin-ui-svg:plus-seccondary\").toUTF8() + \"icon\"), \"m-1.5 text-"+size_combobox->itemText(selected_size_).toUTF8()+" \", PenguinUiWidgetTheme::BtnSecondaryWithIcon);"
    );
    setCopyToClipboardAction(
        table->elementAt(3, 4)->addNew<Button>(std::string(Wt::WString::tr("penguin-ui-svg:plus-success").toUTF8() + "icon"), "m-1.5", PenguinUiWidgetTheme::BtnSuccessWithIcon),
        "->addNew<Button>(std::string(Wt::WString::tr(\"penguin-ui-svg:plus-success\").toUTF8() + \"icon\"), \"m-1.5 text-"+size_combobox->itemText(selected_size_).toUTF8()+" \", PenguinUiWidgetTheme::BtnSuccessWithIcon);"
    );
    setCopyToClipboardAction(
        table->elementAt(4, 4)->addNew<Button>(std::string(Wt::WString::tr("penguin-ui-svg:plus-danger").toUTF8() + "icon"), "m-1.5", PenguinUiWidgetTheme::BtnDangerWithIcon),
        "->addNew<Button>(std::string(Wt::WString::tr(\"penguin-ui-svg:plus-danger\").toUTF8() + \"icon\"), \"m-1.5 text-"+size_combobox->itemText(selected_size_).toUTF8()+" \", PenguinUiWidgetTheme::BtnDangerWithIcon);"
    );
    setCopyToClipboardAction(
        table->elementAt(5, 4)->addNew<Button>(std::string(Wt::WString::tr("penguin-ui-svg:plus-warning").toUTF8() + "icon"), "m-1.5", PenguinUiWidgetTheme::BtnWarningWithIcon),
        "->addNew<Button>(std::string(Wt::WString::tr(\"penguin-ui-svg:plus-warning\").toUTF8() + \"icon\"), \"m-1.5 text-"+size_combobox->itemText(selected_size_).toUTF8()+" \", PenguinUiWidgetTheme::BtnWarningWithIcon);"
    );
    setCopyToClipboardAction(
        table->elementAt(6, 4)->addNew<Button>(std::string(Wt::WString::tr("penguin-ui-svg:plus-info").toUTF8() + "icon"), "m-1.5", PenguinUiWidgetTheme::BtnInfoWithIcon),
        "->addNew<Button>(std::string(Wt::WString::tr(\"penguin-ui-svg:plus-info\").toUTF8() + \"icon\"), \"m-1.5 text-"+size_combobox->itemText(selected_size_).toUTF8()+" \", PenguinUiWidgetTheme::BtnInfoWithIcon);"
    );
    setCopyToClipboardAction(
        table->elementAt(7, 4)->addNew<Button>(std::string(Wt::WString::tr("penguin-ui-svg:plus-alternate").toUTF8() + "icon"), "m-1.5", PenguinUiWidgetTheme::BtnAlternateWithIcon),
        "->addNew<Button>(std::string(Wt::WString::tr(\"penguin-ui-svg:plus-alternate\").toUTF8() + \"icon\"), \"m-1.5 text-"+size_combobox->itemText(selected_size_).toUTF8()+" \", PenguinUiWidgetTheme::BtnAlternateWithIcon);"
    );
    setCopyToClipboardAction(
        table->elementAt(8, 4)->addNew<Button>(std::string(Wt::WString::tr("penguin-ui-svg:plus-inverse").toUTF8() + "icon"), "m-1.5", PenguinUiWidgetTheme::BtnInverseWithIcon),
        "->addNew<Button>(std::string(Wt::WString::tr(\"penguin-ui-svg:plus-inverse\").toUTF8() + \"icon\"), \"m-1.5 text-"+size_combobox->itemText(selected_size_).toUTF8()+" \", PenguinUiWidgetTheme::BtnInverseWithIcon);"
    );

    setCopyToClipboardAction(
        table->elementAt(1, 5)->addNew<Button>(std::string(Wt::WString::tr("penguin-ui-svg:plus-primary").toUTF8()), "m-1.5", PenguinUiWidgetTheme::BtnPrimaryAction),
        "->addNew<Button>(std::string(Wt::WString::tr(\"penguin-ui-svg:plus-primary\").toUTF8()), \"m-1.5 text-"+size_combobox->itemText(selected_size_).toUTF8()+" \", PenguinUiWidgetTheme::BtnPrimaryAction);"
    );
    setCopyToClipboardAction(
        table->elementAt(2, 5)->addNew<Button>(std::string(Wt::WString::tr("penguin-ui-svg:plus-seccondary").toUTF8()), "m-1.5", PenguinUiWidgetTheme::BtnSecondaryAction),
        "->addNew<Button>(std::string(Wt::WString::tr(\"penguin-ui-svg:plus-seccondary\").toUTF8()), \"m-1.5 text-"+size_combobox->itemText(selected_size_).toUTF8()+" \", PenguinUiWidgetTheme::BtnSecondaryAction);"
    );
    setCopyToClipboardAction(
        table->elementAt(3, 5)->addNew<Button>(std::string(Wt::WString::tr("penguin-ui-svg:plus-success").toUTF8()), "m-1.5", PenguinUiWidgetTheme::BtnSuccessAction),
        "->addNew<Button>(std::string(Wt::WString::tr(\"penguin-ui-svg:plus-success\").toUTF8()), \"m-1.5 text-"+size_combobox->itemText(selected_size_).toUTF8()+" \", PenguinUiWidgetTheme::BtnSuccessAction);"
    );
    setCopyToClipboardAction(
        table->elementAt(4, 5)->addNew<Button>(std::string(Wt::WString::tr("penguin-ui-svg:plus-danger").toUTF8()), "m-1.5", PenguinUiWidgetTheme::BtnDangerAction),
        "->addNew<Button>(std::string(Wt::WString::tr(\"penguin-ui-svg:plus-danger\").toUTF8()), \"m-1.5 text-"+size_combobox->itemText(selected_size_).toUTF8()+" \", PenguinUiWidgetTheme::BtnDangerAction);"
    );
    setCopyToClipboardAction(
        table->elementAt(5, 5)->addNew<Button>(std::string(Wt::WString::tr("penguin-ui-svg:plus-warning").toUTF8()), "m-1.5", PenguinUiWidgetTheme::BtnWarningAction),
        "->addNew<Button>(std::string(Wt::WString::tr(\"penguin-ui-svg:plus-warning\").toUTF8()), \"m-1.5 text-"+size_combobox->itemText(selected_size_).toUTF8()+" \", PenguinUiWidgetTheme::BtnWarningAction);"
    );
    setCopyToClipboardAction(
        table->elementAt(6, 5)->addNew<Button>(std::string(Wt::WString::tr("penguin-ui-svg:plus-info").toUTF8()), "m-1.5", PenguinUiWidgetTheme::BtnInfoAction),
        "->addNew<Button>(std::string(Wt::WString::tr(\"penguin-ui-svg:plus-info\").toUTF8()), \"m-1.5 text-"+size_combobox->itemText(selected_size_).toUTF8()+" \", PenguinUiWidgetTheme::BtnInfoAction);"
    );
    setCopyToClipboardAction(
        table->elementAt(7, 5)->addNew<Button>(std::string(Wt::WString::tr("penguin-ui-svg:plus-alternate").toUTF8()), "m-1.5", PenguinUiWidgetTheme::BtnAlternateAction),
        "->addNew<Button>(std::string(Wt::WString::tr(\"penguin-ui-svg:plus-alternate\").toUTF8()), \"m-1.5 text-"+size_combobox->itemText(selected_size_).toUTF8()+" \", PenguinUiWidgetTheme::BtnAlternateAction);"
    );
    setCopyToClipboardAction(
        table->elementAt(8, 5)->addNew<Button>(std::string(Wt::WString::tr("penguin-ui-svg:plus-inverse").toUTF8()), "m-1.5", PenguinUiWidgetTheme::BtnInverseAction),
        "->addNew<Button>(std::string(Wt::WString::tr(\"penguin-ui-svg:plus-inverse\").toUTF8()), \"m-1.5 text-"+size_combobox->itemText(selected_size_).toUTF8()+" \", PenguinUiWidgetTheme::BtnInverseAction);"
    );

    setCopyToClipboardAction(
        table->elementAt(1, 6)->addNew<Button>(std::string(Wt::WString::tr("penguin-ui-svg:loader-primary").toUTF8() + "loader"), "m-1.5", PenguinUiWidgetTheme::BtnPrimaryLoader),
        "->addNew<Button>(std::string(Wt::WString::tr(\"penguin-ui-svg:loader-primary\").toUTF8() + \"loader\"), \"m-1.5 text-"+size_combobox->itemText(selected_size_).toUTF8()+" \", PenguinUiWidgetTheme::BtnPrimaryLoader);"
    );
    setCopyToClipboardAction(
        table->elementAt(2, 6)->addNew<Button>(std::string(Wt::WString::tr("penguin-ui-svg:loader-seccondary").toUTF8() + "loader"), "m-1.5", PenguinUiWidgetTheme::BtnSecondaryLoader),
        "->addNew<Button>(std::string(Wt::WString::tr(\"penguin-ui-svg:loader-seccondary\").toUTF8() + \"loader\"), \"m-1.5 text-"+size_combobox->itemText(selected_size_).toUTF8()+" \", PenguinUiWidgetTheme::BtnSecondaryLoader);"
    );
    setCopyToClipboardAction(
        table->elementAt(3, 6)->addNew<Button>(std::string(Wt::WString::tr("penguin-ui-svg:loader-success").toUTF8() + "loader"), "m-1.5", PenguinUiWidgetTheme::BtnSuccessLoader),
        "->addNew<Button>(std::string(Wt::WString::tr(\"penguin-ui-svg:loader-success\").toUTF8() + \"loader\"), \"m-1.5 text-"+size_combobox->itemText(selected_size_).toUTF8()+" \", PenguinUiWidgetTheme::BtnSuccessLoader);"
    );
    setCopyToClipboardAction(
        table->elementAt(4, 6)->addNew<Button>(std::string(Wt::WString::tr("penguin-ui-svg:loader-danger").toUTF8() + "loader"), "m-1.5", PenguinUiWidgetTheme::BtnDangerLoader),
        "->addNew<Button>(std::string(Wt::WString::tr(\"penguin-ui-svg:loader-danger\").toUTF8() + \"loader\"), \"m-1.5 text-"+size_combobox->itemText(selected_size_).toUTF8()+" \", PenguinUiWidgetTheme::BtnDangerLoader);"
    );
    setCopyToClipboardAction(
        table->elementAt(5, 6)->addNew<Button>(std::string(Wt::WString::tr("penguin-ui-svg:loader-warning").toUTF8() + "loader"), "m-1.5", PenguinUiWidgetTheme::BtnWarningLoader),
        "->addNew<Button>(std::string(Wt::WString::tr(\"penguin-ui-svg:loader-warning\").toUTF8() + \"loader\"), \"m-1.5 text-"+size_combobox->itemText(selected_size_).toUTF8()+" \", PenguinUiWidgetTheme::BtnWarningLoader);"
    );
    setCopyToClipboardAction(
        table->elementAt(6, 6)->addNew<Button>(std::string(Wt::WString::tr("penguin-ui-svg:loader-info").toUTF8() + "loader"), "m-1.5", PenguinUiWidgetTheme::BtnInfoLoader),
        "->addNew<Button>(std::string(Wt::WString::tr(\"penguin-ui-svg:loader-info\").toUTF8() + \"loader\"), \"m-1.5 text-"+size_combobox->itemText(selected_size_).toUTF8()+" \", PenguinUiWidgetTheme::BtnInfoLoader);"
    );
    setCopyToClipboardAction(
        table->elementAt(7, 6)->addNew<Button>(std::string(Wt::WString::tr("penguin-ui-svg:loader-alternate").toUTF8() + "loader"), "m-1.5", PenguinUiWidgetTheme::BtnAlternateLoader),
        "->addNew<Button>(std::string(Wt::WString::tr(\"penguin-ui-svg:loader-alternate\").toUTF8() + \"loader\"), \"m-1.5 text-"+size_combobox->itemText(selected_size_).toUTF8()+" \", PenguinUiWidgetTheme::BtnAlternateLoader);"
    );
    setCopyToClipboardAction(
        table->elementAt(8, 6)->addNew<Button>(std::string(Wt::WString::tr("penguin-ui-svg:loader-inverse").toUTF8() + "loader"), "m-1.5", PenguinUiWidgetTheme::BtnInverseLoader),
        "->addNew<Button>(std::string(Wt::WString::tr(\"penguin-ui-svg:loader-inverse\").toUTF8() + \"loader\"), \"m-1.5 text-"+size_combobox->itemText(selected_size_).toUTF8()+" \", PenguinUiWidgetTheme::BtnInverseLoader);"
    );

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
void WidgetsDisplay::setCopyToClipboardAction(Wt::WInteractWidget  *widget, const std::string &text)
{
    widget->clicked().connect([=]() { 
        widget->doJavaScript("navigator.clipboard.writeText('"+text+"');"); 
        widget->setAttributeValue("x-data", "");
        widget->setAttributeValue("onclick", "$el.classList.add('bg-red-200');setTimeout(() => $el.classList.remove('bg-red-200'), 2000);");
    });

}

