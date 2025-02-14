#include <iostream>

#include "Matrices.h"
#include "io.h"
int main() {
  // TEST
  Matrices::Matrix matrix(3, 3);
  for (int row = 0; row < matrix.getRows(); row++) {
    for (int col = 0; col < matrix.getCols(); col++) {
      print(matrix(row, col));
    }
  }
}
