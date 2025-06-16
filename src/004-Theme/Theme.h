#pragma once
#include <Wt/WTheme.h>

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



enum WidgetThemeRoleRest {
    WComboBox = 10,
    WLineEdit = 11,
};

class Theme : public Wt::WTheme
{
public:
    Theme(std::string name = "arctic");

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
private:
    std::string name_;
};
