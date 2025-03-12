#include <fstream>

#include "string.h"
using namespace String;

void quitTester(int score) {
  std::cout << "Score: " << score << std::endl;
  std::ofstream scoreFile("score.txt");
  scoreFile << score << std::endl;
  scoreFile.close();
  exit(0);
}

int main() {
  int score = 0;

  std::cout << "Calling char[] constructor" << std::endl;
  string s1("abC");

  std::cout << "Testing size function" << std::endl;
  if (s1.size() == 3) {
    std::cout << "Passed.  +1" << std::endl;
    score++;
  } else {
    std::cout << "Failed." << std::endl;
    std::cout << s1.size() << std::endl;
  }

  std::cout << "Testing [] access operator" << std::endl;
  if (s1[0] == 'a' && s1[1] == 'b' && s1[2] == 'C') {
    std::cout << "Passed.  +1" << std::endl;
    score++;
  } else {
    std::cout << "Failed." << std::endl;
    std::cout << s1[0] << ' ' << s1[1] << ' ' << s1[2] << std::endl;
  }

  std::cout << "Testing == with a C string" << std::endl;
  if (s1 == "abC" && !(s1 == "ABc") && !(s1 == "abCd") && !(s1 == "a")) {
    std::cout << "Passed.  +1" << std::endl;
    score++;
  } else {
    std::cout << "Failed." << std::endl;
    std::cout << s1 << std::endl;
    std::cout << "Cannot finish testing without c-string == operator.  Exiting."
              << std::endl;
    quitTester(score);
  }

  std::cout << "Testing [] assignment" << std::endl;
  s1[2] = 'c';
  if (s1 == "abc") {
    std::cout << "Passed.  +1" << std::endl;
    score++;
  } else {
    std::cout << "Failed." << std::endl;
    std::cout << s1 << std::endl;
  }

  std::cout << "Testing copy constructor and == with string" << std::endl;
  string s2 = s1;
  if (s2 == s1 && s2 == string("abc") && !(s2 == string("ABc")) &&
      !(s2 == string("abCd")) && !(s2 == string("a"))) {
    std::cout << "Passed.  +1" << std::endl;
    score++;
  } else {
    std::cout << "Failed." << std::endl;
    std::cout << s2 << std::endl;
    std::cout << "Cannot finish testing without string == operator.  Exiting."
              << std::endl;
    quitTester(score);
  }

  std::cout << "Testing copy assignment" << std::endl;
  string s3 = "C++ is the best!";
  s2 = s3;
  if (s3 == s2 && s2 == "C++ is the best!") {
    std::cout << "Passed.  +1" << std::endl;
    score++;
  } else {
    std::cout << "Failed." << std::endl;
    std::cout << s2 << std::endl;
  }

  std::cout << "Testing copy assignment using a C String" << std::endl;
  s2 = "C strings are different than C++ strings\n";
  if (s2 == "C strings are different than C++ strings\n") {
    std::cout << "Passed.  +1" << std::endl;
    score++;
  } else {
    std::cout << "Failed." << std::endl;
    std::cout << s2 << std::endl;
  }

  std::cout << "Testing insertion << and extraction operators >>" << std::endl;
  std::ofstream ofsInsert("testOut.txt");
  ofsInsert << "antidisestablishmentarianism";
  ofsInsert.close();
  std::ifstream ifsExtract("testOut.txt");
  string extractStr;
  ifsExtract >> extractStr;
  if (extractStr == "antidisestablishmentarianism") {
    std::cout << "Passed.  +2" << std::endl;
    score += 2;
  } else {
    std::cout << "Failed." << std::endl;
    std::cout << extractStr << std::endl;
  }

  std::cout << "Testing str + str" << std::endl;
  s3 = s1 + " " + s2;
  if (s3 == "abc C strings are different than C++ strings\n") {
    std::cout << "Passed.  +1" << std::endl;
    score++;
  } else {
    std::cout << "Failed." << std::endl;
    std::cout << s3 << std::endl;
  }

  std::cout << "Testing str + char" << std::endl;
  s3 = s3 + 'Z';
  if (s3 == "abc C strings are different than C++ strings\nZ") {
    std::cout << "Passed.  +1" << std::endl;
    score++;
  } else {
    std::cout << "Failed." << std::endl;
    std::cout << s3 << std::endl;
  }

  std::cout << "Testing >" << std::endl;
  if (string("apples") > string("apple") &&
      !(string("apple") > string("apple")) &&
      string("zebra") > string("apples") &&
      !(string("apples") > string("zebra"))) {
    std::cout << "Passed.  +1" << std::endl;
    score++;
  } else {
    std::cout << "Failed." << std::endl;
  }

  std::cout << "Testing find" << std::endl;
  string s4 = "Do all dogs chase cats?  Are all dogs good puppers?";
  if (s4.find("Do") == 0 && s4.find("dogs") == 7 && s4.find("dogs", 8) == 33 &&
      s4.find("puppers?") == s4.size() - 8) {
    std::cout << "Passed.  +2" << std::endl;
    score += 2;
  } else {
    std::cout << "Failed." << std::endl;
    std::cout << s4.find("Do") << ' ' << s4.find("dogs") << ' '
              << s4.find("dogs", 8) << ' ' << s4.find("puppers?") << std::endl;
  }

  quitTester(score);

  return 0;
}
