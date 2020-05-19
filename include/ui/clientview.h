
#ifndef ANSIRC_INCLUDE_UI_CLIENTVIEW_H_
#define ANSIRC_INCLUDE_UI_CLIENTVIEW_H_

#include <string>

class ClientView {
 public:
  ClientView() = default;
  virtual ~ClientView() = default;

  virtual void newMessage(std::string msg) = 0;
  virtual void motdUpdate(std::string motd) = 0;

};

#endif //ANSIRC_INCLUDE_UI_CLIENTVIEW_H_
