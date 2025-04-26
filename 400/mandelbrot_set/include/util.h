#pragma once

#include "Matrix.h"

#include <array>
#include <cmath>

#define _USE_MATH_DEFINES // for VS

static constexpr float epsilon = 216.0f / 24389.0f;
static constexpr float kappa = 24389.0f / 27.0f;
const std::array<float, 3> REF_WHITE_D65 = {0.95047f, 1.00000f, 1.08883f};


float to_radians(const float degrees) { return degrees * (M_PI / 180.0); }


float apply_gamma(const float c) {
  if (c <= 0) {
    return c;
  }

  return (c <= 0.0031308f) ? (c * 12.92f)
                           : 1.055f * std::pow(c, 1.0f / 2.4f) - 0.055f;
}


float remove_gamma(float c) {
  if (c <= 0) {
    return c;
  }

  return (c <= 0.04045f) ? (c / 12.92f) : std::pow((c + 0.055f) / 1.055f, 2.4f);
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
