#include <iostream>
#include <string>

void display(std::string, bool = false);
std::string buildArrStr(int[]);

int main() {
  int arr[20] = {};
  for (int i = 0; i < 20; i++) {
    int randomNum = rand() % 10 + 1;
    arr[i] = randomNum;
  }

  display(buildArrStr(arr), 1);
}

void display(std::string output, bool returnFlag) {
  if (returnFlag) output += "\n";
  std::cout << output;
}

std::string buildArrStr(int arr[]) {
  std::string resultString;
  for (int i = 0; i < 20; i++) {
    resultString += " " + std::to_string(arr[i]) + ",";
  }

  return resultString;
}
