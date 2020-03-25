#ifndef ANSIRC_SRC_IRC_H_
#define ANSIRC_SRC_IRC_H_

#include <string>
#include <vector>

class IRCClient {
 public:
  IRCClient() = default;
  virtual ~IRCClient() = default;

  virtual void sendIrc(std::string command, std::vector<std::string> args, std::string comment) = 0;

  virtual void sendPong(std::string content) = 0;

  virtual void sendCredentials() = 0;
  virtual void sendNick() = 0;
  virtual void sendUser() = 0;

  virtual void sendPrivateMessage(std::string receiver, std::string message) = 0;
  virtual void sendChannelMessage(std::string channel, std::string message) = 0;

  virtual void sendJoin(std::string dist) = 0;
  virtual void sendJoinChannel(std::string channel) = 0;
};

#endif //ANSIRC_SRC_IRC_H_
