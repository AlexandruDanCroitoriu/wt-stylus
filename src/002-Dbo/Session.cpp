#include "002-Dbo/Session.h"

#include <Wt/Auth/AuthService.h>
#include <Wt/Auth/HashFunction.h>
#include <Wt/Auth/PasswordService.h>
#include <Wt/Auth/PasswordStrengthValidator.h>
#include <Wt/Auth/PasswordVerifier.h>
#include <Wt/Auth/GoogleService.h>
#include <Wt/Auth/FacebookService.h>
#include <Wt/Auth/Dbo/AuthInfo.h>
// #include <Wt/Auth/Mfa/TotpProcess.h>

#include <Wt/Dbo/backend/Sqlite3.h>

using namespace Wt;

namespace {

  Auth::AuthService myAuthService;
  Auth::PasswordService myPasswordService(myAuthService);
  std::vector<std::unique_ptr<Auth::OAuthService>> myOAuthServices;

}


void addUser(Wt::Dbo::Session& session, UserDatabase& users, const std::string& loginName,
             const std::string& email, const std::string& password)
{
  Wt::Dbo::Transaction t(session);
  auto user = session.addNew<User>(loginName);
  auto authUser = users.registerNew();
  authUser.addIdentity(Wt::Auth::Identity::LoginName, loginName);
  authUser.setEmail(email);
  myPasswordService.updatePassword(authUser, password);

  // Link User and auth user
  Wt::Dbo::ptr<AuthInfo> authInfo = session.find<AuthInfo>("where id = ?").bind(authUser.id());
  authInfo.modify()->setUser(user);
}


void Session::configureAuth()
{
  myAuthService.setAuthTokensEnabled(true, "logincookie");
  myAuthService.setEmailVerificationEnabled(false);
  myAuthService.setEmailVerificationRequired(false);

  // myAuthService.setMfaProvider(Wt::Auth::Identity::MultiFactor);
  // myAuthService.setMfaRequired(true);
  // myAuthService.setMfaThrottleEnabled(true);

  auto verifier = std::make_unique<Auth::PasswordVerifier>();
  verifier->addHashFunction(std::make_unique<Auth::BCryptHashFunction>(12));
  myPasswordService.setVerifier(std::move(verifier));
  myPasswordService.setPasswordThrottle(std::make_unique<Wt::Auth::AuthThrottle>());
  myPasswordService.setStrengthValidator(std::make_unique<Auth::PasswordStrengthValidator>());

  if (Auth::GoogleService::configured()) {
    myOAuthServices.push_back(std::make_unique<Auth::GoogleService>(myAuthService));
  }

  if (Auth::FacebookService::configured()) {
    myOAuthServices.push_back(std::make_unique<Auth::FacebookService>(myAuthService));
  }

  for (const auto& oAuthService : myOAuthServices) {
    oAuthService->generateRedirectEndpoint();
  }

  if (created_) {
    addUser(*this, *users_.get(), "admin", "admin@example.com", "admin");
    addUser(*this, *users_.get(), "user", "user@example.com", "user");
  }
}

Session::Session(const std::string &sqliteDb)
{
  // const char *postgres_password = std::getenv("POSTGRES_PASSWORD");
  // if (postgres_password)
  //   std::cout << "POSTGRES_PASSWORD: recived succesfuly" << std::endl;
  // else
  //   std::cout << "POSTGRES_PASSWORD is not set." << std::endl;

  // const char *vps_ip = std::getenv("VPS_IP");
  // if (vps_ip)
  //   std::cout << "VPS_IP: recived succesfuly" << std::endl;
  // else
  //   std::cout << "VPS_IP is not set." << std::endl;

  // std::string postgres_conn_str = "host=" + std::string(vps_ip) + " dbname=postgres user=postgres password=" + std::string(postgres_password);
  // auto connection = std::make_unique<Dbo::backend::Postgres>(postgres_conn_str.c_str());
  auto connection = std::make_unique<Dbo::backend::Sqlite3>(sqliteDb);

  connection->setProperty("show-queries", "true");

  setConnection(std::move(connection));

  mapClass<User>("user");
  mapClass<AuthInfo>("auth_info");
  mapClass<AuthInfo::AuthIdentityType>("auth_identity");
  mapClass<AuthInfo::AuthTokenType>("auth_token");

 try {
    if (!created_) {
      createTables();
      created_ = true;
      Wt::log("info") << "Created database.";
    } else {
      Wt::log("info") << "Using existing database";
    }
  } catch (Wt::Dbo::Exception& e) {
    Wt::log("info") << "Using existing database";
  }
  users_ = std::make_unique<UserDatabase>(*this);
}


Auth::AbstractUserDatabase& Session::users()
{
  return *users_;
}

dbo::ptr<User> Session::user() const
{
  if (login_.loggedIn()) {
    dbo::ptr<AuthInfo> authInfo = users_->find(login_.user());
    return authInfo->user();
  } else
    return dbo::ptr<User>();
}

const Auth::AuthService& Session::auth()
{
  return myAuthService;
}

const Auth::PasswordService& Session::passwordAuth()
{
  return myPasswordService;
}

std::vector<const Auth::OAuthService *> Session::oAuth()
{
  std::vector<const Auth::OAuthService *> result;
  result.reserve(myOAuthServices.size());
  for (const auto& auth : myOAuthServices) {
    result.push_back(auth.get());
  }
  return result;
}


// void Session::createInitialData()
// {
//   // dbo::Transaction transaction(*this);

//   // dbo::ptr<THECLASS> class = add(std::make_unique<THECLASS>());
//   // class.modify()->class_name = "value";

//   // transaction.commit();
// }
