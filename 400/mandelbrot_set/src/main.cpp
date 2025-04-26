#include <SFML/Graphics.hpp>
#include <iostream>

#include "../include/ComplexPlane.h"
#include "../include/window.h"

int main() {
  sf::RenderWindow window;
  setup_window(window, SCREEN_WIDTH, SCREEN_HEIGHT);
  sf::RenderStates states;

  ComplexPlane complexPlane(SCREEN_WIDTH, SCREEN_HEIGHT);
  sf::Font font;
  sf::Text text;

  while (window.isOpen()) {
    sf::Event event;

    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed ||
          sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
        window.close();
      }

      sf::Vector2i mousePos = sf::Mouse::getPosition(window);

      if (complexPlane.m_state != CALCULATING &&
          sf::Mouse::isButtonPressed(sf::Mouse::Button::Right)) {
        complexPlane.zoomOut();
        complexPlane.setCenter(mousePos);
      }

      if (complexPlane.m_state != CALCULATING &&
          sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
        complexPlane.zoomIn();
        complexPlane.setCenter(mousePos);
      }

      if (event.type == sf::Event::MouseMoved) {
        complexPlane.setMouseLocation(mousePos);
      }
    }

    // Update
    complexPlane.updateRender();
    complexPlane.loadText(text);

    // Draw
    window.clear();
    complexPlane.draw(window, states);
    window.draw(text);
    window.display();
  }
}
