#pragma once

#include <algorithm>
#include <array>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#define _USE_MATH_DEFINES // for VS

class Matrix {
public:
  Matrix(int rows, int cols);
  explicit Matrix(const std::vector<std::vector<float>> &arr_2d);

  size_t rows() const { return m_rows; }
  size_t cols() const { return m_cols; }

  const float &operator()(int i, int j) const; // Read
  float &operator()(int i, int j);             // Assign

  Matrix multiply(const Matrix &other) const;
  Matrix column_wise_scaling(const Matrix &other) const;
  Matrix invert() const;
  std::vector<float> flatten() const;

private:
  std::vector<std::vector<float>> m_values;
  size_t m_rows;
  size_t m_cols;
};


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


Matrix arr_to_column(std::array<float, 3> arr) {
  return Matrix({{arr[0]}, {arr[1]}, {arr[2]}});
}


Matrix create_to_xyz_transformation_matrix(std::array<float, 3> ref_white) {
  const auto [r_x, r_y, r_z] =
      std::array<float, 3>{0.6400f, 0.3300f, 0.212656f};
  const auto [g_x, g_y, g_z] =
      std::array<float, 3>{0.3000f, 0.6000f, 0.715158f};
  const auto [b_x, b_y, b_z] =
      std::array<float, 3>{0.1500f, 0.0600f, 0.072186f};

  const float r_X = r_x / r_y;
  const float r_Y = 1;
  const float r_Z = (1 - r_x - r_y) / r_y;

  const float g_X = g_x / g_y;
  const float g_Y = 1;
  const float g_Z = (1 - g_x - g_y) / g_y;

  const float b_X = b_x / b_y;
  const float b_Y = 1;
  const float b_Z = (1 - b_x - b_y) / b_y;

  const std::vector<std::vector<float>> XYZ = {
      {r_X, g_X, b_X}, {r_Y, g_Y, b_Y}, {r_Z, g_Z, b_Z}};

  const Matrix XYZ_matrix(XYZ);
  const Matrix XYZ_matrix_inverted = XYZ_matrix.invert();
  const Matrix S_matrix =
      XYZ_matrix_inverted.multiply(arr_to_column(ref_white));

  return XYZ_matrix.column_wise_scaling(S_matrix);
}


// ================================
// ============ Color =============
// ================================

namespace Color_Space {

static constexpr float epsilon = 216.0f / 24389.0f;
static constexpr float kappa = 24389.0f / 27.0f;
const std::array<float, 3> REF_WHITE_D65 = {0.95047f, 1.00000f, 1.08883f};

// forward declarations
class Lab;
class Lch_Ab;
class Rgb;
class Xyz;

class Color {
protected:
  std::array<float, 3> m_values;

public:
  Color(float x, float y, float z);

  /**
   * @brief Returns the color's internal channel values.
   * @return Array containing 3 float components
   */
  [[nodiscard]] std::array<float, 3> get_values() const;

  /**
   * @brief Returns the color as a 3x1 column matrix representation.
   * @return Matrix with column form of color values.
   */
  [[nodiscard]] Matrix to_column() const;

  virtual void print() const;

  [[nodiscard]] bool operator==(const Color &other) const;
  [[nodiscard]] bool operator!=(const Color &other) const;
};


class Lab : public Color {
public:
  /**
   * @brief Constructs a Lab color.
   * @param l Lightness (L*)
   * @param a Green–Red component (a*)
   * @param b Blue–Yellow component (b*)
   */
  Lab(float l, float a, float b);

  /**
   * @brief Converts Lab to XYZ color space.
   * @return the converted color as a Xyz object
   */
  Xyz to_xyz() const;

  /**
   * @brief Converts Lab to cylindrical Lch(ab) format.
   * @return the converted color as a Lch_Ab object
   */
  Lch_Ab to_lch_ab() const;

  /**
   * @brief Prints Lab components to the console.
   */
  void print() const override;
};


class Lch_Ab : public Color {
public:
  /**
   * @brief Constructs an Lch(ab) color (cylindrical Lab).
   * @param l Lightness
   * @param c Chroma
   * @param h Hue angle [degrees]
   */
  Lch_Ab(float l, float c, float h);

  [[nodiscard]] Lab to_lab() const;

  /**
   * @brief Prints Lch(ab) components to the console.
   */
  void print() const override;
};


class Rgb : public Color {
public:
  /**
   * @brief Constructs an RGB color.
   * @param r Red channel
   * @param g Green channel
   * @param b Blue channel
   */
  Rgb(float r, float g, float b);

  /**
   * @brief Converts RGB to XYZ color space.
   * @param profile The RGB color space (sRGB, Adobe RGB, etc.)
   * @return the converted color as a Xyz object
   */
  [[nodiscard]] Xyz to_xyz() const;

  /**
   * @brief Prints RGB components to the console.
   */
  void print() const override;
};


class Xyz : public Color {
public:
  /**
   * @brief Constructs an XYZ color.
   * @param X a mix of the three CIE RGB curves chosen to be nonnegative (X)
   * @param Y luminance (Y)
   * @param Z quasi-equal to the "blue" monochromatic primary (Z)
   */
  Xyz(float X, float Y, float Z);

