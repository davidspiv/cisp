#ifndef UTIL_H
#define UTIL_H

#include "ImageEditor.h"

// effectively "tiles" picture on a per-pixel basis to avoid bounds-errors when
// sampling near the edge
int mirrorPixel(int x, int max);

// normalizes a value assuming both the current min and new min are zero
size_t scaleValue(double x, size_t currMax, size_t newMax);

// clamps value to a max
size_t clamp(double x, size_t max);

#endif
