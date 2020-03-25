#ifndef ANSIRC_SRC_PARSER_H_
#define ANSIRC_SRC_PARSER_H_

#include "message.h"

class IrcParser {
 public:
  IrcParser();
  ~IrcParser();

  IrcMessage parseSingleMessage(const std::string &text);
 };

#endif //ANSIRC_SRC_PARSER_H_
