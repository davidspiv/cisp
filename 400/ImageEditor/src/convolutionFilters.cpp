#include "../include/ImageEditor.h"
#include "../include/util.h"
#include <algorithm>
#include <cmath>

// GAUSSIAN

// creates a 1D matrix that, when multiplied by its transposed counterpart
// will create a normalized gaussian kernel of arbitrary size
vector<double> calcGaussianKernelComponent(size_t size) {
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
            [sum](double val) { return (val / sum); });

  return kernel;
}

//  A gaussian kernel is a separable matrix, which means we can use its products
//  to perform two separate convolutions for each pixel. We then multiply the
//  results of both operations together to get an equivalent result, saving a
//  4th inner loop and a bunch of multiplication. Modifies picture object.
void ImageEditor::gaussianBlur(const size_t strength) {
  Picture tempPic = pic;
  const size_t width = pic.width();
  const size_t height = pic.height();

  // kSize will be rounded down to an odd number to keep target pixel centered
  const size_t kSize = strength % 2 ? strength : strength - 1;

  // half the matrix not including the center
  const int kRadius = kSize / 2;

  // used both as an 1 x kSize kernel and a transposed kSize x 1 kernel
  vector<double> gaussianKernelComponent = calcGaussianKernelComponent(kSize);

  //  horizontal first pass writes to temporary picture object
  for (size_t j = 0; j < height; j++) {
    for (size_t i = 0; i < width; i++) {
      double rSum = 0;
      double gSum = 0;
      double bSum = 0;

      for (int k = -kRadius; k <= kRadius; k++) {
        double weight = gaussianKernelComponent[k + kRadius];
        size_t pixel = mirrorPixel(i + k, width);

        rSum += weight * pic.red(pixel, j);
        gSum += weight * pic.green(pixel, j);
        bSum += weight * pic.blue(pixel, j);
      }

      tempPic.set(i, j, rSum, gSum, bSum);
    }
  }

  // vertical second pass writes directly to final picture object
  for (size_t j = 0; j < height; j++) {
    for (size_t i = 0; i < width; i++) {
      double rSum = 0;
      double gSum = 0;
      double bSum = 0;

      for (int k = -kRadius; k <= kRadius; k++) {
        double weight = gaussianKernelComponent[k + kRadius];
        size_t pixel = mirrorPixel(j + k, height);

        rSum += weight * tempPic.red(i, pixel);
        gSum += weight * tempPic.green(i, pixel);
        bSum += weight * tempPic.blue(i, pixel);
      }

      pic.set(i, j, rSum, gSum, bSum);
    }
  }
}


// SOBEL

// picture is grayscale
void ImageEditor::sobelFilter(const size_t strength) {
  const size_t width = pic.width();
  const size_t height = pic.height();

  //   const size_t kSize = strength % 2 ? strength : strength - 1;

  const array<int, 3> sobelFilterXComponent = {-1, 0, 1};
  const array<int, 3> sobelFilterYComponent = {1, 2, 1};
  const int kRadius = 1;

  // use vector of ints; intermediate values will be truncated at 255 with pic
  // object
  vector<vector<int>> buffY(height, vector<int>(width, 0));
  vector<vector<int>> buffX(height, vector<int>(width, 0));

  // First pass: horizontal filtering
  for (size_t j = 0; j < height; j++) {
    for (size_t i = 0; i < width; i++) {
      int xSum = 0;
      int ySum = 0;

      for (int k = -kRadius; k <= kRadius; k++) {
        size_t pixel = mirrorPixel(i + k, width);

        xSum += sobelFilterXComponent[k + kRadius] * pic.red(pixel, j);
        ySum += sobelFilterYComponent[k + kRadius] * pic.red(pixel, j);
      }

      buffX[j][i] = xSum;
      buffY[j][i] = ySum;
    }
  }

  // Second pass: vertical filtering
  for (size_t j = 0; j < height; j++) {
    for (size_t i = 0; i < width; i++) {
      int xSum = 0;
      int ySum = 0;

      for (int k = -kRadius; k <= kRadius; k++) {
        size_t pixel = mirrorPixel(j + k, height);

        xSum += sobelFilterYComponent[k + kRadius] * buffX[pixel][i];
        ySum += sobelFilterXComponent[k + kRadius] * buffY[pixel][i];
      }

      const double mag = sqrt(xSum * xSum + ySum * ySum);
      const size_t channel = clamp(mag, 255);

      pic.set(i, j, channel, channel, channel);
    }
  }
};

// vector<double> calcSobelKernelProduct(size_t size) {
//   vector<double> kernel(size, 0);
//   int radius = size / 2;

//   for (int i = -radius; i <= radius; i++) {
//     kernel[i + radius] = i;
//   }

//   return kernel;
// }
