#include <algorithm>
#include <iostream>
#include <string>

void display(std::string, bool = false);
int* popTestArr(int*, int);
int sequentialSearch(int*, int);
int binarySearch(int*, int);
// std::string buildArrStr(int[], int);

int main() {
  const int arrSize = 100;
  int testArr[arrSize];
  popTestArr(testArr, arrSize);
  display("SEQ COUNT: " + std::to_string(sequentialSearch(testArr, arrSize)),
          1);
  std::sort(testArr, testArr + arrSize);
  display("BIN COUNT: " + std::to_string(binarySearch(testArr, arrSize)), 1);
  // display(buildArrStr(testArr, arrSize), 1);
}

void display(std::string output, bool returnFlag) {
  if (returnFlag) output += "\n";
  std::cout << output;
}

int* popTestArr(int* arr, int size) {
  for (int i = 0; i < size; i++) {
    int randomNum = rand();
    arr[i] = randomNum;
  }
  arr[size / 2] = 69;
  return arr;
}

int sequentialSearch(int* arr, int size) {
  int count = 0;
  for (int i = 0; i < size; i++) {
    count++;
    if (arr[i] == 69) return count;
  };
  return 0;
}

int binarySearch(int* arr, int size) {
  int count = 0;
  int middleIndex = size / 2;
  while (middleIndex && middleIndex < size) {
    count++;
    middleIndex = arr[middleIndex] < 69 ? middleIndex + (middleIndex / 2)
                                        : middleIndex / 2;
    if (arr[middleIndex] == 69) return count;
  };
  return 0;
}

// std::string buildArrStr(int arr[], int size) {
//   std::string resultString;

//   for (int i = 0; i < size; i++) {
//     resultString += " " + std::to_string(arr[i]) + ",\n";
//   }

//   return resultString;
// }
