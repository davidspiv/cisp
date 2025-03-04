#include "../include/ImageEditor.h"
#include "../include/util.h"
#include <fstream>
#include <sstream>

void ImageEditor::ascii(const string &outFileName) {
  const string asciiSorted = "$@B%8&WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/"
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