  /**
   * @brief Converts XYZ to RGB.
   * @param profile RGB color space to use for conversion.
   * @return RGB color.
   */
  [[nodiscard]] Rgb to_rgb() const;

  /**
   * @brief Converts XYZ to Lab.
   * @return the converted color as a Lab object
   */
  [[nodiscard]] Lab to_lab() const;

  /**
   * @brief Prints XYZ components to the console.
   */
  void print() const;
};


// UTILS

float to_radians(const float degrees) { return degrees * (M_PI / 180.f); }


float to_degrees(const float radians) { return radians * (180.0 / M_PI); }


float remove_gamma(float c) {
  if (c <= 0) {
    return c;
  }

  return (c <= 0.04045f) ? (c / 12.92f) : std::pow((c + 0.055f) / 1.055f, 2.4f);
}


float apply_gamma(const float c) {
  if (c <= 0) {
    return c;
  }

  return (c <= 0.0031308f) ? (c * 12.92f)
                           : 1.055f * std::pow(c, 1.0f / 2.4f) - 0.055f;
}


double normalize_degrees(double x) {
  return x - std::floor(x / 360.0f) * 360.0f;
}


std::array<float, 3>
to_polar_color_space(const std::array<float, 3> &cartesianColor_Space) {
  const auto [l, a, b] = cartesianColor_Space; // LchAb equivalents: a=u and b=v
  const float c = std::sqrt(a * a + b * b);
  const float h_component = to_degrees(std::atan2(b, a));
  const float h = (h_component >= 0) ? h_component : h_component + 360.0;

  return {l, c, h};
}


Color::Color(float x, float y, float z) { m_values = {x, y, z}; }


std::array<float, 3> Color::get_values() const { return m_values; }


void Color::print() const {
  std::cout << "[GEN]" << "\nC1: " << m_values[0] << "\nC2: " << m_values[1]
            << "\nC3: " << m_values[2] << "\n\n";
}


Matrix Color::to_column() const {
  return Matrix({{m_values[0]}, {m_values[1]}, {m_values[2]}});
};


bool Color::operator==(const Color &other) const {
  auto [x, y, z] = m_values;
  float error = 1;
  auto [other_x, other_y, other_z] = other.get_values();
  return (std::abs(x - other_x) < error) && (std::abs(y - other_y) < error) &&
         (std::abs(z - other_z) < error);
}


bool Color::operator!=(const Color &other) const { return !(*this == other); }


// ================================
// =========== Lab Space ==========
// ================================

Lab::Lab(float l, float a, float b) : Color(l, a, b) {}


Xyz Lab::to_xyz() const {
  const float fX = (m_values[0] + 16.0f) / 116.0f;
  const float fY = fX + (m_values[1] / 500.0f);
  const float fZ = fX - (m_values[2] / 200.0f);

  const float rX = (std::pow(fY, 3.0f) > epsilon)
                       ? std::pow(fY, 3)
                       : (116.0f * fY - 16.0f) / kappa;
  const float rY =
      (m_values[0] > (kappa * epsilon)) ? std::pow(fX, 3) : m_values[0] / kappa;
  const float rZ = (std::pow(fZ, 3.0f) > epsilon)
                       ? std::pow(fZ, 3)
                       : (116.0f * fZ - 16.0f) / kappa;

  auto [ref_x, ref_y, ref_z] = REF_WHITE_D65;

  const float x = rX * ref_x;
  const float y = rY * ref_y;
  const float z = rZ * ref_z;

  return Xyz(x, y, z);
}


Lch_Ab Lab::to_lch_ab() const {
  const auto [l, c, h] = to_polar_color_space(m_values);

  return Lch_Ab(l, c, h);
};


void Lab::print() const {
  std::cout << "[Lab]" << "\nL: " << m_values[0] << "\na: " << m_values[1]
            << "\nb: " << m_values[2] << "\n\n";
}


// ================================
// ========= LCH(ab) Space =========
// ================================

Lch_Ab::Lch_Ab(float l, float c, float h) : Color(l, c, h) {}


Lab Lch_Ab::to_lab() const {
  auto [l, c, h_deg] = m_values;
  const float h_rad = to_radians(h_deg);

  const float a = c * std::cos(h_rad);
  const float b = c * std::sin(h_rad);

  return Lab(l, a, b);
}


void Lch_Ab::print() const {
  std::cout << "[LCHab]" << "\nL: " << m_values[0] << "\nc: " << m_values[1]
            << "\nh: " << m_values[2] << "\n\n";
}


// ================================
// ========== sRGB Space ==========
// ================================


Rgb::Rgb(float r, float g, float b) : Color(r, g, b) {
  auto validate = [](float c) {
    if (std::min(255.0f, std::max(0.0f, c)) != c) {
      throw std::domain_error("Channel initalized outside of range [0, 255].");
    }
  };

  validate(r);
  validate(g);
  validate(b);
};


Xyz Rgb::to_xyz() const {

  auto [r, g, b] = m_values;

  // Step 1: Normalize input RGB [0–255] -> [0.0–1.0]
  const float r_lin = remove_gamma(r / 255.0f);
  const float g_lin = remove_gamma(g / 255.0f);
  const float b_lin = remove_gamma(b / 255.0f);

  // Step 2: Convert to XYZ using matrix
  const Matrix M_matrix = create_to_xyz_transformation_matrix(REF_WHITE_D65);
  const Matrix rgb_lin({{r_lin}, {g_lin}, {b_lin}});
  const Matrix xyz_matrix = M_matrix.multiply(rgb_lin);

  return Xyz(xyz_matrix(0, 0), xyz_matrix(1, 0), xyz_matrix(2, 0));
}


void Rgb::print() const {
  std::cout << "[Rgb]" << "\nr: " << m_values[0] << "\ng: " << m_values[1]
            << "\nb: " << m_values[2] << std::endl;
}


// ================================
// ========== XYZ Space ===========
// ================================

Xyz::Xyz(float x, float y, float z) : Color(x, y, z) {}


Rgb Xyz::to_rgb() const {

  const Matrix color_as_column = to_column();

  const Matrix M_matrix =
      create_to_xyz_transformation_matrix(REF_WHITE_D65).invert();

  Matrix xyz_as_matrix = M_matrix.multiply(color_as_column);

  // Absolute colorimetric
  const float r_corr = apply_gamma(xyz_as_matrix(0, 0));
  const float g_corr = apply_gamma(xyz_as_matrix(1, 0));
  const float b_corr = apply_gamma(xyz_as_matrix(2, 0));

  const float r_norm = std::clamp(r_corr, 0.0f, 1.0f) * 255.0f;
  const float g_norm = std::clamp(g_corr, 0.0f, 1.0f) * 255.0f;
  const float b_norm = std::clamp(b_corr, 0.0f, 1.0f) * 255.0f;

  return Rgb(r_norm, g_norm, b_norm);
}


Lab Xyz::to_lab() const {
  auto [w_X, w_Y, w_Z] = REF_WHITE_D65;

  const float xR = m_values[0] / w_X;
  const float yR = m_values[1] / w_Y;
  const float zR = m_values[2] / w_Z;

  const float fX = (xR > epsilon) ? std::cbrt(xR) : (kappa * xR + 16) / 116.0f;
  const float fY = (yR > epsilon) ? std::cbrt(yR) : (kappa * yR + 16) / 116.0f;
  const float fZ = (zR > epsilon) ? std::cbrt(zR) : (kappa * zR + 16) / 116.0f;

  const float l = 116.0f * fY - 16;
  const float a = 500.0f * (fX - fY);
  const float b = 200.0f * (fY - fZ);

  return Lab(l, a, b);
}


void Xyz::print() const {
  std::cout << "[XYZ]" << "\nX: " << m_values[0] << "\nY: " << m_values[1]
            << "\nZ: " << m_values[2] << "\n\n";
}

} // namespace Color_Space


