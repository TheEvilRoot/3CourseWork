//
// Created by Прогрессивный Долгострой on 25.03.20.
//

#include "include/socket.h"

Socket::Socket(const char *address, uint16_t port) :
    address_ {std::string(address) },
    port_ { port },
    isAlive_ { false }{
  memset(&serverAddress_, 0, sizeof(serverAddress_));

  serverAddress_.sin_family = AF_INET;
  serverAddress_.sin_addr.s_addr = inet_addr(address);
  serverAddress_.sin_port = (uint16_t) htons(port);

  sockD_ = socket(AF_INET, SOCK_STREAM, 0);
  if (sockD_ >= 0)
    connection_ = connect(sockD_, (struct sockaddr*) &serverAddress_, sizeof(serverAddress_));

  if (connection_ == 0)
    setAlive(true);
}

Socket::~Socket() {
  ::close(sockD_);
}

ssize_t Socket::send(std::string data) {
  if (data.back() != '\n')
    data += '\n';

  size_t dataSize = data.length();
  const char *dataBuffer = data.c_str();

  return handleSendErrno(::send(sockD_, dataBuffer, dataSize, 0));
}

std::string Socket::recv(size_t maxBuffer) {
  char *buffer = new char[maxBuffer + 1];
  memset(buffer, 0, maxBuffer + 1);

  ssize_t actualRecv = handleRecvErrno(::recv(sockD_, buffer, maxBuffer, 0));
  if (actualRecv > 0) {
    buffer[actualRecv] = 0;
  }

  std::string strBuffer(buffer);
  delete[] buffer;

  return strBuffer;
}

uint16_t Socket::getServerPort() const {
  return port_;
}

int Socket::getSockDesc() const {
  return sockD_;
}

std::string Socket::getServer() const {
  return address_;
}

int Socket::getConnectionStatus() const {
  return connection_;
}

void Socket::bufferRecv(size_t maxPacket) {
  auto data = recv(maxPacket);
  getBuffer().push(data);
}

Buffer &Socket::getBuffer()  {
  return recvBuffer_;
}

void Socket::setAlive(bool alive) {
  isAlive_ = alive;
}

bool Socket::isAlive() const {
  return isAlive_;
}

ssize_t Socket::handleSendErrno(ssize_t err) {
  if (err < 0) {
    perror("send -1 bytes");
    setAlive(false);
  } else if (err == 0) {
    fprintf(stderr, "send 0 bytes\n");
  }
  return err;
}

ssize_t Socket::handleRecvErrno(ssize_t err) {
  if (err < 0) {
    perror("recv -1 bytes");
    setAlive(false);
  } else if (err == 0) {
    fprintf(stderr, "recv stream closed\n");
    setAlive(false);
  }

  return err;
}