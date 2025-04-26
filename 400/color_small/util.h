#pragma once

#include "Color.h"
#include "Matrix.h"

using namespace Color_Space;

static constexpr float epsilon = 216.0f / 24389.0f;
static constexpr float kappa = 24389.0f / 27.0f;
const Xyz REF_WHITE_D65 = Xyz(0.95047f, 1.00000f, 1.08883f);

Matrix create_to_xyz_transformation_matrix(Xyz ref_white) {
  auto [r_x, r_y, r_z] = Xyz(0.6400f, 0.3300f, 0.212656f).get_values();
  auto [g_x, g_y, g_z] = Xyz(0.3000f, 0.6000f, 0.715158f).get_values();
  auto [b_x, b_y, b_z] = Xyz(0.1500f, 0.0600f, 0.072186f).get_values();

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
  const Matrix S_matrix = XYZ_matrix_inverted.multiply(ref_white.to_column());

  return XYZ_matrix.column_wise_scaling(S_matrix);
}
