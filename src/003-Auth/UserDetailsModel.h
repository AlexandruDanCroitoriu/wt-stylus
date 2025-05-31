#pragma once
#include <Wt/WFormModel.h>

using namespace Wt;

class Session;

class UserDetailsModel : public WFormModel
{
public:
  static const Field FavouritePetField;

  UserDetailsModel(Session& session);

  void save(const Auth::User& user);

private:
  Session& session_;
};
