#include "../include/ImageEditor.h"
#include "../include/util.h"

#include <algorithm>
#include <cmath>

unsigned char calcErrCorr(const unsigned char val, const double weight) {
  return std::clamp(val + static_cast<int>(weight), 0, 255);
};


void ImageEditor::dither() {
  int width = pic.width();
  int height = pic.height();

  //   Floyd–Steinberg
  for (int j = 0; j < height; j++) {
    for (int i = 0; i < width; i++) {
      unsigned char rCurr = pic.red(i, j);
      unsigned char gCurr = pic.green(i, j);
      unsigned char bCurr = pic.blue(i, j);

      unsigned char rQuant = quantizeChannel(rCurr);
      unsigned char gQuant = quantizeChannel(gCurr);
      unsigned char bQuant = quantizeChannel(bCurr);

      // set the pixels as we go
      pic.set(i, j, rQuant, gQuant, bQuant);

      // Compute quantization error
      int rErr = rCurr - rQuant;
      int gErr = gCurr - gQuant;
      int bErr = bCurr - bQuant;

      // down - weighted with 43.75% of the error
      if (j + 1 < height) {
        const int rNew = calcErrCorr(pic.red(i, j + 1), rErr * 0.4375);
        const int gNew = calcErrCorr(pic.green(i, j + 1), gErr * 0.4375);
        const int bNew = calcErrCorr(pic.blue(i, j + 1), bErr * 0.4375);

        pic.set(i, j + 1, rNew, gNew, bNew);
      }

      // down-left - weighted with 18.75% of the error
      if (i - 1 >= 0 && j + 1 < height) {
        const int rNew = calcErrCorr(pic.red(i - 1, j + 1), rErr * 0.1875);
        const int gNew = calcErrCorr(pic.green(i - 1, j + 1), gErr * 0.1875);
        const int bNew = calcErrCorr(pic.blue(i - 1, j + 1), bErr * 0.1875);

        pic.set(i - 1, j + 1, rNew, gNew, bNew);
      }

      // right - weighted with 31.25%  of the error
      if (i + 1 < width) {
        const int rNew = calcErrCorr(pic.red(i + 1, j), rErr * 0.3125);
        const int gNew = calcErrCorr(pic.green(i + 1, j), gErr * 0.3125);
        const int bNew = calcErrCorr(pic.blue(i + 1, j), bErr * 0.3125);

        pic.set(i + 1, j, rNew, gNew, bNew);
      }

      // down-right - weighted with 6.25% of the error
      if (i + 1 < width && j + 1 < height) {
        const int rNew = calcErrCorr(pic.red(i + 1, j + 1), rErr * 0.0625);
        const int gNew = calcErrCorr(pic.green(i + 1, j + 1), gErr * 0.0625);
        const int bNew = calcErrCorr(pic.blue(i + 1, j + 1), bErr * 0.0625);

        pic.set(i + 1, j + 1, rNew, gNew, bNew);
      }
    }
  }
}
