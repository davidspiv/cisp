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
  double factorialCache[n + 1];
  double *factorialCachePointer = factorialCache;

  createFactorialCache(factorialCachePointer, n);

  double result = 0;
  bool isPositive = n % 4 == 3;

  for (int i = n; i > 0; i -= 1) {
    const double coefficient = (1 / factorialCache[i]);
    if (i % 2) {
      if (isPositive) {
        result += coefficient;
      } else {
        result -= coefficient;
      }
      isPositive = !isPositive;
    }
    result *= x;
  }

  return result;
}

int main() {
  const double rad = 3.1;
  const double approx = taylorSeriesSine(rad, 20);

  cout << setprecision(10) << "approx: " << approx << endl;
  cout << setprecision(10) << "answer: " << sin(rad) << endl;

  return 0;
}
