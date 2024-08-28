#include <time.h>

#include <fstream>
#include <iostream>
#include <string>

int main() {
  std::ofstream outputStream("numbers.dat");
  std::ifstream inputStream("numbers.dat");
  std::string fileLine;
  int fileSum = 0;

  srand(time(0));

  for (int i = 0; i < 1000; i++) {
    outputStream << rand() % 1000 << std::endl;
  }

  if (!inputStream.is_open()) {
    std::cerr << "Error opening the file!" << std::endl;
    return 1;
  }

  while (std::getline(inputStream, fileLine)) {
    fileSum += stoi(fileLine);
  }

  // Close the file
  inputStream.close();
  std::cout << fileSum << std::endl;
}
