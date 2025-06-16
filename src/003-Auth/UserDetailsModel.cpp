#include "003-Auth/UserDetailsModel.h"
#include "002-Dbo/User.h"
#include "002-Dbo/Session.h"


#include <Wt/Auth/Identity.h>

const Wt::WFormModel::Field
UserDetailsModel::FavouritePetField = "favourite-pet";

UserDetailsModel::UserDetailsModel(Session& session)
  : WFormModel(),
    session_(session)
{
  addField(FavouritePetField, Wt::WString::tr("Auth:favourite-pet-info"));
}

void UserDetailsModel::save(const Wt::Auth::User& authUser)
{
  Wt::Dbo::ptr<User> user = session_.user(authUser);
  user.modify()->favouritePet_ = valueText(FavouritePetField).toUTF8();
  user.modify()->name_ = authUser.identity(Wt::Auth::Identity::LoginName).toUTF8();
}
