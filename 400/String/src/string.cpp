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

string::string(const string& other) {
  len = other.len;
  if (len > 0) {
    buffer = new char[len];
    for (int i = 0; i < len; i++) {
      buffer[i] = other.buffer[i];
    }
  } else {
    buffer = nullptr;
  }
}

// int string::length() const {};
// int string::size() const {};

// char& string::operator[](int i) {};
// const char& string::operator[](int i) const {};

// string string::operator+(const string& other) const {};
// string string::operator+(const char& ch) const {};

// bool string::operator==(const string& other) const {};
// int string::find(const char* s, int pos = 0) const {};

// void string::print(std::ostream& out) const {};

// std::ostream& operator<<(std::ostream& out, const string& str) {};
// std::istream& operator>>(std::istream& in, string& str) {};
// bool operator!=(const string& s, const string& t) {};
// bool operator>(const string& lhs, const string& rhs) {};
