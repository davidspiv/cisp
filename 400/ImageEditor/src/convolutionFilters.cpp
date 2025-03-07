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
      double rWeightedAvg = 0;
      double gWeightedAvg = 0;
      double bWeightedAvg = 0;

      for (int k = -kRadius; k <= kRadius; k++) {
        const double weight = gaussianKernelComponent[k + kRadius];
        const size_t pixel = mirrorPixel(i + k, width);

        rWeightedAvg += weight * pic.red(pixel, j);
        gWeightedAvg += weight * pic.green(pixel, j);
        bWeightedAvg += weight * pic.blue(pixel, j);
      }

      tempPic.set(i, j, rWeightedAvg, gWeightedAvg, bWeightedAvg);
    }
  }

  // vertical second pass writes directly to final picture object
  for (size_t j = 0; j < height; j++) {
    for (size_t i = 0; i < width; i++) {
      double rWeightedAvg = 0;
      double gWeightedAvg = 0;
      double bWeightedAvg = 0;

      for (int k = -kRadius; k <= kRadius; k++) {
        const double weight = gaussianKernelComponent[k + kRadius];
        const size_t pixel = mirrorPixel(j + k, height);

        rWeightedAvg += weight * tempPic.red(i, pixel);
        gWeightedAvg += weight * tempPic.green(i, pixel);
        bWeightedAvg += weight * tempPic.blue(i, pixel);
      }

      pic.set(i, j, rWeightedAvg, gWeightedAvg, bWeightedAvg);
    }
  }
}


// SOBEL

// picture is grayscale
void ImageEditor::sobelOperator() {
  const size_t width = pic.width();
  const size_t height = pic.height();

  const array<double, 3> sobelKernalProductA = {-.5, 0, .5};
  const array<double, 3> sobelKernalProductB = {.5, 1, .5};

  // using pic object as buffer results in significant truncation of
  // intermediate values, use int vector instead
  vector<vector<int>> yGrads(height, vector<int>(width, 0));
  vector<vector<int>> xGrads(height, vector<int>(width, 0));

  //   vector<vector<int>> slopes(height, vector<int>(width, 0));

  // First pass: horizontal filtering
  for (size_t j = 0; j < height; j++) {
    for (size_t i = 0; i < width; i++) {
      int xGrad = 0;
      int yGrad = 0;

      for (int k = -1; k <= 1; k++) {
        size_t pixel = mirrorPixel(i + k, width);

        xGrad += sobelKernalProductA[k + 1] * pic.red(pixel, j);
        yGrad += sobelKernalProductB[k + 1] * pic.red(pixel, j);
      }

      xGrads[j][i] = xGrad;
      yGrads[j][i] = yGrad;
    }
  }

  // Second pass: vertical filtering
  for (size_t j = 0; j < height; j++) {
    for (size_t i = 0; i < width; i++) {
      double xGrad = 0;
      double yGrad = 0;

      for (int k = -1; k <= 1; k++) {
        size_t pixel = mirrorPixel(j + k, height);

        xGrad += sobelKernalProductB[k + 1] * xGrads[pixel][i];
        yGrad += sobelKernalProductA[k + 1] * yGrads[pixel][i];
      }

      // pythagorean addition
      const double gradMagnitude = sqrt(xGrad * xGrad + yGrad * yGrad);
      const double slope = std::atan2(yGrad, xGrad);

      // Normalize slope to [0,1] then scale to [0,255] for rbg
      const double normSlope =
          std::clamp((slope + M_PI) / (2 * M_PI) * 255.0, 0.0, 255.0);

      const uint8_t c = gradMagnitude > 75 ? normSlope : 0;

      //   slopes[j][i] = c

      pic.set(i, j, c, c, c);
    }
  }
};
