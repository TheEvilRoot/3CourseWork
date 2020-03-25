#ifndef ANSIRC_SRC_HANDLER_H_
#define ANSIRC_SRC_HANDLER_H_

#include "listener.h"
#include "message.h"
#include "parser.h"

class IrcHandler {
 public:
  explicit IrcHandler(IrcParser *parser);
  ~IrcHandler();

  void addListener(MessageListener *listener);
  void removeListener(MessageListener *listener);

  void pushMessage(const std::string& rawMessage);
  void handleBaseMessage(IrcMessage &message);

 private:
  IrcParser *parser_;
  std::vector<MessageListener *> listeners_;
};

#endif
