#include "003-Auth/AuthWidget.h"
#include "RegistrationView.h"
#include "002-Dbo/Session.h"
#include "003-Auth/UserDetailsModel.h"
#include "002-Dbo/User.h"
#include "002-Dbo/Permission.h"
#include <Wt/Auth/PasswordService.h>
#include <Wt/WApplication.h>

AuthWidget::AuthWidget(Session& session)
  : Auth::AuthWidget(Session::auth(), session.users(), session.login()),
    session_(session)
{ 
  	setInternalBasePath("/auth");
    
    createInitialData();
    // model()->addPasswordAuth(&Session::passwordAuth());
    // model()->addOAuth(Session::oAuth());
    // setRegistrationEnabled(true);

    // processEnvironment();
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


void AuthWidget::createInitialData()
{
  Wt::Dbo::Transaction t(session_);
  
  // STYLUS_FILES_MANAGER permission for admin user maxuli
  Wt::Dbo::ptr<Permission> permission = session_.find<Permission>().where("name = ?").bind("STYLUS_FILES_MANAGER").resultValue();
  if (!permission) {
    permission = session_.add(std::make_unique<Permission>("STYLUS_FILES_MANAGER"));
  }
  
  std::string username = "maxuli";
  std::string email = "admin@example.com";
  std::string password = "asdfghj1";

  // Check if user already exists
  Wt::Dbo::ptr<User> existingUser = session_.find<User>().where("name = ?").bind(username).resultValue();
  if (existingUser) {
    wApp->log("info") << "User already exists: " << username;
    return; // User already exists, no need to create again
  }
  
  
  Wt::Dbo::ptr<User> user = session_.add(std::make_unique<User>(username));
  user.modify()->permissions_.insert(permission);
  // permission.modify()->users_.insert(user);

  Wt::Auth::User authUser = session_.users().registerNew();
  authUser.addIdentity(Wt::Auth::Identity::LoginName, username);
  authUser.setEmail(email);
  session_.passwordAuth().updatePassword(authUser, password);
  
  Wt::Dbo::ptr<AuthInfo> authInfo = session_.find<AuthInfo>("where id = ?").bind(authUser.id());
  authInfo.modify()->setUser(user);
  
  t.commit();
}
