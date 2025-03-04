#include "../include/ImageEditor.h"

#include <array>
#include <cmath>
#include <fstream>
#include <sstream>
#include <string>

#include "../include/util.h"
using namespace std;

ImageEditor::ImageEditor(const string& inFileName) : pic(Picture(inFileName)) {}

void ImageEditor::save(const string& outFileName) { pic.save(outFileName); }

// (image negative)
ImageEditor& ImageEditor::operator-() {
  size_t width = pic.width();
  size_t height = pic.height();
  for (size_t j = 0; j < height; j++) {
    for (size_t i = 0; i < width; i++) {
      int red = 255 - pic.red(i, j);
      int green = 255 - pic.green(i, j);
      int blue = 255 - pic.blue(i, j);
      pic.set(i, j, red, green, blue);
    }
  }

  return *this;
};

// add c to all
ImageEditor& ImageEditor::operator+=(const Color& c) {
  size_t width = pic.width();
  size_t height = pic.height();
  for (size_t j = 0; j < height; j++) {
    for (size_t i = 0; i < width; i++) {
      int red = pic.red(i, j) + c.r;
      int green = pic.green(i, j) + c.g;
      int blue = pic.blue(i, j) + c.b;

      if (red < 0) {
        red = 0;
      } else if (red > 255) {
        red = 255;
      }

      if (green < 0) {
        green = 0;
      } else if (green > 255) {
        green = 255;
      }

      if (blue < 0) {
        blue = 0;
      } else if (blue > 255) {
        blue = 255;
      }

      pic.set(i, j, red, green, blue);
    }
  }

  return *this;
};

// subtract c from all pixels
ImageEditor& ImageEditor::operator-=(const Color& c) {
  return *this += {-c.r, -c.g, -c.b};
};

// compare to another image
bool ImageEditor::operator==(const ImageEditor& ie) const {
  size_t width = pic.width();
  size_t height = pic.height();

  for (size_t j = 0; j < height; j++) {
    for (size_t i = 0; i < width; i++) {
      if (ie.pic.red(i, j) != pic.red(i, j)) return false;
      if (ie.pic.green(i, j) != pic.green(i, j)) return false;
      if (ie.pic.blue(i, j) != pic.blue(i, j)) return false;
    }
  }
  return true;
};

// compare to another image
bool ImageEditor::operator!=(const ImageEditor& ie) const {
  return !(*this == ie);
};

// expand by factor of n by n
ImageEditor& ImageEditor::operator*=(unsigned int n) {
  if (n < 1 || n > 10) throw runtime_error("N must be from 1 to 10");

  Picture picOut(pic.width() * n, pic.height() * n);
  size_t width = pic.width();
  size_t height = pic.height();

  for (size_t j = 0; j < width; j++) {
    for (size_t i = 0; i < height; i++) {
      int red = pic.red(i, j);
      int green = pic.green(i, j);
      int blue = pic.blue(i, j);

      for (size_t k = 0; k < n; k++) {
        for (size_t l = 0; l < n; l++) {
          picOut.set(i * n + k, j * n + l, red, green, blue);
        }
      }
    }
  }

  pic = picOut;

  return *this;
}

void ImageEditor::grayscaleViaLuminance() { pic = pic.grays(); }

void ImageEditor::grayscaleViaLightness() {
  size_t width = pic.width();
  size_t height = pic.height();

  for (size_t j = 0; j < height; j++) {
    for (size_t i = 0; i < width; i++) {
      Color c = {pic.red(i, j), pic.green(i, j), pic.blue(i, j)};
      const double lightness = calcLightness(c);
      const int grayComponent = scaleValue(lightness, 100, 255);
      pic.set(i, j, grayComponent, grayComponent, grayComponent);
    }
  }
};

