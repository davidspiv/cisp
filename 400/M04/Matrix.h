#ifndef MATRICES_H
#define MATRICES_H

#include <iostream>
#include <vector>

class Matrix {
 public:
  Matrix(int rows, int cols);

  size_t rows() const { return m_rows; }
  size_t cols() const { return m_cols; }

  const float& operator()(int i, int j) const;  // Read
  float& operator()(int i, int j);              // Assign

  Matrix multiply(const Matrix& other) const;

 private:
  std::vector<std::vector<float>> m_values;
  size_t m_rows;
  size_t m_cols;
};

std::ostream& operator<<(std::ostream& os, const Matrix& a);

#endif
