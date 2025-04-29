#pragma once

#include "window.h"

#include <SFML/Graphics.hpp>
#include <complex>
#include <sstream>

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
  void updateRender(const int threadCount);

private:
  void zoom();
  sf::Vector2f mapPixelToCoords(sf::Vector2i mousePixel);
  int countIterations(sf::Vector2f coord);
  void iterationsToRGB(size_t count, u_int8_t &r, u_int8_t &g, u_int8_t &b);
  void calcChunk(const int rowStart, const int rowsToCalc);
};
