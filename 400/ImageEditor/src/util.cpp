// https://stackoverflow.com/questions/596216/formula-to-determine-perceived-brightness-of-rgb-color

#include <algorithm>
#include <cmath>

#include "ImageEditor.h"

size_t scaleValue(double x, size_t currMax, size_t newMax) {
  if (!currMax) {
    throw std::invalid_argument("currMax cannot be zero");
  }
  return newMax * (x / currMax);
}

int mirrorPixel(int x, int max) {
  if (x < 0)
    return -x; // Mirror left/top side
  if (x >= max)
    return 2 * (max - 1) - x; // Mirror right/bottom side
  return x;
}

unsigned char quantizeChannel(unsigned char channel) {
  const int regions = 7;
  const double regionSize = 255.0 / (regions - 1.0);

  int quantized = round(channel / regionSize);               // scale down
  quantized = std::max(0, std::min(regions - 1, quantized)); // quantize
  quantized *= regionSize;                                   // scale back up

  return static_cast<unsigned char>(quantized);
};
