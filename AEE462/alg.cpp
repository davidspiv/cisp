#include <math.h>

#include <iostream>
#include <vector>

using namespace std;

// polynomial evaluation using Horner's method - avoids pow()
// pre: vector must have at least 2 elements
double evaluatePolynomial(const vector<double> &coeffs, double x) {
  double numerator;
  double denominator;

  switch (coeffs.size()) {
    case 0:
    case 1:
      throw invalid_argument(
          "Coefficient vector must have at least 2 elements.");

    case 2:
      numerator = -coeffs[1];
      denominator = coeffs[0];
      break;

    case 3:
      numerator = coeffs[0] * x * x - coeffs[2];
      denominator = 2 * coeffs[0] * x + coeffs[1];
      break;

    default:
      numerator =
          (coeffs.size() - 2) * coeffs[0] * x + (coeffs.size() - 3) * coeffs[1];
      denominator =
          (coeffs.size() - 1) * coeffs[0] * x + (coeffs.size() - 2) * coeffs[1];

      for (size_t i = 2; i < (coeffs.size() - 4); i++) {
        numerator = numerator * x + (coeffs.size() - i - 2) * coeffs[i];
        denominator = denominator * x + (coeffs.size() - i - 1) * coeffs[i];
      }

      numerator = numerator * x * x - coeffs[coeffs.size() - 1];
      denominator = denominator * x + coeffs[coeffs.size() - 2];
      break;
  }

  return numerator / denominator;
}

// Newton-Raphson Method
int main() {
  vector<double> polynomial = {8, 1, 0};
  double guess = -3;
  double delta;
  int iterationCount = 0;

  do {
    const double refinedGuess = evaluatePolynomial(polynomial, guess);
    delta = abs((refinedGuess - guess) / guess);
    guess = refinedGuess;
    iterationCount++;

    if (iterationCount > 17) {
      cout << "not converged" << endl;
      return 1;
    }

  } while (delta >= 1e-7);

  cout << "zero: " << guess << endl;
  return 0;
}
