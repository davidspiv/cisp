#include <iostream>

using namespace std;

// int countEvens(const int *arr, size_t arrSize) {
//   int count = 0;
//   for (size_t i = 0; i < arrSize; i += 1) {
//     if (!(arr[i] % 2)) {
//       count += 1;
//     }
//   }

//   return count;
// }

// int centeredAverage(const int* testArr, const size_t arrSize) {
//   const int INT_MAX = 2147483647;

//   int arrLargest = 0;
//   int arrSmallest = INT_MAX;
//   int sum = 0;

//   for (size_t i = 0; i < arrSize; i += 1) {
//     if (testArr[i] > arrLargest) {
//       arrLargest = testArr[i];
//     }

//     if (testArr[i] < arrSmallest) {
//       arrSmallest = testArr[i];
//     }

//     sum += testArr[i];
//   }

//   sum -= arrLargest + arrSmallest;

//   return sum / (arrSize - 2);
// }

// void shiftLeft(int*& testArr, const size_t arrSize) {
//   const int firstElement = testArr[0];
//   for (size_t i = 0; i < arrSize - 1; i += 1) {
//     testArr[i] = testArr[i + 1];
//   }
//   testArr[arrSize - 1] = firstElement;

//   for (size_t i = 0; i < arrSize; i++) {
//     cout << testArr[i] << endl;
//   }
// }

// array is currently one element larger than it needs to be
//  int maxSpan(const int*& testArr, const size_t arrSize) {
//    int firstDigits[10];
//    int spans[10] = {};
//    int maxSpan = 0;

//   for (size_t i = 0; i < 10; i++) {
//     firstDigits[i] = -1;
//   }

//   // forwards
//   for (size_t i = 0; i < arrSize; i++) {
//     const int val = testArr[i];
//     if (firstDigits[val] < 0) {
//       firstDigits[val] = i;
//     }
//   }

//   // backwards
//   for (int i = arrSize - 1; i >= 0; i--) {
//     const int val = testArr[i];
//     if (!spans[val]) {
//       spans[val] = i - firstDigits[val];
//     }
//   }

//   // filter largest
//   for (size_t i = 0; i < 10; i++) {
//     if (spans[i] > maxSpan) {
//       maxSpan = spans[i];
//     }
//   }

//   return maxSpan + 1;
// }

int main() {
  int arr[] = {9, 9, 9};
  const int* arrPointer = arr;
  int arrSize = sizeof(arr) / sizeof(*arr);

  

  return 0;
}
