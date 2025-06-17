#pragma once
#include <Wt/WTheme.h>
#include <unordered_map>
enum ThemeConfig
{
    Arctic,
    Minimal,
    Modern,
    HighContrast,
    NeoBrutalism,
    Halloween,
    Zombie,
    Pastel,
    Nineties,
    Christmas,
    Prototype,
    News,
    Industrial
};


// enum WidgetThemeRole {
//   MenuItemIcon = 100,
//   MenuItemCheckBox = 101,
//   MenuItemClose = 102,

//   DialogCoverWidget = 200,
//   DialogTitleBar = 201,
//   DialogBody = 202,
//   DialogFooter = 203,
//   DialogCloseIcon = 204,
//   DialogContent = 205, // For bootstrap 3 modal-content style

//   TableViewRowContainer = 300,

//   DatePickerPopup = 400,
//   DatePickerIcon = 401,
//   TimePickerPopup = 410,
//   TimePickerPopupContent = 411,

//   PanelTitleBar = 500,
//   PanelCollapseButton = 501,
//   PanelTitle = 502,
//   PanelBody = 503,
//   PanelBodyContent = 504,

//   AuthWidgets = 600,

//   InPlaceEditing = 700,
//   InPlaceEditingButtonsContainer = 701,
//   InPlaceEditingButton = 702,

//   Navbar = 800,
//   NavCollapse = 801,
//   NavBrand = 802,
//   NavbarForm = 803,
//   NavbarSearchForm = 804,
//   NavbarSearchInput = 805,
//   NavbarMenu = 806,
//   NavbarBtn = 807,
//   NavbarAlignLeft = 808,
//   NavbarAlignRight = 809
// };
enum PenguinUiWidgetTheme {
    // widgets
    WComboBox = 10,
    WLineEdit = 11,

    // Buttons
    ButtonPrimary = 100,
    ButtonSecondary = 101,
    ButtonSuccess = 102,
    ButtonDanger = 103,
    ButtonWarning = 104,
    ButtonInfo = 105,
    ButtonAlternate = 106,
    ButtonInverse = 107,
    ButtonPrimaryOutline = 108,
    ButtonSecondaryOutline = 109,
    ButtonSuccessOutline = 110,
    ButtonDangerOutline = 111,
    ButtonWarningOutline = 112,
    ButtonInfoOutline = 113,
    ButtonAlternateOutline = 114,
    ButtonInverseOutline = 115,
    

};

class Theme : public Wt::WTheme
{
public:
    Theme(std::string name = "default");

    void setWidgetThemeClasses(PenguinUiWidgetTheme widgetTheme, const std::string &styleClasses);
    void addWidgetThemeClasses(PenguinUiWidgetTheme widgetTheme, const std::string &styleClasses);

    virtual std::string name() const override;

    /*! \brief Returns the stylesheets.
     *
     * Returns wt.css, plus on IE wt_ie.css, plus on IE6 wt_ie6.css. The
     * style sheets are located in the theme directory in the resources
     * folder.
     */
    virtual std::vector<Wt::WLinkedCssStyleSheet> styleSheets() const override;

    virtual void apply(Wt::WWidget *widget, Wt::WWidget *child, int widgetRole) const override;
    virtual void apply(Wt::WWidget *widget, Wt::DomElement &element, int elementRole) const override;
    void apply(Wt::WWidget *widget, PenguinUiWidgetTheme widgetTheme);

    /*! \brief Returns a generic CSS class name for a disabled element.
     *
     * The CSS class Wt-disabled is applied to disabled classes.
     */
    virtual std::string disabledClass() const override;

    /*! \brief Returns a generic CSS class name for an active element.
     *
     * The CSS class Wt-selected is applied to active classes.
     */
    virtual std::string activeClass() const override;

    virtual std::string utilityCssClass(int utilityCssClassRole) const override;

    /*! \brief Returns whether the theme allows for an anchor to be styled
     *         as a button.
     *
     * Returns false.
     */
    virtual bool canStyleAnchorAsButton() const override;

    virtual void applyValidationStyle(Wt::WWidget *widget,
                                      const Wt::WValidator::Result &validation,
                                      Wt::WFlags<Wt::ValidationStyleFlag> styles) const override;

    virtual bool canBorderBoxElement(const Wt::DomElement &element) const override;

    ThemeConfig current_theme_ = ThemeConfig::Arctic; // Default theme
    void setPenguinUiConfig();
private:
    std::string name_;
    std::unordered_map<int, std::string> widgetThemeClasses_;

};
