#include "../include/ComplexPlane.h"

#include <SFML/Graphics.hpp>

ComplexPlane::ComplexPlane(int pixelWidth, int pixelHeight)
    : m_vArray(
          sf::VertexArray(sf::PrimitiveType::Points, pixelWidth * pixelHeight)),
      m_pixel_size(sf::Vector2i(pixelWidth, pixelHeight)),
      m_aspectRatio((double)pixelWidth / (double)pixelHeight),
      m_plane_center(sf::Vector2f(0.f, 0.f)),
      m_plane_size(BASE_WIDTH, BASE_HEIGHT * m_aspectRatio), m_zoomCount(0),
      m_state(State::CALCULATING) {};


void ComplexPlane::draw(sf::RenderTarget &target,
                        sf::RenderStates states) const {
  target.draw(m_vArray);
};


void ComplexPlane::zoomln() {};


void ComplexPlane::zoomOut() {};


void ComplexPlane::setCenter(sf::Vector2i mousePixel) {};


void ComplexPlane::setMouseLocation(sf::Vector2i mousePixel) {};


void ComplexPlane::loadText(sf::Text &text) {};


void ComplexPlane::updateRender() {
  const size_t screenWidth = m_plane_size.x;
  const size_t screenHeight = m_plane_size.y;

  for (size_t j = 0; j < screenHeight; j++) {
    for (size_t i = 0; i < screenWidth; i++) {
      m_vArray[j + i * screenWidth].position = {(float)j, (float)i};
    }
  }
};


int ComplexPlane::countIterations(sf::Vector2f coord) {};


void ComplexPlane::iterationsToRGB(size_t count, u_int8_t &r, u_int8_t &g,
                                   u_int8_t &b) {};


sf::Vector2f ComplexPlane::mapPixelToCoords(sf::Vector2i mousePixel) {};
