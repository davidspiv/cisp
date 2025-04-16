#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include <random>
#include <sstream>
#include <vector>

using namespace std;

sf::Vector2f getRandVertex(const vector<sf::Vector2f> &vertices) {
  int x = 0;  // Minimum value
  int y = 2;  // Maximum value

  // Generate a random number between x and y
  int idx = rand() % (y - x + 1) + x;
  return vertices[idx];
};

sf::Vector2f calcMidpoint(const sf::Vector2f &a, const sf::Vector2f &b) {
  int xCoord = (a.x + b.x) / 2.0;
  int yCoord = (a.y + b.y) / 2.0;
  return sf::Vector2f(xCoord, yCoord);
};

int main() {
  srand(time(0));
  // Create a video mode object
  sf::VideoMode vm(1920, 1080);
  // Create and open a window for the game
  sf::RenderWindow window(vm, "Chaos Game!!", sf::Style::Default);

  // open window in center of screen
  auto desktopMode = sf::VideoMode::getDesktopMode();
  window.setPosition(
      sf::Vector2i(desktopMode.width * 0.5 - window.getSize().x * 0.5,
                   desktopMode.height * 0.5 - window.getSize().y * 0.5));

  window.setFramerateLimit(60);

  vector<sf::Vector2f> vertices;
  vector<sf::Vector2f> points;
  sf::Vector2f targetPoint;

  while (window.isOpen()) {
    /*
    ****************************************
    Input
    ****************************************
    */
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        // Quit the game when the window is closed
        window.close();
      }
      if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Left) {
          //   std::cout << "the left button was pressed" << std::endl;
          //   std::cout << "mouse x: " << event.mouseButton.x << std::endl;
          //   std::cout << "mouse y: " << event.mouseButton.y << std::endl;

          if (vertices.size() < 3) {
            vertices.push_back(
                sf::Vector2f(event.mouseButton.x, event.mouseButton.y));
          } else if (points.size() == 0) {
            targetPoint =
                sf::Vector2f(event.mouseButton.x, event.mouseButton.y);
            points.push_back(targetPoint);
          }
        }
      }
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
      window.close();
    }
    /*
    ****************************************
    Update
    ****************************************
    */

    if (points.size() > 0 && points.size() < 10000) {
      /// generate more point(s)
      for (size_t i = 0; i < 100; i++) {
        /// select random vertex
        sf::Vector2f randVertex = getRandVertex(vertices);
 
        /// calculate midpoint between random vertex and the last point in the
        /// vector push back the newly generated coord.
        targetPoint = calcMidpoint(targetPoint, randVertex);
        points.push_back(targetPoint);
      }
    }

    /*
    ****************************************
    Draw
    ****************************************
    */
    window.clear();
    for (size_t i = 0; i < vertices.size(); i++) {
      sf::RectangleShape rect(sf::Vector2f(10, 10));
      rect.setPosition(sf::Vector2f(vertices[i].x, vertices[i].y));
      rect.setFillColor(sf::Color::Blue);
      window.draw(rect);
    }
    /// Draw points
    for (size_t i = 0; i < points.size(); i++) {
      sf::RectangleShape rect(sf::Vector2f(10, 10));
      rect.setPosition(sf::Vector2f(points[i].x, points[i].y));
      rect.setFillColor(sf::Color::White);
      window.draw(rect);
    }
    window.display();
  }
}
