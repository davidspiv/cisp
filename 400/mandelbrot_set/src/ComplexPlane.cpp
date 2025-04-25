#include "../include/ComplexPlane.h"

#include <SFML/Graphics.hpp>

ComplexPlane::ComplexPlane(int pixelWidth, int pixelHeight) {};
void ComplexPlane::draw(sf::RenderTarget& target,
                        sf::RenderStates states) const {};
void ComplexPlane::zoomln() {};
void ComplexPlane::zoomOut() {};
void ComplexPlane::setCenter(sf::Vector2i mousePixel) {};
void ComplexPlane::setMouseLocation(sf::Vector2i mousePixel) {};
void ComplexPlane::loadText(sf::Text& text) {};
void ComplexPlane::updateRender() {};
int ComplexPlane::countIterations(sf::Vector2f coord) {};
void ComplexPlane::iterationsToRGB(size_t count, u_int8_t& r, u_int8_t& g,
                                   u_int8_t& b) {};
sf::Vector2f ComplexPlane::mapPixelToCoords(sf::Vector2i mousePixel) {};
