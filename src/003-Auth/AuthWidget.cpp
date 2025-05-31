#include "003-Auth/AuthWidget.h"
#include "RegistrationView.h"
#include "002-Dbo/Session.h"
#include "003-Auth/UserDetailsModel.h"

AuthWidget::AuthWidget(Session& session)
  : Auth::AuthWidget(Session::auth(), session.users(), session.login()),
    session_(session)
{ 
  	setInternalBasePath("/auth");
}

std::unique_ptr<WWidget> AuthWidget::createRegistrationView(const Auth::Identity& id)
{
  auto registrationView = std::make_unique<RegistrationView>(session_, this);
  std::unique_ptr<Auth::RegistrationModel> model = createRegistrationModel();

  if (id.isValid())
    model->registerIdentified(id);

  registrationView->setModel(std::move(model));
  return std::move(registrationView);
}
