//
// Created by Прогрессивный Долгострой on 25.03.20.
//

#include "parser.h"

IrcParser::IrcParser() = default;
IrcParser::~IrcParser() = default;

IrcMessage IrcParser::parseSingleMessage(const std::string &text) {
  return IrcMessage(text);
}

