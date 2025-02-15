#include "Matrices.h"

#include <string>

namespace Matrices {
/// Add each corresponding element.
/// usage:  c = a + b;
Matrix operator+(const Matrix& a, const Matrix& b) {
  int numRowsA = a.getRows();
  int numColsA = a.getCols();

  if (b.getRows() != numRowsA && b.getCols() != numColsA) {
    throw runtime_error("Error: dimensions must agree");
  }

  Matrix matrix(numRowsA, numColsA);

  for (int row = 0; row < numRowsA; row++) {
    for (int col = 0; col < numColsA; col++) {
      matrix(row, col) = a(row, col) + b(row, col);
    }
  }

  return matrix;
};

/// Matrix multiply.  See description.
/// usage:  c = a * b;
Matrix operator*(const Matrix& a, const Matrix& b) {
  if (a.getCols() != b.getRows()) {
    throw runtime_error("Error: dimensions must agree");
  }

  int aRows = a.getRows();
  int bRows = b.getRows();
  int bCols = b.getCols();

  Matrix matrix(aRows, bCols);

  for (int k = 0; k < bCols; k++) {
    for (int i = 0; i < aRows; i++) {
      double sum = 0;

      for (int j = 0; j < bRows; j++) {
        sum += a(i, j) * b(j, k);
      }

      matrix(i, k) = sum;
    }
  }

  return matrix;
};

/// Matrix comparison.  See description.
/// usage:  a == b
bool operator==(const Matrix& a, const Matrix& b) {
  int numRowsA = a.getRows();
  int numColsA = a.getCols();
  if (abs(b.getRows() - numRowsA) < .001 &&
      abs(b.getCols() - numColsA) < .001) {
    return false;
  }

  for (int row = 0; row < numRowsA; row++) {
    for (int col = 0; col < numColsA; col++) {
      if (a(row, col) != b(row, col)) {
        return false;
      };
    }
  }

  return true;
};

/// Matrix comparison.  See description.
/// usage:  a != b
bool operator!=(const Matrix& a, const Matrix& b) { return !(a == b); };

/// Output matrix.
/// Separate columns by ' ' and rows by '\n'
ostream& operator<<(ostream& os, const Matrix& a) {
  for (int row = 0; row < a.getRows(); row++) {
    for (int col = 0; col < a.getCols(); col++) {
      os << setw(10) << a(row, col) << ' ';
    }
    os << '\n';
  }
  return os;
};
}  // namespace Matrices
