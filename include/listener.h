#ifndef ANSIRC_INCLUDE_LISTENER_H_
#define ANSIRC_INCLUDE_LISTENER_H_

#include <iostream>
#include <vector>
#include <string>

#include "message.h"

class MessageListener {
public:
  MessageListener(std::initializer_list<std::string> whitelist = { }): whitelist_ { whitelist.begin(), whitelist.end() } { };
  virtual ~MessageListener() = default;

  virtual bool onBaseMessage(const IrcMessage &msg) {
    auto cmd = msg.getCommand();

    if (cmd == "PING")
      return onPingMessage(msg);
    if (cmd == "PRIVMSG")
      return onPrivMsgMessage(msg);

    std::cout << msg << std::endl;

    return false;
  }

  virtual bool onPingMessage(const IrcMessage &) = 0;
  virtual bool onPrivMsgMessage(const IrcMessage &) = 0;

  bool shouldHandle(const std::string &messageType) const {
    return whitelist_.empty() || std::find(whitelist_.begin(), whitelist_.end(), messageType) != whitelist_.end();
  };

private:
  std::vector<std::string> whitelist_;
};

#endif // ANSIRC_INCLUDE_LISTENER_H_
