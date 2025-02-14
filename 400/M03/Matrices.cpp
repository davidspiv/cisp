#include "Matrices.h"

#include <string>

namespace Matrices {
/// Add each corresponding element.
/// usage:  c = a + b;
Matrix operator+(const Matrix& a, const Matrix& b) {
  int numRows = a.getRows();
  int numCols = a.getCols();
  if (b.getRows() == numRows && b.getCols() == numCols) {
    Matrix matrix(numRows, numCols);
    for (int row = 0; row < numRows; row++) {
      for (int col = 0; col < numCols; col++) {
        matrix(row, col) = a(row, col) + b(row, col);
      }
    }

    return matrix;
  }
  throw runtime_error("Error: dimensions must agree");
};

/// Matrix multiply.  See description.
/// usage:  c = a * b;
Matrix operator*(const Matrix& a, const Matrix& b);

/// Matrix comparison.  See description.
/// usage:  a == b
bool operator==(const Matrix& a, const Matrix& b) {
  int numRows = a.getRows();
  int numCols = a.getCols();
  if (b.getRows() != numRows && b.getCols() != numCols) {
    return false;
  }

  for (int row = 0; row < numRows; row++) {
    for (int col = 0; col < numCols; col++) {
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
