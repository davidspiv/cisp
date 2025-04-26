#include <SFML/Graphics.hpp>
#include <iostream>

#include "../include/ComplexPlane.h"

constexpr int TARGET_FPS = 60;
constexpr int ANTIALIAS_LEVEL = 8;
constexpr char WINDOW_TITLE[] = "Mandelbrot Set";

void setup_window(sf::RenderWindow &window, const int width, const int height) {
  sf::ContextSettings settings;
  settings.antialiasingLevel = ANTIALIAS_LEVEL;

  window.create(sf::VideoMode(width, height), WINDOW_TITLE, sf::Style::Default,
                settings);

  if (!window.isOpen()) {
    throw std::runtime_error("Failed to create SFML window\n");
    return;
  }

  std::cout << "Anti-Aliasing: "
            << (window.getSettings().antialiasingLevel ? "ON" : "OFF") << "\n";

  auto desktop = sf::VideoMode::getDesktopMode();
  window.setPosition({static_cast<int>(desktop.width / 2 - width / 2),
                      static_cast<int>(desktop.height / 2 - height / 2)});

  window.setFramerateLimit(TARGET_FPS);
}

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

      sf::Vector2i mousePos = sf::Mouse::getPosition();

      if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right)) {
        complexPlane.zoomOut();
        complexPlane.setCenter(mousePos);
      }

      if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
        complexPlane.zoomIn();
        complexPlane.setCenter(mousePos);
      }

      if (event.type == sf::Event::MouseMoved) {
        complexPlane.setMouseLocation(mousePos);
      }
    }

    // Update
    complexPlane.updateRender();
    // complexPlane.loadText(text);

    // Draw
    window.clear();
    complexPlane.draw(window, states);
    // window.draw(text);
    window.display();
  }
}
