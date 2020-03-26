#include <iostream>

#include "client.h"
#include "socket.h"

int main() {
  Client client("192.168.100.7",
      6667,
      "UserName",
      "RealName",
      "NickName",
      "Hello");

  auto parser = new IrcParser;
  auto handler = new IrcHandler(parser);

  client.setHandler(handler);
  client.connect();

  while (client.isConnected()) {
    std::string string;
    std::getline(std::cin, string);
    std::cout << "<< " << string << std::endl;
    client.sendCurrentChannelMessage(string);
  }

  client.shutdown();
  return 0;
}
