#include "include/parser.h"

IrcParser::IrcParser() = default;
IrcParser::~IrcParser() = default;

IrcMessage IrcParser::parseSingleMessage(const std::string &text) {
  IrcMessage message(text);
  std::string string = text;

  if (string[0] == ':') {
    auto parts = splitString(string.substr(1), ' ', 1);
    message.setSource(parts[0]);
    string = parts[1];
  }

  auto tokens = getToken(string);
  message.setCommand(tokens.first);

  while (tokens.second) {
    string = tokens.second.value_or("");
    if (string[0] == ':') {
      message.setTrailing(string.substr(1));
      break;
    } else {
      tokens = getToken(string);
      message.addParam(tokens.first);
    }
  }

  return message;
}



std::vector<std::string> IrcParser::splitString(const std::string &source,
                                                char sep, int max) {
  std::vector<std::string> ret;
  std::string::size_type pos = 0, prevPos = 0;

  while ((pos = source.find(sep, pos)) != std::string::npos) {
    std::string substring(source.substr(prevPos, pos - prevPos));
    ret.push_back(substring);
    prevPos = ++pos;
    if (max > 0 && max == ret.size()) {
      pos = std::string::npos;
      break;
    }
  }

  ret.push_back(source.substr(prevPos, pos - prevPos));
  return ret;
}

std::pair<std::string, std::experimental::optional<std::string>> IrcParser::getToken(const std::string &string) {
  auto split = splitString(string, ' ', 1);
  if (split.size() == 1)
    return std::make_pair(split[0], std::experimental::optional<std::string>());
  return std::make_pair(split[0], std::experimental::make_optional(split[1]));
}

