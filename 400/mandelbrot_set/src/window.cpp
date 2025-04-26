#include "../include/window.h"

#include <SFML/Graphics.hpp>
#include <iostream>

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
