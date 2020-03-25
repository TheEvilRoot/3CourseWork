#ifndef ANSIRC_SRC_HANDLER_H_
#define ANSIRC_SRC_HANDLER_H_

#include "message.h"
#include "parser.h"

class IrcHandler {
 public:
  IrcHandler(IrcParser *parser);
  ~IrcHandler();

  void pushMessage(std::string rawMessage);
  void handleBaseMessage(IrcMessage &message);

 private:
  IrcParser *parser_;
};

#endif //ANSIRC_SRC_HANDLER_H_
