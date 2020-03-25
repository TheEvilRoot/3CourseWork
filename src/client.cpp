//
// Created by Прогрессивный Долгострой on 25.03.20.
//

#include "client.h"

#include <utility>
#include <iostream>
#include <sstream>

Client::Client(const char *serverAddress,
               uint16_t port,
               std::string userName,
               std::string realName,
               std::string nickName,
               const std::string& initialChannel):
               serverAddress_ { serverAddress },
               port_ { port },
               userName_ {std::move( userName )},
               realName_ {std::move( realName )},
               nickName_ {std::move( nickName )},
               socket_ { nullptr } { setChannel(initialChannel); }

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

void Client::connect() {
  if (isConnected()) {
    return;
  }

  socket_ = new Socket(serverAddress_, port_);
  if (socket_->getSockDesc() >= 0 && socket_->getConnectionStatus() == 0) {
    pthread_create(&readThread_, nullptr, &Client::readHandler, this);
  } else {
    perror("new Socket");
  }

  sendCredentials();
  setChannel(currentChannel_);
}

Socket *Client::getSocket() const {
  return socket_;
}

void Client::sendIrc(std::string command, std::vector<std::string> args, std::string comment) {
  if (isConnected()) {
    std::ostringstream commandStream;
    commandStream << command << " ";

    if (!args.empty()) {
      for (const auto &arg : args)
        commandStream << arg << " ";
    }

    if (!comment.empty())
      commandStream << ":" << comment;

    socket_->send(commandStream.str());
  }
}

void Client::sendPong(std::string content) {
  sendIrc("PONG",  { content });
}

void Client::sendCredentials() {
  sendNick();
  sendUser();
}

void Client::sendNick() {
  sendIrc("NICK", { nickName_ });
}

void Client::sendUser() {
  sendIrc("USER", { userName_, "0", "*" }, realName_);
}

void Client::sendPrivateMessage(std::string receiver, std::string message) {
  sendIrc("PRIVMSG", { receiver }, message);
}

void Client::sendChannelMessage(std::string channel, std::string message) {
  sendIrc("PRIVMSG", { "#" + channel }, message);
}

void Client::sendJoin(std::string dist) {
  sendIrc("JOIN", { dist });
}

void Client::sendJoinChannel(std::string channel) {
  sendIrc("JOIN", { "#" + channel });
}

void Client::joinRead() {
  pthread_join(readThread_, nullptr);
}

void Client::sendCurrentChannelMessage(std::string message) {
  sendChannelMessage(currentChannel_, message);
}

void Client::shutdown() {
  sendIrc("QUIT", { }, "Goodby!");
  joinRead();
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
        std::cout << data << "%";
        std::cout.flush();
      }
    }
  }

  std::cout << "%%%\n";
  return client;
}