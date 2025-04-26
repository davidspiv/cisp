#include <SFML/Graphics.hpp>
#include <iostream>

#include "../include/ComplexPlane.h"

constexpr int SCREEN_WIDTH = 1920 / 4.0;
constexpr int SCREEN_HEIGHT = 1080 / 4.0;
constexpr int TARGET_FPS = 60;
constexpr int ANTIALIAS_LEVEL = 8;
constexpr char WINDOW_TITLE[] = "Template Demo";

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
    }

    // Update

    // Draw
    window.clear();
    // window.draw(...);
    window.display();
  }
}
