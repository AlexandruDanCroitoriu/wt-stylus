#pragma once
#include <Wt/Dbo/Types.h>
#include <Wt/WGlobal.h>
#include <Wt/Dbo/Dbo.h>
#include <Wt/Dbo/WtSqlTraits.h>

namespace dbo = Wt::Dbo;

class THECLASS
{
public:
  Wt::WString name;

  template <class Action>
  void persist(Action &a)
  {
    dbo::field(a, name, "name");
  }
};
