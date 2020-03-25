#ifndef ANSIRC_SRC_BUFFER_H_
#define ANSIRC_SRC_BUFFER_H_

#include <string>
#include <vector>

class Buffer {
 public:
  Buffer() = default;

  void push(const std::string& newData);
  std::string pop();
  void reset();
  void next();

  [[nodiscard]] bool empty() const;
  [[nodiscard]] std::string getLeftData() const;
  [[nodiscard]] std::vector<std::string> getList() const;

 private:
  std::string data_;
  std::vector<std::string> list_;
};

#endif
