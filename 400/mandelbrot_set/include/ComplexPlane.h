#pragma once

#include <SFML/Graphics.hpp>

const unsigned int MAX_ITER = 64;
const float BASE_WIDTH = 4.0;
const float BASE_HEIGHT = 4.0;
const float BASE_ZOOM = 0.5;

const enum State { CALCULATING, DISPLAYING };

class ComplexPlane {
 private:
  sf::VertexArray m_vArray;
  State m_state;
  sf::Vector2f m_mouseLocation;
  sf::Vector2i m_pixel_size;
  sf::Vector2f m_plane_center;
  sf::Vector2f m_plane_size;
  int m_zoomCount;
  float m_aspectRatio;

 public:
  ComplexPlane(int pixelWidth, int pixelHeight);
  void draw(sf::RenderTarget& target, sf::RenderStates states) const;
  void zoomln();
  void zoomOut();
  void setCenter(sf::Vector2i mousePixel);
  void setMouseLocation(sf::Vector2i mousePixel);
  void loadText(sf::Text& text);
  void updateRender();
  int countIterations(sf::Vector2f coord);
  void iterationsToRGB(size_t count, u_int8_t& r, u_int8_t& g, u_int8_t& b);
  sf::Vector2f mapPixelToCoords(sf::Vector2i mousePixel);
};
