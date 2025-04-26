#include "../include/ComplexPlane.h"
#include "../include/timer.h"
#include "../include/window.h"

#include <cmath>
#include <iostream>
#include <thread>

ComplexPlane::ComplexPlane(int pixelWidth, int pixelHeight)
    : m_pixel_size(sf::Vector2i(pixelWidth, pixelHeight)),
      m_aspectRatio((double)pixelHeight / pixelWidth),
      m_plane_center(sf::Vector2f(0.f, 0.f)),
      m_plane_size(BASE_WIDTH, BASE_HEIGHT * m_aspectRatio), m_zoomCount(0),
      m_vArray(
          sf::VertexArray(sf::PrimitiveType::Points, pixelWidth * pixelHeight)),
      m_state(CALCULATING) {};


void ComplexPlane::draw(sf::RenderTarget &target,
                        sf::RenderStates states) const {
  target.draw(m_vArray);
};


void ComplexPlane::zoom() {
  const float scalar = std::pow(BASE_ZOOM, m_zoomCount);

  const float x = BASE_WIDTH * scalar;
  const float y = BASE_HEIGHT * m_aspectRatio * scalar;

  m_plane_size = sf::Vector2f(x, y);
  m_state = CALCULATING;
}


void ComplexPlane::zoomIn() {
  ++m_zoomCount;
  zoom();
};


void ComplexPlane::zoomOut() {
  --m_zoomCount;
  zoom();
};


void ComplexPlane::setCenter(sf::Vector2i mousePixel) {
  m_plane_center = mapPixelToCoords(mousePixel);
  m_state = CALCULATING;
};


void ComplexPlane::setMouseLocation(sf::Vector2i mousePixel) {
  m_mouseLocation = mapPixelToCoords(mousePixel);
};


void ComplexPlane::loadText(sf::Text &text) {
  static sf::Font font;
  if (!font.loadFromFile("Cascadia.ttf")) {
    throw std::runtime_error("unable to open font file");
  }

  std::stringstream ss;
  ss << "Mandelbrot Set\nCenter: (" << m_plane_center.x << ','
     << m_plane_center.y << ")\n"
     << "Cursor: (" << m_mouseLocation.x << ',' << m_mouseLocation.y << ")\n"
     << "Left-click to Zoom in\nRight-click to Zoom out";

  text.setFont(font);
  text.setCharacterSize(24); // Pixels
  text.setFillColor(sf::Color(255, 255, 255));
  text.setString(ss.str());
}


void ComplexPlane::updateRender(const int threadCount) {
  if (m_state == DISPLAYING)
    return;

  //   Timer timer;

  const int chunkSize = std::ceil((float)m_pixel_size.x / threadCount);

  std::vector<std::thread> threads;
  threads.resize(threadCount);

  for (int i = 0; i < m_pixel_size.x; i += chunkSize) {
    int actualChunkSize =
        (chunkSize < m_pixel_size.x - i) ? chunkSize : m_pixel_size.x - i;

    threads.emplace_back(
        [i, actualChunkSize, this]() { calcChunk(i, actualChunkSize); });
  }

  // Join all threads
  for (auto &t : threads) {
    if (t.joinable()) {
      t.join();
    }
  }

  m_state = DISPLAYING;
}


sf::Vector2f ComplexPlane::mapPixelToCoords(sf::Vector2i mousePixel) {

  const float xMag = m_plane_size.x;
  const float yMag = m_plane_size.y;

  const float xOffset = (m_plane_center.x - m_plane_size.x / 2.0);
  const float yOffset = (m_plane_center.y - m_plane_size.y / 2.0);

  const float x = ((float)(mousePixel.x) / (m_pixel_size.x)) * xMag + xOffset;
  const float y =
      (1.0f - ((float)(mousePixel.y) / SCREEN_HEIGHT)) * yMag + yOffset; // flip

  return sf::Vector2f(x, y);
};


int ComplexPlane::countIterations(sf::Vector2f coord) {
  std::complex<double> c = {coord.x, coord.y};
  std::complex<double> z = c;
  int i = 0;
  while (abs(z) < 2.0 && i < int(MAX_ITER)) {
    z = z * z + c;
    i++;
  }

  return i;
};

struct RGB {
  float r, g, b;
};

RGB hsvToRgb(float h, float s, float v) {
  float c = v * s;
  float x = c * (1 - std::fabs(std::fmod(h / 60.0f, 2) - 1));
  float m = v - c;

  float r, g, b;

  if (h < 60) {
    r = c;
    g = x;
    b = 0;
  } else if (h < 120) {
    r = x;
    g = c;
    b = 0;
  } else if (h < 180) {
    r = 0;
    g = c;
    b = x;
  } else if (h < 240) {
    r = 0;
    g = x;
    b = c;
  } else if (h < 300) {
    r = x;
    g = 0;
    b = c;
  } else {
    r = c;
    g = 0;
    b = x;
  }

  return {r + m, g + m, b + m};
}


std::vector<RGB> generateRainbowColors(int sampleCount) {
  std::vector<RGB> colors;
  colors.reserve(sampleCount);

  for (int i = 0; i < sampleCount; ++i) {
    float hue = (360.0f * i) / sampleCount; // hue from 0 to just under 360
    RGB color = hsvToRgb(hue, 1.0f, 1.0f);  // full saturation and brightness
    colors.push_back(color);
  }

  return colors;
}


float apply_gamma(const float c) {
  if (c <= 0) {
    return c;
  }

  return (c <= 0.0031308f) ? (c * 12.92f)
                           : 1.055f * std::pow(c, 1.0f / 2.4f) - 0.055f;
}


// Map the given iteration count to an r,g,b color
void ComplexPlane::iterationsToRGB(size_t count, u_int8_t &r, u_int8_t &g,
                                   u_int8_t &b) {
  const static std::vector<RGB> colors = generateRainbowColors(MAX_ITER);

  if (count == MAX_ITER) {
    r = g = b = 0;
    return;
  }

  RGB color = colors[count];

  //   const float r_corr = apply_gamma(color.r);
  //   const float g_corr = apply_gamma(color.g);
  //   const float b_corr = apply_gamma(color.b);

  r = std::clamp(color.r, 0.0f, 1.0f) * 255.0f;
  g = std::clamp(color.g, 0.0f, 1.0f) * 255.0f;
  b = std::clamp(color.b, 0.0f, 1.0f) * 255.0f;
}


void ComplexPlane::calcChunk(const int rowStart, const int rowsToCalc) {
  for (int j = rowStart; j < rowStart + rowsToCalc; j++) {
    for (int i = 0; i < m_pixel_size.y; i++) {
      m_vArray[j + i * m_pixel_size.x].position = {(float)j, (float)i};

      const sf::Vector2f complexCoord = mapPixelToCoords(sf::Vector2i(j, i));

      const int iterations = countIterations(complexCoord);

      u_int8_t r;
      u_int8_t g;
      u_int8_t b;

      iterationsToRGB(iterations, r, g, b);

      m_vArray[j + i * m_pixel_size.x].color = {r, g, b};
    }
  }
}
