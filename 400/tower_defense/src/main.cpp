// Include important C++ libraries here
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

int main() {
  const float w_width = 1920.0f;
  const float w_height = 1080.0f;

  std::vector<sf::Vector2f> squares;

  sf::VideoMode vm(static_cast<u_int>(w_width), static_cast<u_int>(w_height));

  // Create and open a window for the game
  sf::RenderWindow window(vm, "TD_test", sf::Style::Default);
  auto desktopMode = sf::VideoMode::getDesktopMode();
  window.setPosition(
      sf::Vector2i(desktopMode.width * 0.5 - window.getSize().x * 0.5,
                   desktopMode.height * 0.5 - window.getSize().y * 0.5));
  window.setFramerateLimit(60);

  sf::View view;
  view.setSize(w_width, w_height);
  view.setCenter(w_width / 2.0f, w_height / 2.0f);
  window.setView(view);

  // Main loop
  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) window.close();
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
      window.close();
    }

    window.clear();    // Clear the screen
    window.display();  // Show what was drawn
  }
}
