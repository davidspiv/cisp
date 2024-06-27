#include <iostream>
#include <string>

void display(std::string, bool = false);
int* popTestArr(int*, int);
std::string buildArrStr(int[], int);

int main() {
  int testArr[10];
  popTestArr(testArr, 10);
  display(buildArrStr(testArr, 10), 1);
}

void display(std::string output, bool returnFlag) {
  if (returnFlag) output += "\n";
  std::cout << output;
}

int* popTestArr(int* arr, int size) {
  for (int i = 0; i < 10; i++) {
    int randomNum = rand();
    arr[i] = randomNum;
  }
  return arr;
}

std::string buildArrStr(int arr[], int size) {
  std::string resultString;

  for (int i = 0; i < size; i++) {
    resultString += " " + std::to_string(arr[i]) + ",\n";
  }

  return resultString;
}
