#ifndef ANSIRC_SRC_CLIENT_H_
#define ANSIRC_SRC_CLIENT_H_

#include <string>

#include "socket.h"
#include "irc.h"

class Client : public IRCClient {
 public:
  Client(const char *serverAddress, uint16_t port, std::string userName, std::string realName, std::string nickName, const std::string& initialChannel);
  ~Client() override;

  void setChannel(const std::string& channel);
  [[nodiscard]] std::string getChannel() const;

  [[nodiscard]] bool isConnected() const;
  void connect();

  void shutdown();

  void sendIrc(std::string command, std::vector<std::string> args = { }, std::string comment = "") override;

  void sendPong(std::string content) override;

  void sendCredentials() override;

  void sendNick() override;

  void sendUser() override;

  void sendPrivateMessage(std::string receiver, std::string message) override;

  void sendChannelMessage(std::string channel, std::string message) override;

  void sendJoin(std::string dist) override;

  void sendJoinChannel(std::string channel) override;

  void sendCurrentChannelMessage(std::string message);

  void joinRead();

 private:
  const char *serverAddress_;
  uint16_t port_;
  std::string userName_;
  std::string realName_;
  std::string nickName_;
  std::string currentChannel_;

  Socket *socket_;
  pthread_t readThread_;

  static void *readHandler(void *clientPtr);

 protected:
  Socket * getSocket() const;
};

#endif
