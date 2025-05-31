#include "003-Auth/UserDetailsModel.h"
#include "002-Dbo/User.h"
#include "002-Dbo/Session.h"


const WFormModel::Field
UserDetailsModel::FavouritePetField = "favourite-pet";

UserDetailsModel::UserDetailsModel(Session& session)
  : WFormModel(),
    session_(session)
{
  addField(FavouritePetField, WString::tr("Auth:str-favourite-pet-info"));
}

void UserDetailsModel::save(const Auth::User& authUser)
{
  Dbo::ptr<User> user = session_.user(authUser);
  user.modify()->favouritePet_ = valueText(FavouritePetField).toUTF8();
}
