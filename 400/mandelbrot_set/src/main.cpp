#include "../include/ComplexPlane.h"
#include "../include/window.h"

#include <SFML/Graphics.hpp>
#include <iostream>
#include <thread>

int main() {
  sf::RenderWindow window;
  setup_window(window, SCREEN_WIDTH, SCREEN_HEIGHT);
  sf::RenderStates states;

  unsigned int threadCount = std::thread::hardware_concurrency();
  std::cout << "Number of concurrent threads supported: " << threadCount
            << std::endl;

  ComplexPlane complexPlane(SCREEN_WIDTH, SCREEN_HEIGHT);
  sf::Font font;
  sf::Text text;

  while (window.isOpen()) {
    // Input
    sf::Event event;

    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed ||
          sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
        window.close();
      }

      sf::Vector2i mousePos = sf::Mouse::getPosition(window);

      if (event.type == sf::Event::MouseMoved) {
        complexPlane.setMouseLocation(mousePos);
      }

      if (complexPlane.m_state != CALCULATING &&
          sf::Mouse::isButtonPressed(sf::Mouse::Button::Right)) {
        complexPlane.setCenter(mousePos);
        complexPlane.zoomOut();
      }

      if (complexPlane.m_state != CALCULATING &&
          sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
        complexPlane.setCenter(mousePos);
        complexPlane.zoomIn();
      }
    }

    // Update
    complexPlane.updateRender(threadCount);
    complexPlane.loadText(text);

    // Draw
    window.clear();
    complexPlane.draw(window, states);
    window.draw(text);
    window.display();
  }
}
