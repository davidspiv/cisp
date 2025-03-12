#include "../include/string.h"

#include <cstring>

using namespace String;

string::string() {
  len = 0;
  buffer = nullptr;
}

string::string(const char s[]) {
  len = std::strlen(s);
  if (len > 0) {
    buffer = new char[len];
    for (int i = 0; i < len; i++) {
      buffer[i] = s[i];
    }
  } else {
    buffer = nullptr;
  }
}

string::~string() { delete[] buffer; }

string& string::operator=(const string& other) {
  if (this == &other) return *this;  // check for self-assignment

  delete[] buffer;
  len = other.len;

  if (len > 0) {
    buffer = new char[len];
    for (int i = 0; i < len; i++) {
      buffer[i] = other.buffer[i];
    }
  } else {
    buffer = nullptr;
  }

  return *this;
}

int string::length() const { return len; };

int string::size() const { return len; };

char& string::operator[](int i) { return buffer[i]; };

const char& string::operator[](int i) const { return buffer[i]; };

string string::operator+(const string& other) const {
  string result;
  result.len = len + other.len;
  result.buffer = new char[result.len];
  for (int i = 0; i < len; i++) {
    result.buffer[i] = buffer[i];
  }
  for (int i = 0; i < other.len; i++) {
    result.buffer[len + i] = other.buffer[i];
  }
  return result;
}

string string::operator+(const char& ch) const {
  string result;
  result.len = len + 1;
  for (int i = 0; i < len; i++) {
    result.buffer[i] = buffer[i];
  }
  result.buffer[len] = ch;
  return result;
};

bool string::operator==(const string& other) const {
  if (len != other.len) {
    return false;
  }
  for (int i = 0; i < len; i++) {
    if (buffer[i] != other.buffer[i]) {
      return false;
    }
  }
  return true;
}

int string::find(const char* s, int pos) const {
  if (pos >= len) return -1;
  for (int i = pos; i < len; i++) {
    if (buffer[i] != s[0]) continue;
    // char ch = s[0];
    // size_t j = 0;

    // while(ch != '\0'){
    // 	j++;
    // 	ch = s[j];
    // }
    return i;
  }
  return -1;
};

void string::print(std::ostream& out) const {
  for (int i = 0; i < len; i++) {
    out << buffer[i];
  }
}

std::ostream& String::operator<<(std::ostream& out, const string& str) {
  str.print(out);
  return out;
}

std::istream& String::operator>>(std::istream& in, string& str) { return in; };

bool String::operator!=(const string& s, const string& t) { return !(s == t); };

bool String::operator>(const string& lhs, const string& rhs) { return true; };
