#ifndef ANSIRC_SRC_SOCKET_H_
#define ANSIRC_SRC_SOCKET_H_

#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "include/buffer.h"

class Socket {
 public:
  Socket(const char *address, uint16_t port);
  ~Socket();

  ssize_t send(std::string data);
  std::string recv(size_t maxBuffer);
  void bufferRecv(size_t maxPacket);

  [[nodiscard]] int getConnectionStatus() const;
  [[nodiscard]] int getSockDesc() const;
  [[nodiscard]] std::string getServer() const;
  [[nodiscard]] uint16_t getServerPort() const;
  [[nodiscard]] Buffer& getBuffer();

  void setAlive(bool alive);
  [[nodiscard]] bool isAlive() const;

 private:
  ssize_t handleSendErrno(ssize_t err);
  ssize_t handleRecvErrno(ssize_t err);

  std::string address_;
  uint16_t port_;
  struct sockaddr_in serverAddress_ { };
  int sockD_;
  int connection_;
  Buffer recvBuffer_;
  bool isAlive_;
};

#endif
