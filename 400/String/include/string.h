#ifndef STRING_H
#define STRING_H
#include <iostream>

// using std::cout;
// using std::endl;
// using std::ifstream;
// using std::istream;
// using std::ofstream;
// using std::ostream;
// using std::streambuf;

namespace String {
class string {
 public:
  string();
  string(const char s[]);

  // big 3
  string(const string& other);
  string& operator=(const string& other);
  ~string();

  int length() const;
  int size() const;

  char& operator[](int i);
  const char& operator[](int i) const;

  string operator+(const string& other) const;
  string operator+(const char& ch) const;

  bool operator==(const string& other) const;
  int find(const char* s, int pos = 0) const;

  void print(std::ostream& out) const;

 private:
  int len;
  char* buffer;
};

std::ostream& operator<<(std::ostream& out, const string& str);
std::istream& operator>>(std::istream& in, string& str);
bool operator!=(const string& s, const string& t);
bool operator>(const string& lhs, const string& rhs);

}  // namespace String
#endif
