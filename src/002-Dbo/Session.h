#pragma once
#include <Wt/Dbo/Session.h>
#include <Wt/Dbo/backend/Sqlite3.h>

using namespace Wt;

class Session : public Wt::Dbo::Session
{
public:
    explicit Session(const std::string &sqliteDb);

private:
    void createInitialData();
};