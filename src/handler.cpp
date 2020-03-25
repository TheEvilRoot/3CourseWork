//
// Created by Прогрессивный Долгострой on 25.03.20.
//

#include "handler.h"

#include <iostream>

IrcHandler::IrcHandler(IrcParser *parser): parser_ { parser } { }
IrcHandler::~IrcHandler() = default;

void IrcHandler::handleBaseMessage(IrcMessage &message) {
  std::cout << message.getRaw();
}

void IrcHandler::pushMessage(std::string rawMessage) {
  auto message = parser_->parseSingleMessage(rawMessage);
  handleBaseMessage(message);
}
