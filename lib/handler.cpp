#include "include/handler.h"

#include <iostream>

IrcHandler::IrcHandler(IrcParser *parser): parser_ { parser } { }
IrcHandler::~IrcHandler() = default;

void IrcHandler::handleBaseMessage(IrcMessage &message) {
  for (auto listener : listeners_) {
    if (listener->shouldHandle(message.getCommand())) {
      if (listener->onBaseMessage(message)) {
        break;
      }
    }
  }
}

void IrcHandler::pushMessage(const std::string& rawMessage) {
  auto message = parser_->parseSingleMessage(rawMessage);
  handleBaseMessage(message);
}

void IrcHandler::addListener(MessageListener *listener) {
  listeners_.push_back(listener);
}

void IrcHandler::removeListener(MessageListener *listener) {
  auto pos = std::find(listeners_.begin(), listeners_.end(), listener);
  if (pos != listeners_.end()) {
    listeners_.erase(pos);
  }
}
