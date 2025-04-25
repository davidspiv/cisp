#include <SFML/Graphics.hpp>
#include <iostream>

constexpr int SCREEN_WIDTH = 1920;
constexpr int SCREEN_HEIGHT = 1080;
constexpr int TARGET_FPS = 60;
constexpr int ANTIALIAS_LEVEL = 8;
constexpr char WINDOW_TITLE[] = "Template Demo";

void setup_window(sf::RenderWindow &window, sf::Vector2i screen_size) {
  sf::ContextSettings settings;
  settings.antialiasingLevel = ANTIALIAS_LEVEL;

  window.create(sf::VideoMode(screen_size.x, screen_size.y), WINDOW_TITLE,
                sf::Style::Default, settings);

  if (!window.isOpen()) {
    throw std::runtime_error("Failed to create SFML window\n");
    return;
  }

  std::cout << "Anti-Aliasing: "
            << (window.getSettings().antialiasingLevel ? "ON" : "OFF") << "\n";

  auto desktop = sf::VideoMode::getDesktopMode();
  window.setPosition(
      {static_cast<int>(desktop.width / 2 - screen_size.x / 2),
       static_cast<int>(desktop.height / 2 - screen_size.y / 2)});

  window.setFramerateLimit(TARGET_FPS);
}

int main() {
  const sf::Vector2i SCREEN_DIM(SCREEN_WIDTH, SCREEN_HEIGHT);
  sf::RenderWindow window;
  setup_window(window, SCREEN_DIM);

  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed ||
          sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
        window.close();
      }
    }

    // Drawing
    window.clear(sf::Color::Black);
    // window.draw(...);
    window.display();
  }
}
