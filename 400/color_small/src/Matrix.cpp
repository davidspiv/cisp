#include "../include/Matrix.h"

#include <iomanip>
#include <vector>

Matrix::Matrix(int rows, int cols)
    : m_values(
          std::vector<std::vector<float>>(rows, std::vector<float>(cols, 0))),
      m_rows(rows), m_cols(cols) {};


Matrix::Matrix(const std::vector<std::vector<float>> &arr_2d) {
  m_rows = arr_2d.size();
  m_cols = arr_2d[0].size();
  m_values = arr_2d;
};


float &Matrix::operator()(int i, int j) { return m_values[i][j]; }


const float &Matrix::operator()(int i, int j) const { return m_values[i][j]; }


Matrix Matrix::multiply(const Matrix &other) const {
  if (m_cols != other.rows()) {
    throw std::runtime_error("Error: mismatched inner dimensions");
  }

  Matrix result(m_rows, other.cols());

  for (size_t i = 0; i < m_rows; i++) {
    for (size_t k = 0; k < other.cols(); k++) {
      float sum = 0;
      for (size_t j = 0; j < m_cols; j++) {
        sum += m_values[i][j] * other(j, k);
      }
      result(i, k) = sum;
    }
  }

  return result;
}


Matrix Matrix::column_wise_scaling(const Matrix &other) const {
  Matrix result(m_rows, m_cols);

  for (size_t j = 0; j < m_cols; j++) {
    for (size_t i = 0; i < m_rows; i++) {
      result(i, j) = m_values[i][j] * other(j, 0);
    }
  }

  return result;
};


// LLM code
Matrix Matrix::invert() const {
  if (m_rows != m_cols) {
    throw std::runtime_error(
        "Matrix inversion only defined for square matrices.");
  }

  int n = m_rows;
  Matrix result(n, n);
  Matrix augmented(n, 2 * n);

  // Step 1: Set up augmented matrix [A | I]
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
      augmented(i, j) = (*this)(i, j);              // original A
      augmented(i, j + n) = (i == j) ? 1.0f : 0.0f; // identity matrix I
    }
  }

  // Step 2: Forward elimination
  for (int i = 0; i < n; ++i) {
    // Find pivot
    float pivot = augmented(i, i);
    if (pivot == 0.0f) {
      throw std::runtime_error("Matrix is singular and cannot be inverted.");
    }

    // Normalize the pivot row
    for (int j = 0; j < 2 * n; ++j) {
      augmented(i, j) /= pivot;
    }

    // Eliminate column i in other rows
    for (int k = 0; k < n; ++k) {
      if (k == i)
        continue;
      float factor = augmented(k, i);
      for (int j = 0; j < 2 * n; ++j) {
        augmented(k, j) -= factor * augmented(i, j);
      }
    }
  }

  // Step 3: Copy right half to result
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
      result(i, j) = augmented(i, j + n);
    }
  }

  return result;
}


std::vector<float> Matrix::flatten() const {
  std::vector<float> values(m_cols * m_rows, 0.0f);

  for (size_t i = 0; i < m_cols; i++) {
    for (size_t j = 0; j < m_rows; j++) {
      values[i + j] = m_values[i][j];
    }
  }
  return values;
};


std::ostream &operator<<(std::ostream &os, const Matrix &a) {
  for (size_t row = 0; row < a.rows(); row++) {
    for (size_t col = 0; col < a.cols(); col++) {
      os << std::setw(10) << a(row, col) << ' ';
    }
    os << '\n';
  }
  return os;
}
