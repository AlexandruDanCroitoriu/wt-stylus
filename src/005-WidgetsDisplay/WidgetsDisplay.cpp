#include "005-WidgetsDisplay/WidgetsDisplay.h"
#include <Wt/WApplication.h>
#include <Wt/WPushButton.h>
#include "004-Theme/Theme.h"

WidgetsDisplay::WidgetsDisplay()
{

}


void WidgetsDisplay::createButtons()
{
    auto content = addNew<Wt::WContainerWidget>();
    std::string wrapper_styles = "flex items-center space-x-2 flex-wrap mb-2";
    auto content_default = content->addNew<Wt::WContainerWidget>();
    content_default->setStyleClass(wrapper_styles);
    auto btn_primary = content_default->addNew<Wt::WPushButton>("primary");
    auto btn_secondary = content_default->addNew<Wt::WPushButton>("secondary");
    auto btn_success = content_default->addNew<Wt::WPushButton>("success");
    auto btn_danger = content_default->addNew<Wt::WPushButton>("danger");
    auto btn_warning = content_default->addNew<Wt::WPushButton>("warning");
    auto btn_info = content_default->addNew<Wt::WPushButton>("info");
    auto btn_alternate = content_default->addNew<Wt::WPushButton>("alternate");
    auto btn_inverse = content_default->addNew<Wt::WPushButton>("inverse");
    btn_primary->setStyleClass("text-xs");
    btn_secondary->setStyleClass("text-sm");
    btn_success->setStyleClass("text-base");
    btn_danger->setStyleClass("text-lg");
    btn_warning->setStyleClass("text-xl");
    btn_info->setStyleClass("text-lg");
    btn_alternate->setStyleClass("text-base");
    btn_inverse->setStyleClass("text-sm");
    dynamic_cast<Theme*>(wApp->theme().get())->apply(btn_primary, PenguinUiWidgetTheme::ButtonPrimary);
    dynamic_cast<Theme*>(wApp->theme().get())->apply(btn_secondary, PenguinUiWidgetTheme::ButtonSecondary);
    dynamic_cast<Theme*>(wApp->theme().get())->apply(btn_success, PenguinUiWidgetTheme::ButtonSuccess);
    dynamic_cast<Theme*>(wApp->theme().get())->apply(btn_danger, PenguinUiWidgetTheme::ButtonDanger);
    dynamic_cast<Theme*>(wApp->theme().get())->apply(btn_warning, PenguinUiWidgetTheme::ButtonWarning);
    dynamic_cast<Theme*>(wApp->theme().get())->apply(btn_info, PenguinUiWidgetTheme::ButtonInfo);
    dynamic_cast<Theme*>(wApp->theme().get())->apply(btn_alternate, PenguinUiWidgetTheme::ButtonAlternate);
    dynamic_cast<Theme*>(wApp->theme().get())->apply(btn_inverse, PenguinUiWidgetTheme::ButtonInverse);

    auto content_outline = content->addNew<Wt::WContainerWidget>();
    content_outline->setStyleClass(wrapper_styles);
    btn_primary = content_outline->addNew<Wt::WPushButton>("text-xs");
    btn_secondary = content_outline->addNew<Wt::WPushButton>("text-sm");
    btn_success = content_outline->addNew<Wt::WPushButton>("text-base");
    btn_danger = content_outline->addNew<Wt::WPushButton>("text-lg");
    btn_warning = content_outline->addNew<Wt::WPushButton>("text-xl");
    btn_info = content_outline->addNew<Wt::WPushButton>("text-lg");
    btn_alternate = content_outline->addNew<Wt::WPushButton>("text-base");
    btn_inverse = content_outline->addNew<Wt::WPushButton>("text-sm");
    btn_primary->setStyleClass("text-xs");
    btn_secondary->setStyleClass("text-sm");
    btn_success->setStyleClass("text-base");
    btn_danger->setStyleClass("text-lg");
    btn_warning->setStyleClass("text-xl");
    btn_info->setStyleClass("text-lg");
    btn_alternate->setStyleClass("text-base");
    btn_inverse->setStyleClass("text-sm");
    dynamic_cast<Theme*>(wApp->theme().get())->apply(btn_primary, PenguinUiWidgetTheme::ButtonPrimaryOutline);
    dynamic_cast<Theme*>(wApp->theme().get())->apply(btn_secondary, PenguinUiWidgetTheme::ButtonSecondaryOutline);
    dynamic_cast<Theme*>(wApp->theme().get())->apply(btn_success, PenguinUiWidgetTheme::ButtonSuccessOutline);
    dynamic_cast<Theme*>(wApp->theme().get())->apply(btn_danger, PenguinUiWidgetTheme::ButtonDangerOutline);
    dynamic_cast<Theme*>(wApp->theme().get())->apply(btn_warning, PenguinUiWidgetTheme::ButtonWarningOutline);
    dynamic_cast<Theme*>(wApp->theme().get())->apply(btn_info, PenguinUiWidgetTheme::ButtonInfoOutline);
    dynamic_cast<Theme*>(wApp->theme().get())->apply(btn_alternate, PenguinUiWidgetTheme::ButtonAlternateOutline);
    dynamic_cast<Theme*>(wApp->theme().get())->apply(btn_inverse, PenguinUiWidgetTheme::ButtonInverseOutline);
}
