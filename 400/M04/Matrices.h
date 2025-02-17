#ifndef MATRICES_H
#define MATRICES_H

#include <iomanip>
#include <iostream>
#include <vector>

using namespace std;

namespace Matrices {
class Matrix {
 public:
  /// Construct a matrix of the specified size.
  /// Initialize each element to 0.
  Matrix(int _rows, int _cols)
      : a(vector<vector<double>>(_rows, std::vector<double>(_cols, 0))),
        rows(_rows),
        cols(_cols) {};

  /// inline accessors / mutators, these are done:

  /// Read element at row i, column j
  /// usage:  double x = a(i,j);
  const double& operator()(int i, int j) const { return a.at(i).at(j); }

  /// Assign element at row i, column j
  /// usage:  a(i,j) = x;
  double& operator()(int i, int j) {
    // return a[i][j];
    return a.at(i).at(j);
  }

  int getRows() const { return rows; }
  int getCols() const { return cols; }

  private:
  vector<vector<double>> a;
  int rows;
  int cols;
};

/// Add each corresponding element.
/// usage:  c = a + b;
Matrix operator+(const Matrix& a, const Matrix& b);

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

#endif  // MATRICES_H
