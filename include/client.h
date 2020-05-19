#ifndef ANSIRC_SRC_CLIENT_H_
#define ANSIRC_SRC_CLIENT_H_

#include <string>

#include "socket.h"
#include "irc.h"
#include "handler.h"

#include "ui/clientview.h"

class Client : public IrcClient, private MessageListener {
 public:
  Client(const char *serverAddress,
      uint16_t port,
      std::string userName,
      std::string realName,
      std::string nickName,
      const std::string& initialChannel,
      ClientView *view);
  ~Client() override;

  void setChannel(const std::string& channel);
  [[nodiscard]] std::string getChannel() const;

  [[nodiscard]] bool isConnected() const;
  bool connect();

  [[nodiscard]] IrcHandler * getHandler() const;
  void setHandler(IrcHandler *handler);
  void removeHandler();

  void shutdown();

  void sendRaw(const std::string& rawIrc);

  void sendIrc(std::string command, std::vector<std::string> args, std::string comment)override;

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
  bool onBaseMessage(const IrcMessage &msg) override;
  bool onPrivMsgMessage(const IrcMessage &message) override;
  bool onJoinMessage(const IrcMessage &message) override;
  bool onNamesReplyMessage(const IrcMessage &message) override;
  bool onPingMessage(const IrcMessage &message) override;
  bool onMOTDStart(const IrcMessage &) override;
  bool onMOTDContent(const IrcMessage &) override;
  bool onMOTDEnds(const IrcMessage &) override;

  const char *serverAddress_;
  uint16_t port_;
  std::string userName_;
  std::string realName_;
  std::string nickName_;
  std::string currentChannel_;
  MessageSource selfSource_;
  std::string motd_;

  Socket *socket_;
  pthread_t readThread_;
  IrcHandler *handler_;

  ClientView *view_;

  static void *readHandler(void *clientPtr);

 protected:
  Socket * getSocket() const;
};

#endif
