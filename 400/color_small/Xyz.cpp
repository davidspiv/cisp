#include "../include/Color.h"
#include "../include/util.h"

#include <algorithm>
#include <array>
#include <cmath>

namespace Color_Space {

float apply_gamma(const float c) {
  if (c <= 0) {
    return c;
  }

  return (c <= 0.0031308f) ? (c * 12.92f)
                           : 1.055f * std::pow(c, 1.0f / 2.4f) - 0.055f;
}


Xyz::Xyz(float x, float y, float z) : Color(x, y, z) {}


Rgb Xyz::to_rgb() const {
  const Matrix color_as_column = REF_WHITE_D65.to_column();

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
  auto [w_X, w_Y, w_Z] = REF_WHITE_D65.get_values();

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
