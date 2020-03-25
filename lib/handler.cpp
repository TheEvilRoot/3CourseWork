//
// Created by Прогрессивный Долгострой on 25.03.20.
//

#include "include/handler.h"

#include <iostream>

IrcHandler::IrcHandler(IrcParser *parser): parser_ { parser } { }
IrcHandler::~IrcHandler() = default;

void IrcHandler::handleBaseMessage(IrcMessage &message) {
  std::cout << message << "\n";
}

void IrcHandler::pushMessage(std::string rawMessage) {
  auto message = parser_->parseSingleMessage(rawMessage);
  handleBaseMessage(message);
}
