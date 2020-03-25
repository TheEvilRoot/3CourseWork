#ifndef ANSIRC_SRC_MESSAGE_H_
#define ANSIRC_SRC_MESSAGE_H_

#include <string>
#include <utility>

class IrcMessage {
 public:
  explicit IrcMessage(std::string raw):
  raw_{std::move( raw )} { }

  [[nodiscard]] std::string getRaw() const {
    return raw_;
  }

 private:
  std::string raw_;
};

#endif //ANSIRC_SRC_MESSAGE_H_
