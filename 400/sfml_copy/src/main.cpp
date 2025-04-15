// Include important C++ libraries here
#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include <sstream>

#include "Vec2D.h"

using namespace sf;

struct Player {
  Vec2D pos = Vec2D(580, 720);
  float vel = 5.5f;
  Vec2D posDelta;
};

// sqrt(2)/2
constexpr double UNIT_DIAG = 0.70710678118;

int main() {
  // Create a video mode object
  sf::VideoMode vm(1920, 1080);

  // Create and open a window for the game
  sf::RenderWindow window(vm, "TD_test", sf::Style::Default);
  auto desktopMode = sf::VideoMode::getDesktopMode();
  window.setPosition(
      sf::Vector2i(desktopMode.width * 0.5 - window.getSize().x * 0.5,
                   desktopMode.height * 0.5 - window.getSize().y * 0.5));
  window.setFramerateLimit(60);

  sf::View view;
  view.setSize(1920.0f, 1080.0f);
  view.setCenter(1920.0f / 2.0f, 1080.0f / 2.0f);
  window.setView(view);

  bool acceptInput = false;
  bool paused = true;

  Player player;

  // Main loop
  while (window.isOpen()) {
    Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) window.close();
      if (event.type == Event::KeyReleased && !paused) {
        // Listen for key presses again
        acceptInput = true;
      }
    }

    if (Keyboard::isKeyPressed(Keyboard::Escape)) {
      window.close();
    }
    // Start the game
    if (Keyboard::isKeyPressed(Keyboard::Return)) {
      paused = false;

      acceptInput = true;
    }

    // Wrap the player controls to
    // Make sure we are accepting input
    player.posDelta = {0, 0};
    if (acceptInput) {
      // First handle pressing the right cursor key
      if (Keyboard::isKeyPressed(Keyboard::Right) ||
          Keyboard::isKeyPressed(Keyboard::D)) {
        player.posDelta.x += 1;
      }
      if (Keyboard::isKeyPressed(Keyboard::Left) ||
          Keyboard::isKeyPressed(Keyboard::A)) {
        player.posDelta.x -= 1;
      }
      if (Keyboard::isKeyPressed(Keyboard::Up) ||
          Keyboard::isKeyPressed(Keyboard::W)) {
        player.posDelta.y -= 1;
      }
      if (Keyboard::isKeyPressed(Keyboard::Down) ||
          Keyboard::isKeyPressed(Keyboard::S)) {
        player.posDelta.y += 1;
      }

      if (player.posDelta.x && player.posDelta.y) {
        player.posDelta = player.posDelta * UNIT_DIAG;
      }
      player.pos += player.posDelta * player.vel;

      Texture texturePlayer;
      texturePlayer.loadFromFile("assets/player.png");
      Sprite spritePlayer;
      spritePlayer.setTexture(texturePlayer);
      spritePlayer.setPosition(player.pos.x, player.pos.y);

      window.clear();  // Clear the screen
      window.draw(spritePlayer);
      window.display();  // Show what was drawn
    }
  }
}
