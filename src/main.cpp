#include <iostream>

#include <unistd.h>
#include <csignal>
#include <sys/wait.h>

#include "client.h"
#include "socket.h"

pthread_mutex_t serverReadyMutex;
pthread_mutex_t bufferUpdatedMutex;

pthread_mutex_t cmdBufferMutex;
std::string buffer;

void writeBuffer(const std::string &value) {
  pthread_mutex_lock(&cmdBufferMutex);
  buffer.clear();
  buffer.append(value);
  pthread_mutex_unlock(&cmdBufferMutex);
  pthread_mutex_unlock(&bufferUpdatedMutex);
}

void getBuffer(const std::function<void(const std::string&)>& handler) {
  pthread_mutex_lock(&cmdBufferMutex);
  handler(buffer);
  pthread_mutex_unlock(&cmdBufferMutex);
}

void createMutex(pthread_mutex_t &mutex, bool initialValue = false) {
  pthread_mutex_init(&mutex, nullptr);
  if (initialValue)
    pthread_mutex_lock(&mutex);
}

void *startServer(void *) {
  std::cout << "Starting server..." << std::endl;
  std::cout <<  "Server PID " << getpid() << std::endl;

  std::cout << "Waiting to client be ready..." << std::endl;

  Client client("127.0.0.1",
      9997,
      "UserName",
      "RealName",
      "NickName",
      "Hello");

  auto parser = new IrcParser;
  auto handler = new IrcHandler(parser);

  client.setHandler(handler);
  if (!client.connect()) {
    std::cout << "Failed to connect!" << std::endl;
    return nullptr;
  }

  std::cout << "Connected!" << std::endl;
  pthread_mutex_unlock(&serverReadyMutex);

  while (true) {
    pthread_mutex_lock(&bufferUpdatedMutex);
    getBuffer([&](const auto &input) {
      client.sendCurrentChannelMessage(input);
    });
  }

  client.shutdown();
  std::cout << "Server closed" << std::endl;
  return nullptr;
}

void *startClient(void *) {
  pthread_mutex_lock(&serverReadyMutex);
  while (true) {
    std::cout << "> ";
    std::string input;
    std::getline(std::cin, input);
    writeBuffer(input);
  }
  return nullptr;
}

int main(const int argc, const char *argv[]) {
  createMutex(serverReadyMutex, true);
  createMutex(bufferUpdatedMutex, true);
  createMutex(cmdBufferMutex);

  pthread_t clientId;

  pthread_create(&clientId, nullptr, startClient, nullptr);
  startServer(nullptr);

  return 0;
}
