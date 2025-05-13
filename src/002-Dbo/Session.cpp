#include "002-Dbo/Session.h"
#include "002-Dbo/Tables.h"
#include <Wt/Dbo/ptr.h>

// #include <Wt/Dbo/backend/Postgres.h>

#include <Wt/WString.h>
#include <iostream>

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
  setConnection(std::move(connection));

  mapClass<THECLASS>("name_of_tha_class");

  try
  {
    createTables();
    createInitialData();
    std::cerr << "Created database.\n";
  }
  catch (Wt::Dbo::Exception &e)
  {
    std::cerr << e.what() << '\n';
    std::cerr << "Using existing database\n";
  }
}

void Session::createInitialData()
{
  // dbo::Transaction transaction(*this);

  // dbo::ptr<THECLASS> class = add(std::make_unique<THECLASS>());
  // class.modify()->class_name = "value";

  // transaction.commit();
}
