#include <math.h>

#include <iomanip>
#include <iostream>

#include "timer.h"

using namespace std;

void createFactorialArr(double *&factorialArr, size_t degree) {
  factorialArr[0] = 1;
  for (size_t i = 1; i <= degree; i++) {
    factorialArr[i] = i * factorialArr[i - 1];
  }
}

double taylor(double x, size_t n) {
  Timer timer;
  if (!(n % 2)) {
    n -= 1;
  }

  bool isPositive = n % 4 == 3;

  double factorialArr[n];
  double *factorialArrPointer = factorialArr;

  // double (*factorialArrPointer)[n];

  createFactorialArr(factorialArrPointer, n);
  double result = (1 / factorialArr[n]) * x;

  for (int i = n - 1; i > 0; i -= 1) {
    if (i % 2) {
      result = isPositive ? result + (1 / factorialArr[i])
                          : result - (1 / factorialArr[i]);
      isPositive = !isPositive;
    }
    result *= x;
  }

  return result;
}

int main() {
  const double rad = -2.2;
  const double approx = taylor(rad, 100);

  cout << setprecision(10) << "approx: " << approx << endl;
  cout << setprecision(10) << "answer: " << sin(rad) << endl;

  return 0;
}
