#include "002-Dbo/User.h"

#include <Wt/Dbo/Impl.h>
#include <Wt/Auth/Dbo/AuthInfo.h>

DBO_INSTANTIATE_TEMPLATES(User)

User::User(const std::string& name)
  : name_(name)
{
}
