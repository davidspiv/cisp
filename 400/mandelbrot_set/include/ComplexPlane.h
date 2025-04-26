#pragma once

#include <SFML/Graphics.hpp>

#include <complex>
#include <sstream>

constexpr int SCREEN_WIDTH = 1920 / 2.0;
constexpr int SCREEN_HEIGHT = 1080 / 2.0;

const unsigned int MAX_ITER = 64;
const float BASE_WIDTH = 4.0;
const float BASE_HEIGHT = 4.0;
const float BASE_ZOOM = 0.5;

enum State { CALCULATING, DISPLAYING };

class ComplexPlane : public sf::Drawable {
private:
  const sf::Vector2i m_pixel_size;
  const float m_aspectRatio;
  sf::Vector2f m_plane_center;
  sf::Vector2f m_plane_size;
  int m_zoomCount;
  State m_state;
  sf::VertexArray m_vArray;
  sf::Vector2f m_mouseLocation;

public:
  ComplexPlane(int pixelWidth, int pixelHeight);

  void draw(sf::RenderTarget &target, sf::RenderStates states) const;
  void zoomIn();
  void zoomOut();
  void setCenter(sf::Vector2i mousePixel);
  void setMouseLocation(sf::Vector2i mousePixel);
  void loadText(sf::Text &text);
  void updateRender();

private:
  sf::Vector2f mapPixelToCoords(sf::Vector2i mousePixel);
  int countIterations(sf::Vector2f coord);
  void iterationsToRGB(size_t count, u_int8_t &r, u_int8_t &g, u_int8_t &b);
  void zoom();
};


ComplexPlane::ComplexPlane(int pixelWidth, int pixelHeight)
    : m_pixel_size(sf::Vector2i(pixelWidth, pixelHeight)),
      m_aspectRatio((double)pixelWidth / pixelHeight),
      m_plane_center(sf::Vector2f(0.f, 0.f)),
      m_plane_size(BASE_WIDTH, BASE_HEIGHT * m_aspectRatio), m_zoomCount(0),
      m_state(CALCULATING),
      m_vArray(sf::VertexArray(sf::PrimitiveType::Points,
                               pixelWidth * pixelHeight)) {};


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


void ComplexPlane::loadText(sf::Text &text) {};


void ComplexPlane::updateRender() {
  if (m_state == DISPLAYING)
    return;

  for (size_t j = 0; j < SCREEN_WIDTH; j++) {
    for (size_t i = 0; i < SCREEN_HEIGHT; i++) {
      m_vArray[j + i * SCREEN_WIDTH].position = {(float)j, (float)i};

      const sf::Vector2f complexCoord =
          mapPixelToCoords(sf::Vector2i((int)j, (int)i));

      const int iterations = countIterations(complexCoord);

      u_int8_t r;
      u_int8_t g;
      u_int8_t b;

      iterationsToRGB(iterations, r, g, b);

      m_vArray[j + i * SCREEN_WIDTH].color = {r, g, b};
      m_state = DISPLAYING;
    }
  }
};


int ComplexPlane::countIterations(sf::Vector2f coord) {
  std::complex<double> c = {coord.x, coord.y};
  std::complex<double> z = c;
  int i = 0;
  while (abs(z) < 2.0 && i < 64) {
    z = z * z + c;
    i++;
  }

  return i;
};


// Map the given iteration count to an r,g,b color
void ComplexPlane::iterationsToRGB(size_t count, u_int8_t &r, u_int8_t &g,
                                   u_int8_t &b) {
  if (count == MAX_ITER) {
    r = g = b = 0;
  }
  r = g = b = static_cast<u_int8_t>(255.0 * count / MAX_ITER);
}


sf::Vector2f ComplexPlane::mapPixelToCoords(sf::Vector2i mousePixel) {

  auto mapNumericRange = [](int n, int a, int b, float c, float d) {
    return ((float)(n - a) / (b - a)) * (d - c) + c;
  };

  const float x = mapNumericRange(mousePixel.x, 0, SCREEN_WIDTH, -2, 2);
  const float y = mapNumericRange(mousePixel.y, 0, SCREEN_HEIGHT, -2, 2);

  return sf::Vector2f(x, y);
};
