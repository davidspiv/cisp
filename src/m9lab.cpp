#include <time.h>

#include <iostream>
#include <string>

using String = std::string;

void display(String output, bool returnFlag = 0);
int* popTestArr(int* arr, int size);
int selectSort(int*, int);
int insertSort(int*, int);

int main() {
  const int arrSize = 20;
  int orgArr[arrSize], selectSortArr[arrSize], insertSortArr[arrSize];
  popTestArr(orgArr, arrSize);

  for (int i = 0; i < arrSize; i++) {
    selectSortArr[i] = insertSortArr[i] = orgArr[i];
  }

  const int selectSortSwaps = selectSort(selectSortArr, arrSize);
  const int insertSortSwaps = insertSort(insertSortArr, arrSize);

  for (int i = 0; i < arrSize; i++) {
    if (selectSortArr[i] != insertSortArr[i]) {
      display("Error with sorting: arrays do not match", 1);
      return 1;
    }
  }

  display(
      "Both algorithms successfully sorted an array of 20 random numbers in "
      "ascending order.",
      1);
  display("INSERT ARR swaps: " + std::to_string(insertSortSwaps), 1);
  display("SELECT ARR swaps: " + std::to_string(selectSortSwaps), 1);
}

void display(String output, bool returnFlag) {
  if (returnFlag) output += "\n";
  std::cout << output;
}

int* popTestArr(int* arr, int size) {
  srand(time(0));
  for (int i = 0; i < size; i++) {
    int randomNum = rand();
    arr[i] = randomNum;
  }
  return arr;
}

int selectSort(int* arr, int arrSize) {
  int swapCount = 0;

  for (int i = 0; i < arrSize - 1; i++) {
    int minIndex = i;
    for (int j = i + 1; j < arrSize; j++) {
      if (arr[j] < arr[minIndex]) minIndex = j;
    }

    if (minIndex != i) {
      std::swap(arr[minIndex], arr[i]);
      swapCount++;
    }
  }
  return swapCount;
}

int insertSort(int* arr, int arrSize) {
  int swapCount = 0;

  for (int i = 0; i < arrSize; i++) {
    int key = arr[i];
    int currentIndex = i - 1;

    while (currentIndex >= 0 && arr[currentIndex] > key) {
      std::swap(arr[currentIndex], arr[currentIndex + 1]);
      swapCount++;
      currentIndex -= 1;
    }
  }
  return swapCount;
}
