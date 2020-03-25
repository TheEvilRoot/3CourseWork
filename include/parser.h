#ifndef ANSIRC_SRC_PARSER_H_
#define ANSIRC_SRC_PARSER_H_

#include <vector>
#include <string>
#include <experimental/optional>

#include "message.h"

class IrcParser {
 public:
  IrcParser();
  ~IrcParser();

  IrcMessage parseSingleMessage(const std::string &text);

  static std::vector<std::string> splitString(const std::string& source, char sep, int max = -1);

protected:
  std::pair<std::string, std::experimental::optional<std::string>> getToken(const std::string &string);
 };

#endif //ANSIRC_SRC_PARSER_H_
