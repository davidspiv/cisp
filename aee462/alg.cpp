#include <math.h>

#include <iostream>
#include <vector>

using namespace std;

double calcResult(const vector<double> &a, double x) {
  double numerator = (a.size() - 2.0) * a[0] * x + (a.size() - 3.0) * a[1];
  double denominator = (a.size() - 1.0) * a[0] * x + (a.size() - 2.0) * a[1];

  for (size_t i = 2; i < (a.size() - 1); i++) {
    if (i != (a.size() - 2)) {
      numerator = numerator * x + (a.size() - i - 2.0) * a[i];
    }

    denominator = denominator * x + (a.size() - i - 1.0) * a[i];
  }

  numerator = numerator * pow(x, 2) - a[a.size() - 1];

  return numerator / denominator;
}

int main() {
  vector<double> a = {5, 2, -2};
  double x = 1;

  double delta;
  auto isConverging = [](int count) { return count < 19; };
  int iterationCount = 0;

  do {
    const double x1 = calcResult(a, x);
    delta = abs((x1 - x) / x);
    x = x1;
    iterationCount++;
  } while (delta >= 0.00001 && isConverging(iterationCount));

  cout << "x: " << x << endl;
  return 0;
}
