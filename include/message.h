#ifndef ANSIRC_SRC_MESSAGE_H_
#define ANSIRC_SRC_MESSAGE_H_

#include <string>
#include <unordered_map>
#include <vector>
#include <utility>
#include <ostream>

class IrcMessage {
 public:
  explicit IrcMessage(std::string raw):
  raw_{std::move( raw )} { }

  [[nodiscard]] std::string getRaw() const {
    return raw_;
  }

  [[nodiscard]] std::string getSource() const {
    return source_;
  }

  [[nodiscard]] std::vector<std::string> getParams() const {
    return params_;
  }

  [[nodiscard]] std::string getTrailing() const {
    return trailing_;
  }

  [[nodiscard]] std::string getCommand() const {
    return command_;
  }

  void setTrailing(const std::string &trailing) {
    trailing_ = trailing;
  }

  void setCommand(const std::string &command) {
    command_ = command;
  }

  void addParam(const std::string &param) {
    params_.push_back(param);
  }

  void setSource(const std::string& source) {
    source_ = source;
  }

  friend std::ostream& operator<<(std::ostream &out, const IrcMessage &msg) {
    out << "Source: " << msg.source_ << "\n";
    out << "Command: " << msg.command_ << "\n";
    out << "Params: \n";
    for (const auto & param : msg.params_)
      out << "\t" << param << "\n";
    out << "Trailing: " << msg.trailing_ << "\n";
    return out;
  }

 private:
  std::string raw_;
  std::string source_;
  std::vector<std::string> params_;
  std::string command_;
  std::string trailing_;
};

#endif //ANSIRC_SRC_MESSAGE_H_
