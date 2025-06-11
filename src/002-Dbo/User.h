#pragma once
#include <Wt/Dbo/Types.h>
#include <Wt/WGlobal.h>
#include "002-Dbo/Permission.h"

class User;
using AuthInfo = Wt::Auth::Dbo::AuthInfo<User>;

class User : public Wt::Dbo::Dbo<User>
{
public:
  User() = default;
  explicit User(const std::string& name);

  std::string name_;
  std::string favouritePet_;
  Wt::Dbo::weak_ptr<AuthInfo> authInfo_;
  Wt::Dbo::collection< Wt::Dbo::ptr<Permission> > permissions_;

  bool hasPermission(const Wt::Dbo::ptr<Permission> permission) const;

  template<class Action>
  void persist(Action& a)
  {
    Wt::Dbo::field(a, name_, "name");
    Wt::Dbo::field(a, favouritePet_, "favourite_pet");
    Wt::Dbo::hasOne(a, authInfo_, "user");
    Wt::Dbo::hasMany(a, permissions_, Wt::Dbo::ManyToMany, "users_permissions");
  }
private:
};


DBO_EXTERN_TEMPLATES(User)