/**
 * @brief Generates a smooth rainbow of colors starting from a specified color.
 *
 * @param sample_count The number of colors to generate. Must be at least 2.
 * @param start_color The color where the rainbow starts. Defaults to red.
 * @param rainbow_percent The portion of the color wheel to cover, as a
 * percentage (100 = full rainbow, 50 = half rainbow, etc.). Defaults to 100.
 *
 * @return std::vector<sf::Color> A list of colors smoothly transitioning across
 * the specified portion of the rainbow.
 */
std::vector<sf::Color>
get_rainbow_colors(int sample_count, sf::Color start_color = sf::Color::Red,
                   float rainbow_percent = 100.f) {
  if (sample_count < 2) {
    throw std::domain_error(
        "sample count must be >= 2 for correct interpolation.");
  }

  const float LIGHTNESS = 70.f;
  const float CHROMA = 60.f;
  const float sample_degrees = (360.0f * rainbow_percent) / 100.0f;

  auto [l, c, start_hue] =
      Color_Space::Rgb(start_color.r, start_color.g, start_color.b)
          .to_xyz()
          .to_lab()
          .to_lch_ab()
          .get_values();

  start_hue -= 20.f; // offset to match perceived color

  std::vector<sf::Color> colors;
  colors.reserve(sample_count);

  for (int i = 0; i < sample_count; ++i) {

    const float hue = Color_Space::normalize_degrees(
        start_hue + (sample_degrees * i) / (sample_count - 1));

    const Color_Space::Lch_Ab lch_ab(LIGHTNESS, CHROMA, hue);

    const auto [r, g, b] = lch_ab.to_lab().to_xyz().to_rgb().get_values();

    colors.push_back(sf::Color(static_cast<uint8_t>(r), static_cast<uint8_t>(g),
                               static_cast<uint8_t>(b)));
  }

  return colors;
}
