#include "002-Dbo/Session.h"
#include "002-Dbo/Tables/Permission.h"
#include "001-App/Server.h"

#include <Wt/Dbo/backend/Sqlite3.h>

using namespace Wt;


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
  mapClass<Permission>("permission");
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

dbo::ptr<User> Session::user(const Wt::Auth::User& authUser)
{
  dbo::ptr<AuthInfo> authInfo = users_->find(authUser);

  dbo::ptr<User> user = authInfo->user();

  if (!user) {
    user = add(std::make_unique<User>());
    authInfo.modify()->setUser(user);
  }

  return user;
}

const Auth::AuthService& Session::auth()
{
  return Server::authService;
}

const Auth::PasswordService& Session::passwordAuth()
{
  return Server::passwordService;
}

std::vector<const Auth::OAuthService *> Session::oAuth()
{
  std::vector<const Wt::Auth::OAuthService *> result;
  result.reserve(Server::oAuthServices.size());
  for (const auto& auth : Server::oAuthServices) {
    result.push_back(auth.get());
  }
  return result;
}


void Session::createInitialData()
{
  Wt::Dbo::Transaction t(*this);

  t.commit();
}