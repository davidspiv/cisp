#include <math.h>

#include <iomanip>
#include <iostream>

using namespace std;

void createFactorialCache(double *&factorialCache, size_t iterations) {
  factorialCache[0] = 1;
  for (size_t i = 1; i <= iterations; i++) {
    factorialCache[i] = i * factorialCache[i - 1];
  }
}

double taylorSeriesSine(double x, size_t n) {
  double result = 0;
  bool isAddOperation = n % 4 == 3;
  double factorialCache[n + 1];
  double *factorialCachePointer = factorialCache;

  // because we need the largest factorial for the first term we cache all
  // factorial results at the beginning of the program
  createFactorialCache(factorialCachePointer, n);

  // Horner's method avoids pow() but requires us to evaluate the polynomial
  // from the highest degree first
  for (int i = n; i > 0; i -= 1) {
    const double coefficient = (1 / factorialCache[i]);

    if (i % 2) {
      result += isAddOperation ? -coefficient : coefficient;
      isAddOperation = !isAddOperation;
    }

    result *= x;
  }

  return result;
}

int main() {
  const double rad = -2.2;
  const double approx = taylorSeriesSine(rad, 30);

  cout << setprecision(10) << "approx: " << approx << endl;
  cout << setprecision(10) << "answer: " << sin(rad) << endl;

  return 0;
}
