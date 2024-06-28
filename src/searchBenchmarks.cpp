#include <algorithm>
#include <iostream>
#include <string>

void display(std::string, bool = false);
int* popTestArr(int*, int);
int sequentialSearch(int*, int);
int binarySearch(int*, int);
std::string buildArrStr(int[], int);

const int target = 999999999;

int main() {
  const int arrSize = 100000;
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
  arr[size / 2] = target;
  return arr;
}

int sequentialSearch(int* arr, int size) {
  int count = 0;
  for (int i = 0; i < size; i++) {
    count++;
    if (arr[i] == target) return count;
  };
  return -1;
}

int binarySearch(int* arr, int size) {

  int count = 0;
  int low = 0;
  int high = size - 1;

  {
    while (low <= high) {
      const int mid = low + (high - low) / 2;
      count++;
      if (arr[mid] == target) return count;
      if (arr[mid] < target) {
        low = mid + 1;
      }

      else {
        high = mid - 1;
      }
    }
    return -1;
  }
}

// std::string buildArrStr(int arr[], int size) {
//   std::string resultString;

//   for (int i = 0; i < size; i++) {
//     resultString += " " + std::to_string(arr[i]) + ",\n";
//   }

//   return resultString;
// }
