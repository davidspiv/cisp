#include "../include/picture.h"
#include <cstring>

void Picture::Scale(const size_t factor) {

  const size_t channels = 4;

  const size_t height = static_cast<size_t>(_height);
  const size_t width = static_cast<size_t>(_width);

  const size_t dstWidth = _width * factor * channels;
  const size_t dstHeight = _height * factor * channels;

  std::vector<unsigned char> newValues(_values.size() * factor * factor);

  for (size_t j = 0; j < height; j++) {
    const size_t srcRowIdx = j * width * channels;
    const size_t dstRowIdx = j * factor * dstWidth;

    // Create a temporary row that we can copy multiple times
    std::vector<unsigned char> tempRow(dstWidth);

    // Build first row 'manually'
    for (size_t i = 0; i < width; i++) {
      const size_t srcPixelIdx = srcRowIdx + i * channels;
      const size_t dstPixelIdx = i * factor * channels;

      // Copy the pixel "factor" times
      for (size_t k = 0; k < factor; k++) {
        std::memcpy(tempRow.data() + dstPixelIdx + k * channels,
                    _values.data() + srcPixelIdx, channels);
      }
    }

    // Copy the temporary row "factor" times
    for (size_t x = 0; x < factor; x++) {
      std::memcpy(newValues.data() + dstRowIdx + x * dstWidth, tempRow.data(),
                  tempRow.size());
    }
  }

  _width = dstWidth / channels;
  _height = dstHeight / channels;
  _values.swap(newValues);
}
