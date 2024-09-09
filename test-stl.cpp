#include <algorithm>
#include <cmath>
#include <iostream>

using namespace std;

int increment(int x) { return (x + 1); }

double toAngle(double scalar) {
  while (scalar > 360) {
    scalar -= 360;
  }
  while (scalar < 0) {
    scalar += 360;
  }
  return scalar;
}

// truncates decimals; returns -1 if digitIndex exceeds input
int getDigit(float input, int digitIndex) {
  const int numDigits = floor(log10(input) + 1);
  int divisor = 1;
  int digitVal = -1;

  if (digitIndex > numDigits) {
    return digitVal;
  };

  for (int i = 0; i < digitIndex - 1; i++) {
    divisor *= 10;
  };

  digitVal = static_cast<int>(abs(input)) / divisor;
  digitVal = digitVal % 10;

  return digitVal;
}

int main() {
  int array[] = {123, 14, 14, 23345, 23, 4, 54, 6, 574, 45, 544645};
  const int arrSize = sizeof(array) / sizeof(*array);

  //
  transform(array, array + arrSize, array, increment);

  float angle = 54321.34;

  const int digit = getDigit(angle, 5);

  float test = remainder(angle, 360);
  if (test < 0) {
    test += 360;
  }

  const float test2 = toAngle(angle);

  cout << "max " << arrSize << endl;
  int* i = max_element(array, array + arrSize);
  cout << int(*i) << endl;
  // for (int el : array) {
  //   cout << el << endl;
  // }
}
