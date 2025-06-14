#pragma once
#include <Wt/Auth/AuthWidget.h>

using namespace Wt;

class Session;

class AuthWidget : public Auth::AuthWidget
{
public:
  AuthWidget(Session& session);

  /* We will use a custom registration view */
  virtual std::unique_ptr<WWidget> createRegistrationView(const Auth::Identity& id) override;

private:
  Session& session_;
  void createInitialData();
};