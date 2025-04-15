#include "Matrix.h"

#include <iomanip>

Matrix::Matrix(int rows, int cols)
    : m_values(
          std::vector<std::vector<float>>(rows, std::vector<float>(cols, 0))),
      m_rows(rows),
      m_cols(cols) {};

float& Matrix::operator()(int i, int j) { return m_values[i][j]; }

const float& Matrix::operator()(int i, int j) const { return m_values[i][j]; }

Matrix Matrix::multiply(const Matrix& other) const {
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

std::ostream& operator<<(std::ostream& os, const Matrix& a) {
  for (size_t row = 0; row < a.rows(); row++) {
    for (size_t col = 0; col < a.cols(); col++) {
      os << std::setw(5) << a(row, col) << ' ';
    }
    os << '\n';
  }
  return os;
}
