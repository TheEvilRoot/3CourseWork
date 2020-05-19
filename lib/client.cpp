//
// Created by Прогрессивный Долгострой on 25.03.20.
//

#include "client.h"

#include <utility>
#include <sstream>

Client::Client(const char *serverAddress,
               uint16_t port,
               std::string userName,
               std::string realName,
               std::string nickName,
               const std::string& initialChannel,
               ClientView *view):
               MessageListener({ }),
               serverAddress_ { serverAddress },
               port_ { port },
               userName_ {std::move( userName )},
               realName_ {std::move( realName )},
               nickName_ {std::move( nickName )},
               selfSource_ { nickName_, userName_, std::string(serverAddress_) },
               socket_ { nullptr },
               handler_ { nullptr },
               view_{view} {
  if (!initialChannel.empty()) {
    setChannel(initialChannel);
  }
}

Client::~Client() {
  delete socket_;
}

std::string Client::getChannel() const {
  return currentChannel_;
}

bool Client::isConnected() const {
  return socket_ != nullptr &&
    socket_->getConnectionStatus() == 0 &&
    socket_->getSockDesc() >= 0 &&
    socket_->isAlive();
}

void Client::setChannel(const std::string& channel) {
  currentChannel_ = channel;
  if (currentChannel_.front() == '#')
    currentChannel_.erase(0, 1);

  if (isConnected()) {
    sendJoinChannel(channel);
  }
}

bool Client::connect() {
  if (isConnected()) {
    return true;
  }

  socket_ = new Socket(serverAddress_, port_);
  if (socket_->getSockDesc() >= 0 && socket_->getConnectionStatus() == 0) {
    pthread_create(&readThread_, nullptr, &Client::readHandler, this);
  } else {
    perror("new Socket");
    view_->onConnectionFailure("socker failed");
    return false;
  }

  sendCredentials();
  if (!currentChannel_.empty()) {
    setChannel(currentChannel_);
  }
  view_->onConnected();
  return true;
}

Socket *Client::getSocket() const {
  return socket_;
}

void Client::sendRaw(const std::string& rawIrc) {
  if (isConnected()) {
    socket_->send(rawIrc);
  }
}

void Client::sendIrc(std::string command, std::vector<std::string> args, std::string comment) {
  std::ostringstream commandStream;
  commandStream << command << " ";

  if (!args.empty()) {
    for (const auto &arg : args)
      commandStream << arg << " ";
  }

  if (!comment.empty())
    commandStream << ":" << comment;

  std::string text = commandStream.str();
  sendRaw(text);
  
  IrcMessage message(text);
  message.setCommand(command);
  message.setSource(selfSource_);
  for (const auto & arg : args)
    message.addParam(arg);
  message.setTrailing(comment);
  handler_->handleBaseMessage(message);
}

void Client::sendPong(std::string content) {
  sendIrc("PONG",  { content }, "");
}

void Client::sendCredentials() {
  sendNick();
  sendUser();
}

void Client::sendNick() {
  sendIrc("NICK", { nickName_ }, "");
}

void Client::sendUser() {
  sendIrc("USER", { userName_, "0", "*" }, realName_);
}

void Client::sendPrivateMessage(std::string receiver, std::string message) {
  sendIrc("PRIVMSG", { receiver }, message);
}

void Client::sendChannelMessage(std::string channel, std::string message) {
  sendPrivateMessage("#" + channel, message);
}

void Client::sendJoin(std::string dist) {
  sendIrc("JOIN", { dist }, dist);
}

void Client::sendJoinChannel(std::string channel) {
  sendJoin("#" + channel);
}

void Client::joinRead() {
  pthread_join(readThread_, nullptr);
}

void Client::sendCurrentChannelMessage(std::string message) {
  sendChannelMessage(currentChannel_, std::move(message));
}

void Client::shutdown() {
  sendIrc("QUIT", { }, "Goodby!");
  joinRead();
}

IrcHandler *Client::getHandler() const {
  return handler_;
}

void Client::setHandler(IrcHandler *handler) {
  handler_ = handler;
  handler_->addListener(this);
}

void Client::removeHandler() {
  if (handler_) {
    handler_->removeListener(this);
  }
  handler_ = nullptr;
}

void *Client::readHandler(void *clientPtr) {
  auto *client = (Client*) clientPtr;
  auto socket = client->getSocket();

  while (client->isConnected()) {
    socket->bufferRecv(128);
    if (!socket->getBuffer().empty()) {
      Buffer& buffer = socket->getBuffer();
      while (!buffer.empty()) {
        auto data = buffer.pop();
        if (client->handler_ != nullptr)
          client->handler_->pushMessage(data);
      }
    }
  }

  client->view_->onDisconnected();

  return client;
}

bool Client::onBaseMessage(const IrcMessage &msg) {
  std::cerr << msg << std::endl;
  return MessageListener::onBaseMessage(msg);
}

bool Client::onPingMessage(const IrcMessage &message) {
  view_->onPing();
  sendPong(message.getTrailing());
  view_->onPong();
  return false;
}

bool Client::onMOTDStart(const IrcMessage &message) {
  motd_.clear();
  return true;
}

bool Client::onMOTDContent(const IrcMessage &message) {
  motd_ += message.getTrailing();
  return true;
}

bool Client::onMOTDEnds(const IrcMessage &) {
  view_->onMOTDUpdate(motd_);
  return true;
}

bool Client::onPrivMsgMessage(const IrcMessage &message) {
  view_->onPrivateMessage(
      message.getSource().streamString() +
      " :: " +
      message.getTrailing());
  return true;
}

bool Client::onJoinMessage(const IrcMessage &message) {
  if (message.getSource().nickName == selfSource_.nickName) {
    currentChannel_ = message.getTrailing();
    currentChannel_.erase(currentChannel_.begin());
    currentChannel_.erase(currentChannel_.end() - 1);
  }
  view_->onUserJoin(
      message.getSource().streamString() +
      " has joined channel " +
      message.getTrailing());
  return true;
}

bool Client::onNamesReplyMessage(const IrcMessage &message) {
  auto names = IrcParser::splitString(message.getTrailing(), ' ');
  view_->onCurrentChannelUsersUpdated(names);

  return true;
}

bool Client::onExpectedError(const IrcMessage &message) {
  view_->onErrorMessage(message.getTrailing());
  return true;
}