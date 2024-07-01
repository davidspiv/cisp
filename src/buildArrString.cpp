#include <iostream>
#include <string>

std::string buildArrStr(int* arr, int size) {
  std::string resultString;

  for (int i = 0; i < size; i++) {
    resultString += " " + std::to_string(arr[i]) + ",\n";
  }

  return resultString;
}
