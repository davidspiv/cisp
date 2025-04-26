#pragma once

#include "Matrix.h"

#include <array>
#include <iostream>
#include <string>
#include <unordered_map>

namespace Color_Space {


// forward declarations
class Lab;
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
   * @param illuminant Cie illuminant (default D65)
   */
  Lab(float l, float a, float b);

  /**
   * @brief Converts Lab to XYZ color space.
   * @return the converted color as a Xyz object
   */
  Xyz to_xyz() const;

  /**
   * @brief Approximates color difference between the two colors, using the
   * CIEDE2000 formula.
   * @param other Lab color to compute with.
   * @return perceptual distance as defined by the CIE (ΔE*)
   */
  [[nodiscard]] float diff_cie_2000(const Lab &other) const;

  /**
   * @brief Prints Lab components to the console.
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
   * @param illuminant Cie illuminant (default D65)
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
   * @param illuminant Cie illuminant (default D65)
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


} // namespace Color_Space
