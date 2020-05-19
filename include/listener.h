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
    if (cmd == "JOIN")
      return onJoinMessage(msg);
    
    if (cmd == "004")
      return onMyInfoMessage(msg);
    if (cmd == "005")
      return onServerSupportFeaturesMessage(msg);
    if (cmd == "042")
      return onYourIdMessage(msg);
    if (cmd == "254")
      return onChannelsFormedMessage(msg);
    if (cmd == "375")
      return onMOTDStart(msg);
    if (cmd == "372")
      return onMOTDContent(msg);
    if (cmd == "376")
      return onMOTDEnds(msg);
    if (cmd == "353")
      return onNamesReplyMessage(msg);
    if (cmd == "403")
      return onExpectedError(msg);
    if (cmd == "404")
      return onExternalMessage(msg);
    
    return false;
  }

  virtual bool onMyInfoMessage(const IrcMessage &) { return true; };
  virtual bool onPingMessage(const IrcMessage &) { return true; };
  virtual bool onPrivMsgMessage(const IrcMessage &) { return true; };
  virtual bool onServerSupportFeaturesMessage(const IrcMessage &) { return true; }
  virtual bool onYourIdMessage(const IrcMessage &) { return true; }
  virtual bool onChannelsFormedMessage(const IrcMessage &) { return true; }
  virtual bool onMOTDStart(const IrcMessage &) { return true; }
  virtual bool onMOTDContent(const IrcMessage &) { return true; }
  virtual bool onMOTDEnds(const IrcMessage &) { return true; }
  virtual bool onJoinMessage(const IrcMessage &) { return true; }
  virtual bool onNamesReplyMessage(const IrcMessage &) { return true; }

  virtual bool onExpectedError(const IrcMessage &) { return true; }
  virtual bool onExternalMessage(const IrcMessage&) { return true; }

  virtual bool shouldHandle(const std::string &messageType) const {
    return whitelist_.empty() || std::find(whitelist_.begin(), whitelist_.end(), messageType) != whitelist_.end();
  };

private:
  std::vector<std::string> whitelist_;
};

#endif // ANSIRC_INCLUDE_LISTENER_H_
