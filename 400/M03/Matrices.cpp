#include "Matrices.h"

namespace Matrices {
/// Add each corresponding element.
/// usage:  c = a + b;
Matrix operator+(const Matrix& a, const Matrix& b) {
  if (a.getRows() == b.getRows() && a.getCols() == b.getCols()) {
    Matrix matrix(3, 3);
	
    return matrix;
  }
  throw runtime_error("Error: dimensions must agree");
};

/// Matrix multiply.  See description.
/// usage:  c = a * b;
Matrix operator*(const Matrix& a, const Matrix& b);

/// Matrix comparison.  See description.
/// usage:  a == b
bool operator==(const Matrix& a, const Matrix& b);

/// Matrix comparison.  See description.
/// usage:  a != b
bool operator!=(const Matrix& a, const Matrix& b);

/// Output matrix.
/// Separate columns by ' ' and rows by '\n'
ostream& operator<<(ostream& os, const Matrix& a);
}  // namespace Matrices
