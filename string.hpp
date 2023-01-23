#pragma once
#include <algorithm>
#include <cctype>
#include <cstring>
#include <iostream>

class String {
 public:
  String() = default;
  String(const char* str);
  String(size_t n, char symbol);
  String(const String& str) : String(str.data()) {}

  static void swap(String& str1, String& str2);
  String& operator=(const String& str);

  char* data() { return str_; }
  char* begin() { return data(); }
  char* end() { return begin() + length(); }
  const char* data() const { return str_; }
  const char* cbegin() const { return data(); }
  const char* cend() const { return cbegin() + length(); }

  char& front() { return *begin(); }
  char& back() { return *(end() - 1); }
  const char& front() const { return *cbegin(); }
  const char& back() const { return *(cend() - 1); }

  char& operator[](size_t index) { return *(begin() + index); }
  const char& operator[](size_t index) const { return *(cbegin() + index); }

  void push_back(char symbol);
  void pop_back() { str_[--len_] = '\0'; }

  String& operator+=(const char* c_str);
  String& operator+=(const String& str);
  String& operator+=(char symbol);

  [[nodiscard]] size_t length() const { return len_; }
  [[nodiscard]] size_t size() const { return length(); }
  [[nodiscard]] size_t capacity() const { return cap_; }
  [[nodiscard]] bool empty() const { return length() == 0; }
  void clear() { str_[len_ = 0] = '\0'; }
  void shrink_to_fit();

  size_t find(const char* substr) const;
  size_t find(const String& substr) const { return find(substr.data()); }
  size_t rfind(const char* substr) const;
  size_t rfind(const String& substr) const { return rfind(substr.data()); }
  String substr(size_t start, size_t n) const;

  ~String() { delete[] str_; }

 private:
  size_t len_{0};
  size_t cap_{len_};
  char* str_{new char[cap_ + 1]};

  void realloc(size_t new_capacity, bool copy = true);
  bool rcmp(const char* substr, size_t sub_len, size_t ind) const;
};

String::String(const char* str)
    : len_(std::strlen(str)), cap_(size()), str_(new char[cap_ + 1]) {
  std::copy(str, str + length(), data());
  str_[size()] = '\0';
}
String::String(size_t n, char symbol)
    : len_(n), cap_(size()), str_(new char[cap_ + 1]) {
  std::fill(begin(), end(), symbol);
  str_[size()] = '\0';
}

void String::swap(String& str1, String& str2) {
  std::swap(str1.len_, str2.len_);
  std::swap(str1.cap_, str2.cap_);
  std::swap(str1.str_, str2.str_);
}
String& String::operator=(const String& str) {
  String tmp(str);
  swap(*this, tmp);
  return *this;
}

std::istream& operator>>(std::istream& str_in, String& str) {
  str.clear();
  char symbol;
  str_in.get(symbol);
  while (!str_in.eof() && std::isspace(symbol) != 0) {
    str_in.get(symbol);
  }
  while (!str_in.eof() && std::isspace(symbol) == 0) {
    str.push_back(symbol);
    str_in.get(symbol);
  }
  return str_in;
}
std::ostream& operator<<(std::ostream& str_out, const String& str) {
  return str_out << str.data();
}

bool operator==(const String& str_1, const String& str_2) {
  return str_1.length() == str_2.length() &&
      std::strcmp(str_1.data(), str_2.data()) == 0;
}
bool operator!=(const String& str_1, const String& str_2) {
  return !(str_1 == str_2);
}
bool operator<(const String& str_1, const String& str_2) {
  return std::strcmp(str_1.data(), str_2.data()) < 0;
}
bool operator>(const String& str_1, const String& str_2) {
  return str_2 < str_1;
}
bool operator<=(const String& str_1, const String& str_2) {
  return !(str_1 > str_2);
}
bool operator>=(const String& str_1, const String& str_2) {
  return !(str_1 < str_2);
}

void String::push_back(char symbol) {
  if (length() + 1 > capacity()) {
    realloc(capacity() == 0 ? 1 : 2 * capacity(), capacity() != 0);
  }
  str_[length()] = symbol;
  str_[++len_] = '\0';
}

String& String::operator+=(const char* c_str) {
  size_t c_len = std::strlen(c_str);
  if (length() + c_len > capacity()) {
    realloc(length() + c_len);
  }
  std::copy(c_str, c_str + c_len, end());
  len_ += c_len;
  return *this;
}
String& String::operator+=(const String& str) {
  *this += str.data();
  return *this;
}
String& String::operator+=(char symbol) {
  push_back(symbol);
  return *this;
}

String operator+(const String& str, char symbol) {
  String res_str = str;
  res_str.push_back(symbol);
  return res_str;
}
String operator+(char symbol, const String& str) {
  String res_str = String(1, symbol);
  res_str += str;
  return res_str;
}
String operator+(const String& str1, const String& str2) {
  String res_str = str1;
  res_str += str2;
  return res_str;
}

void String::shrink_to_fit() {
  if (size() < capacity()) {
    realloc(size());
  }
}

size_t String::find(const char* substr) const {
  const char* found = std::strstr(data(), substr);
  found = found == nullptr ? cend() : found;
  return std::distance(cbegin(), found);
}
size_t String::rfind(const char* substr) const {
  size_t sub_len = std::strlen(substr);
  for (size_t i = length() - 1; i >= sub_len; --i) {
    if (rcmp(substr, sub_len, i)) {
      return i - sub_len + 1;
    }
  }
  return length();
}

String String::substr(size_t start, size_t n) const {
  String substring(n, '\0');
  std::strncpy(substring.begin(), cbegin() + start, n);
  return substring;
}

bool String::rcmp(const char* str, size_t len, size_t ind) const {
  size_t cur_len = 0;
  while (cur_len < len && str_[ind] == str[len - cur_len - 1]) {
    ++cur_len;
    if (ind-- == 0) {
      break;
    }
  }
  return len == cur_len;
}

void String::realloc(size_t new_capacity, bool copy) {
  if (new_capacity != cap_) {
    cap_ = new_capacity;
    char* new_str = new char[capacity() + 1];
    if (copy) {
      std::copy(begin(), end(), new_str);
    }
    delete[] str_;
    str_ = new_str;
    str_[capacity()] = '\0';
  }
}
