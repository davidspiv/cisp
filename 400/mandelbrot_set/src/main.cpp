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
    // INPUT
    // separate from event loop to ensure escape key kills process
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
      window.close();
    }

    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
      }

      if (event.type == sf::Event::MouseMoved) {
        complexPlane.setMouseLocation(sf::Mouse::getPosition(window));
      }

      if (complexPlane.m_state != CALCULATING &&
          event.type == sf::Event::MouseButtonPressed) {

        sf::Vector2i mousePos = sf::Mouse::getPosition(window);

        if (event.mouseButton.button == sf::Mouse::Left) {
          complexPlane.setCenter(mousePos);
          complexPlane.zoomIn();

        } else if (event.mouseButton.button == sf::Mouse::Right) {
          complexPlane.setCenter(mousePos);
          complexPlane.zoomOut();
        }
      }
    }

    // UPDATE
    complexPlane.updateRender(threadCount);
    complexPlane.loadText(text);

    // DRAW
    window.clear();
    complexPlane.draw(window, states);
    window.draw(text);
    window.display();
  }
}
