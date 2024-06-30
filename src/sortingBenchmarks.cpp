#include <iostream>
#include <string>

void display(std::string output, bool returnFlag);
int* popTestArr(int* arr, int size);
std::string buildArrStr(int* arr, int size);
int bubbleSort(int*, int);
int selectSort(int*, int);
int insertSort(int*, int);

int main() {
  const int arrSize = 10;
  int orgArr[arrSize], bubbleSortArr[arrSize], selectSortArr[arrSize],
      insertSortArr[arrSize];
  popTestArr(orgArr, arrSize);
  for (int i = 0; i < arrSize; i++) {
    bubbleSortArr[i] = selectSortArr[i] = insertSortArr[i] = orgArr[i];
  }
  const int bubbleSortSwaps = bubbleSort(bubbleSortArr, arrSize);
  const int selectSortSwaps = selectSort(selectSortArr, arrSize);
  const int insertSortSwaps = insertSort(insertSortArr, arrSize);

  display("ORIG ARR:\n" + buildArrStr(orgArr, arrSize), 1);
  display("BUBBLE ARR SWAPS[" + std::to_string(bubbleSortSwaps) + "]:\n" +
              buildArrStr(bubbleSortArr, arrSize),
          1);
  display("SELECT ARR SWAPS[" + std::to_string(selectSortSwaps) + "]:\n" +
              buildArrStr(selectSortArr, arrSize),
          1);
  display("INSERT ARR SWAPS[" + std::to_string(insertSortSwaps) + "]:\n" +
              buildArrStr(insertSortArr, arrSize),
          1);
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
  return arr;
}

int bubbleSort(int* arr, int arrSize) {
  int count = 0;
  for (int i = 0; i < arrSize - 1; i++) {
    for (int j = 0; j < arrSize - i - 1; j++) {
      if (arr[j] > arr[j + 1]) {
        std::swap(arr[j], arr[j + 1]);
        count++;
      }
    }
  }
  return count;
}

int selectSort(int* arr, int arrSize) {
  int count = 0;
  for (int i = 0; i < arrSize - 1; i++) {
    int minIndex = i;
    for (int j = i + 1; j < arrSize; j++) {
      if (arr[j] < arr[minIndex]) minIndex = j;
    }

    if (minIndex != i) {
      std::swap(arr[minIndex], arr[i]);
      count++;
    }
  }
  return count;
}

int insertSort(int* arr, int arrSize) {
  int count = 0;
  for (int i = 0; i < arrSize; i++) {
    int key = arr[i];
    int currentEl = i - 1;
    while (currentEl >= 0 && arr[currentEl] > key) {
      arr[currentEl + 1] = arr[currentEl];
      currentEl = currentEl - 1;
    }
    arr[currentEl + 1] = key;
    count++;
  }
  return count;
}

std::string buildArrStr(int* arr, int size) {
  std::string resultString;

  for (int i = 0; i < size; i++) {
    resultString += " " + std::to_string(arr[i]) + ",\n";
  }

  return resultString;
}
