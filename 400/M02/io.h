#ifndef IO_H
#define IO_H

#include <iomanip>
#include <iostream>
#include <string>

template <typename T>
void print(T output, const std::string &label = "") {
  if (label.empty()) {
    std::cout << output << std::endl;
    return;
  }
  std::cout << label << ": " << output << std::endl;
}

template <typename T>
T getInput(const std::string &prompt) {
  T input;
  std::cout << prompt;
  std::cin >> input;

  while (!std::cin || std::cin.peek() != '\n') {
    std::cin.clear();
    std::cin.ignore(100, '\n');
    std::cout << "error: invalid input, try again" << std::endl << prompt;
    std::cin >> input;
  }

  return input;
}

#endif