void ImageEditor::gaussianBlur(size_t kSize) {
  Picture tempPic = pic;
  const size_t width = pic.width();
  const size_t height = pic.height();

  // kSize will be rounded up to an odd number to keep target pixel centered
  if (!(kSize % 2)) kSize--;

  // half the matrix not including the center
  const int kRadius = kSize / 2;

  // a gaussian kernel is separable which means we can use it's products to
  // perform two separate convolutions on the picture. We then multiply the
  // results of both operations together to get an equivalent result, saving a
  // 4th inner loop. We are sampling kSize * kSize pixels to create each pixel
  // in the target image; this optimization is essential for larger photos

  // used both as an 1 x kSize kernel and a transposed kSize x 1 kernel
  vector<double> gaussianProduct = calcGaussianKernelProduct(kSize);

  //  horizontal first pass writes to temporary picture object
  for (size_t j = 0; j < height; j++) {
    for (size_t i = 0; i < width; i++) {
      double rSum = 0;
      double gSum = 0;
      double bSum = 0;

      for (int k = -kRadius; k <= kRadius; k++) {
        double weight = gaussianProduct[k + kRadius];
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
        double weight = gaussianProduct[k + kRadius];
        size_t pixel = mirrorPixel(j + k, height);

        rSum += weight * tempPic.red(i, pixel);
        gSum += weight * tempPic.green(i, pixel);
        bSum += weight * tempPic.blue(i, pixel);
      }

      pic.set(i, j, rSum, gSum, bSum);
    }
  }
}

// refactor like gaussianBlur
void ImageEditor::sobelFilter() {
  const size_t width = pic.width();
  const size_t height = pic.height();
  Picture newPic = pic;

  const array<array<int, 3>, 3> gX = {{{1, 0, -1}, {2, 0, -2}, {1, 0, -1}}};
  const array<array<int, 3>, 3> gY = {{{1, 2, 1}, {0, 0, 0}, {-1, -2, -1}}};
  const int kRadius = 1;

  for (size_t j = kRadius; j < height - kRadius; j++) {
    for (size_t i = kRadius; i < width - kRadius; i++) {
      if (pic.red(i, j) != pic.green(i, j) || pic.red(i, j) != pic.blue(i, j)) {
        throw invalid_argument("non-grayscale pixel detected");
      }

      double xSum = 0;
      double ySum = 0;

      for (int l = -kRadius; l <= kRadius; l++) {
        for (int k = -kRadius; k <= kRadius; k++) {
          const int xWeight = gX[l + kRadius][k + kRadius];
          const int yWeight = gY[l + kRadius][k + kRadius];

          xSum += xWeight * pic.red(i + k, j + l);
          ySum += yWeight * pic.red(i + k, j + l);
        }
      }

      const double mag = sqrt(xSum * xSum + ySum * ySum);
      const size_t grey = clamp(mag, 255);

      newPic.set(i, j, grey, grey, grey);
    }
  }

  this->pic = newPic;
};

void ImageEditor::ascii(const string& outFileName) {
  const string asciiSorted =
      "$@B%8&WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/"
      "\\|()1{}[]?-_+~<>i!lI;:,\"^`'. ";

  stringstream ss;
  size_t width = pic.width();
  size_t height = pic.height();

  for (size_t j = 0; j < height; j++) {
    double pixSum = 0;
    int pixNum = 0;

    for (size_t i = 0; i < width; i++) {
      if (pic.red(i, j) != pic.green(i, j) || pic.red(i, j) != pic.blue(i, j)) {
        throw invalid_argument("non-grayscale pixel detected");
      }

      pixSum += pic.red(i, j);
      pixNum++;

      if (!(j % 6) && !(i % 3)) {
        const double avg = pixSum / pixNum;
        ss << asciiSorted[scaleValue(avg, 255, asciiSorted.length() - 1)];
        pixSum = pixNum = 0;
      }
    }

    if (!(j % 6)) {
      ss << endl;
    }
  }

  ofstream out;
  out.open(outFileName);
  out << ss.str();
  out.close();
}

void ImageEditor::createTestImage(const string& outFileName) {
  size_t width = pic.width();
  size_t height = pic.height();
  bool isBlack = 1;

  for (size_t j = 0; j < height; j++) {
    for (size_t i = 0; i < width; i++) {
      if (isBlack) {
        pic.set(i, j, 230, 230, 230);
      } else {
        pic.set(i, j, 20, 20, 20);
      }
      if (!(i % 10)) {
        isBlack = !isBlack;
      }
    }
  }

  pic.save("testImage.PNG");
};
