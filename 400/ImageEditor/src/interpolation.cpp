// https://chao-ji.github.io/jekyll/update/2018/07/19/BilinearResize.html

#include "../include/ImageEditor.h"
#include <algorithm>
#include <cmath>


struct Pixel {
  double r, g, b;
};

int clampVal(double val) { return std::clamp(int(std::round(val)), 0, 255); }

void ImageEditor::bilinearResize(double factor) {
  if (factor == 1)
    return;

  // returns a Pixel struct not associated with the ImageEditor class.
  auto getPixel = [&](int x, int y) -> const Pixel {
    return {double(pic.red(x, y)), double(pic.green(x, y)),
            double(pic.blue(x, y))};
  };

  const size_t inHeight = pic.height();
  const size_t inWidth = pic.width();
  const size_t outHeight = round(inHeight * factor);
  const size_t outWidth = round(inWidth * factor);

  const double xRatio = outWidth > 1 ? double(inWidth - 1) / (outWidth - 1) : 0;
  const double yRatio =
      outHeight > 1 ? double(inHeight - 1) / (outHeight - 1) : 0;

  Picture newPic(outWidth, outHeight, 0, 0, 0);

  for (size_t i = 0; i < outHeight; i++) {
    for (size_t j = 0; j < outWidth; j++) {
      const int yLow = std::floor(yRatio * i);
      const int xLow = std::floor(xRatio * j);
      const int xHigh = std::min(xLow + 1, int(inWidth - 1));
      const int yHigh = std::min(yLow + 1, int(inHeight - 1));

      const double yWeight = yRatio * i - yLow;
      const double xWeight = xRatio * j - xLow;

      // A,B,C, and D are known pixel values in original image
      Pixel A = getPixel(xLow, yLow);
      Pixel B = getPixel(xHigh, yLow);
      Pixel C = getPixel(xLow, yHigh);
      Pixel D = getPixel(xHigh, yHigh);

      // computes a weighted average of the values associated with the four
      // closest points
      auto interpolate = [xWeight, yWeight](double a, double b, double c,
                                            double d) {
        // We first compute the interpolated value of AB and CD in the width
        // dimension
        const double interpolatedAB = a * (1 - xWeight) + b * xWeight;
        const double interpolatedCD = c * (1 - xWeight) + d * xWeight;

        // Then we will do linear interpolation between the points generated
        // from the two previous interpolations above
        return interpolatedAB * (1 - yWeight) + interpolatedCD * yWeight;
      };

      auto calcPixel = [interpolate](const Pixel &A, const Pixel &B,
                                     const Pixel &C, const Pixel &D) -> Pixel {
        return {interpolate(A.r, B.r, C.r, D.r),
                interpolate(A.g, B.g, C.g, D.g),
                interpolate(A.b, B.b, C.b, D.b)};
      };

      Pixel pixel = calcPixel(A, B, C, D);

      newPic.set(j, i, clampVal(pixel.r), clampVal(pixel.g), clampVal(pixel.b));
    }
  }

  pic = newPic;
};
