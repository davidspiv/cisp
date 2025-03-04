#include "ImageEditor.h"

// estimates perceived lightness (L*) in the CIELAB colorspace. Returns a
// value from 0 to 100 inclusive
double calcLightness(Color c);

// creates a 1D matrix that, when multiplied by its transposed counterpart
// will create a normalized gaussian kernel of arbitrary size
vector<double> calcGaussianKernelProduct(int size);

// effectively "tiles" picture on a per-pixel basis to avoid bounds-errors when
// sampling near the edge
int mirrorPixel(int x, int max);

// normalizes a value assuming both the current min and new min are zero
size_t scaleValue(double x, size_t currMax, size_t newMax);

// clamps value to a max
size_t clamp(double x, size_t max);

// convert sRGB channel to a linear value
double linearizeRGB(int channel);
