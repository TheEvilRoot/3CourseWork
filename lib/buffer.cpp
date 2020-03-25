#include "include/buffer.h"

void Buffer::push(const std::string &newData) {
  for (auto c : newData) {
    data_ += c;
    if ((c == '\n' && !data_.empty()) || data_.length() >= 512) {
      next();
    }
  }
}

std::string Buffer::pop() {
  std::string top = list_.back();
  list_.pop_back();
  return top;
}

void Buffer::next() {
  list_.push_back(data_);
  data_.clear();
}

void Buffer::reset() {
  data_.clear();
  list_.clear();
}

bool Buffer::empty() const {
  return list_.empty();
}

std::string Buffer::getLeftData() const {
  return data_;
}

std::vector<std::string> Buffer::getList() const {
  return list_;
}
