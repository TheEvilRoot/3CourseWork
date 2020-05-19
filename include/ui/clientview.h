
#ifndef ANSIRC_INCLUDE_UI_CLIENTVIEW_H_
#define ANSIRC_INCLUDE_UI_CLIENTVIEW_H_

#include <string>

#include "listener.h"

class ClientView {
 public:
  ClientView() = default;
  ~ClientView() = default;

  virtual void onMOTDUpdate(std::string motd) = 0;
  virtual void onPrivateMessage(std::string message) = 0;
  virtual void onUserJoin(std::string message) = 0;

  virtual void onPing() = 0;
  virtual void onPong() = 0;

  virtual void onConnected() = 0;
  virtual void onDisconnected() = 0;
  virtual void onConnectionFailure(std::string reason) = 0;

  virtual void onCurrentChannelUsersUpdated(const std::vector<std::string> &users) = 0;

};

#endif //ANSIRC_INCLUDE_UI_CLIENTVIEW_H_