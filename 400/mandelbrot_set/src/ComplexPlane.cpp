#include "../include/ComplexPlane.h"
#include "../include/Color.h"
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


std::vector<Color_Space::Rgb> generateRainbowColors(int sampleCount) {
  std::vector<Color_Space::Rgb> colors;
  colors.reserve(sampleCount);

  for (int i = 0; i < sampleCount; ++i) {
    float hue = (360.0f * i) / sampleCount; // hue from 0 to just under 360

    Color_Space::Lch_Ab Lch_ab(.5f, .5f, hue);

    Color_Space::Rgb rgb = Lch_ab.to_lab().to_xyz().to_rgb();

    colors.push_back(rgb);
  }

  return colors;
}


// Map the given iteration count to an r,g,b color
void ComplexPlane::iterationsToRGB(size_t count, u_int8_t &r, u_int8_t &g,
                                   u_int8_t &b) {
  if (count == MAX_ITER) {
    r = g = b = 0;
    return;
  }

  const static std::vector<Color_Space::Rgb> colors =
      generateRainbowColors(MAX_ITER);

  auto [color_r, color_g, color_b] = colors[count].get_values();

  r = color_r;
  g = color_g;
  b = color_b;
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
