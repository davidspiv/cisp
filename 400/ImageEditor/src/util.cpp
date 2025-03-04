// https://stackoverflow.com/questions/596216/formula-to-determine-perceived-brightness-of-rgb-color

#include <algorithm>
#include <cmath>

#include "ImageEditor.h"

double linearizeRGB(int channel) {
  double normalizedRGB = channel / 255.0;

  if (normalizedRGB <= 0.04045) {
    return normalizedRGB / 12.92;
  } else {
    return pow(((normalizedRGB + 0.055) / 1.055), 2.4);
  }
}

double calcLightness(Color c) {
  // apply standard coefficients for sRGB to get luminance (Y)
  const double linearR = 0.2126 * linearizeRGB(c.r);
  const double linearG = 0.7152 * linearizeRGB(c.g);
  const double linearB = 0.0722 * linearizeRGB(c.b);
  const double Y = linearR + linearG + linearB;

  // convert Y to perceived lightness (L*)
  if (Y <= (216 / 24389.0)) {   // more exact than min CIE standard of 0.008856
    return Y * (24389 / 27.0);  // more exact than min CIE standard of 903.3
  } else {
    return cbrt(Y) * 116 - 16;
  }
}

size_t scaleValue(double x, size_t currMax, size_t newMax) {
  if (!currMax) {
    throw std::invalid_argument("currMax cannot be zero");
  }
  return newMax * (x / currMax);
}

size_t clamp(double x, size_t max) {
  if (x > max) return max;
  return x;
}

vector<double> calcGaussianKernelProduct(int size) {
  if (size % 2 == 0) {
    throw invalid_argument("Kernel size must be odd.");
  }

  double sigma = (size - 1) / 6.0;

  vector<double> kernel(size, 0);
  int radius = size / 2;
  double sum = 0.0;

  // Compute the raw Gaussian values
  for (int i = -radius; i <= radius; i++) {
    kernel[i + radius] = round(exp(-(i * i) / (2.0 * sigma * sigma)) * 100);
    sum += kernel[i + radius];
  }

  // Normalize the kernel
  transform(kernel.begin(), kernel.end(), kernel.begin(),
            [sum](int val) { return (val / sum); });

  return kernel;
}

int mirrorPixel(int x, int max) {
  if (x < 0) return -x;                    // Mirror left/top side
  if (x >= max) return 2 * (max - 1) - x;  // Mirror right/bottom side
  return x;
}
