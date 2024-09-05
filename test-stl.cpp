#include <algorithm>
#include <iostream>

using namespace std;

int main() {
  int array[] = {123, 14, 14, 23345, 23, 4, 54, 6, 574, 45, 544645};
  const int max = sizeof(array) / sizeof(*array);

  cout << "max " << max << endl;
  int* i = max_element(array, array + max);
  cout << int(*i) << endl;
  // for (int el : array) {
  //   cout << el << endl;
  // }
}
