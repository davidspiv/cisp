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
  sf::VertexArray m_vArray;
  sf::Vector2f m_mouseLocation;

public:
  State m_state;

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
  text.setColor(sf::Color::Red);
  text.setString(ss.str());
}


void ComplexPlane::updateRender() {
  if (m_state == DISPLAYING)
    return;

  for (int j = 0; j < m_pixel_size.x; j++) {
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
  m_state = DISPLAYING;
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


// Map the given iteration count to an r,g,b color
void ComplexPlane::iterationsToRGB(size_t count, u_int8_t &r, u_int8_t &g,
                                   u_int8_t &b) {
  if (count == MAX_ITER) {
    r = g = b = 0;
  }
  r = g = b = static_cast<u_int8_t>(255.0 * count / MAX_ITER);
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
