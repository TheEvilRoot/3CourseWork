#include <iostream>

#include "socket.h"
#include "client.h"

int main() {
  Client client("192.168.100.7",
      6667,
      "ANCIrC",
      "ANCIrC",
      "ANCIrC",
      "Hello");
  client.connect();
  client.sendCurrentChannelMessage("Hello from ANCIrC!");
  client.shutdown();
  return 0;
}