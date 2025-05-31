#pragma once
#include <Wt/Dbo/Types.h>
#include <Wt/WGlobal.h>

class User;
using AuthInfo = Wt::Auth::Dbo::AuthInfo<User>;

class User {
public:
  User() = default;
  explicit User(const std::string& name);

  std::string name_;
  std::string favouritePet_;
  Wt::Dbo::weak_ptr<AuthInfo> authInfo_;

  template<class Action>
  void persist(Action& a)
  {
    Wt::Dbo::field(a, name_, "name");
    Wt::Dbo::field(a, favouritePet_, "favourite_pet");
    Wt::Dbo::hasOne(a, authInfo_, "user");
  }
private:
};


DBO_EXTERN_TEMPLATES(User)