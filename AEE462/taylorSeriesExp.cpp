#include <math.h>

#include <iomanip>
#include <iostream>

using namespace std;

long double taylor(long double x, size_t n) {
  long double sum = 0;
  long int factorial = 1;
  bool isAdd = true;
  for (size_t i = 1; i <= n; i += 1) {
    factorial *= i;

    if (i % 2) {
      const long double term = ((pow(x, i)) / factorial);
      if (isAdd) {
        sum += term;
        cout << i << endl;
      } else {
        sum -= term;
      }
      isAdd = !isAdd;
    }
  }

  return sum;
}

int main() {
  const long double rad = .11;
  const long double approx = taylor(rad, 5);

  cout << setprecision(10) << "approx: " << approx << endl;
  cout << setprecision(10) << "answer: " << sin(rad) << endl;

  return 0;
}
