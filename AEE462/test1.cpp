#include <math.h>

#include <iostream>
#include <vector>

using namespace std;

int main() {
  auto isConverging = [](int count) { return count < 19; };

  double x = -1.967392;

  double delta;
  int iterationCount = 0;

  vector<int> a = {3, -10, -31, 4, 111, 205};

  double numerator = (a.size() - 2.0) * a[a.size() - 1] * x +
                     (a.size() - 3.0) * a[a.size() - 2];
  cout << "el:   " << a[a.size() - 1] << endl;
  // cout << "i: " << a.size() - 2.0 << endl;
  cout << "el:   " << a[a.size() - 2] << endl;
  // cout << "i: " << (a.size() - 3.0) << endl;

  for (size_t i = a.size() - 3; i > 1; i--) {
    numerator = numerator * x + (i - 1) * a[i];

    cout << "el:   " << a[i] << endl;
    // cout << "i: " << i - 1 << endl;
  }
  numerator = numerator * pow(x, 2) - a[0];
  //   cout << "el:   " << a[0] << endl;

  double denominator = (a.size() - 1.0) * a[a.size() - 1] * x +
                       (a.size() - 2.0) * a[a.size() - 2];

  // cout << "el:   " << a[a.size() - 1] << endl;
  // cout << "i: " << a.size() - 1.0 << endl;
  // cout << "el:   " << a[a.size() - 2] << endl;
  // cout << "i: " << (a.size() - 2.0) << endl;

  for (size_t i = a.size() - 3; i > 0; i--) {
    denominator = denominator * x + i * a[i];
    // cout << "el:   " << a[i] << endl;
    // cout << "i: " << i << endl;
  }
  const double result = numerator / denominator;

  //   do {
  //     const double x1 = x - (3 * pow(x, 2) - 12 * x + 9) / (6 * x - 12);
  //     delta = abs(x1 - x);
  //     x = x1;
  //     iterationCount++;
  //     cout << x1 << endl;
  //   } while (delta >= 0.00001 && isConverging(iterationCount));

  // cout << "x: " << x << endl;
  // cout << "result: " << result << endl;
  return 0;
}
