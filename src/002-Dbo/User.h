#pragma once
#include <Wt/Dbo/Types.h>
#include <Wt/WGlobal.h>

class User;
using AuthInfo = Wt::Auth::Dbo::AuthInfo<User>;

class User {
public:
  User() = default;
  explicit User(const std::string& name);

  template<class Action>
  void persist(Action& a)
  {
    Wt::Dbo::field(a, name_, "name");
    Wt::Dbo::hasOne(a, m_authInfo, "user");
  }
private:
  std::string name_;
  Wt::Dbo::weak_ptr<AuthInfo> m_authInfo;
};


DBO_EXTERN_TEMPLATES(User)