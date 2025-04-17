// Include important C++ libraries here
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

struct Board {
  float margin;

  float width;
  float height;

  int cols;
  int rows;

  Board(const float w_width, const float w_height, const float t_size) {
    margin = 2 * t_size;

    width = w_width - 2 * margin;
    height = w_height - 2 * margin;

    cols = static_cast<int>(width / t_size);
    rows = static_cast<int>(height / t_size);
  }
};

std::vector<sf::Vertex> get_lines(const Board &board, float t_size) {
  std::vector<sf::Vertex> lines;

  const float right = board.margin + board.cols * t_size;
  const float bottom = board.margin + board.rows * t_size;

  // Vertical grid lines
  for (int col = 0; col <= board.cols; ++col) {
    float x = board.margin + col * t_size;
    lines.push_back(
        sf::Vertex(sf::Vector2f(x, board.margin), sf::Color::White));
    lines.push_back(sf::Vertex(sf::Vector2f(x, bottom), sf::Color::White));
  }

  // Horizontal grid lines
  for (int row = 0; row <= board.rows; ++row) {
    float y = board.margin + row * t_size;
    lines.push_back(
        sf::Vertex(sf::Vector2f(board.margin, y), sf::Color::White));
    lines.push_back(sf::Vertex(sf::Vector2f(right, y), sf::Color::White));
  }

  return lines;
}

int main() {
  constexpr float w_width = 1920.0f;
  constexpr float w_height = 1080.0f;
  constexpr int t_size = 100;

  // WINDOW SETUP
  sf::VideoMode vm(static_cast<u_int>(w_width), static_cast<u_int>(w_height));
  sf::RenderWindow window(vm, "TD_test", sf::Style::Default);
  auto desktopMode = sf::VideoMode::getDesktopMode();
  window.setPosition(
      sf::Vector2i(desktopMode.width * 0.5 - window.getSize().x * 0.5,
                   desktopMode.height * 0.5 - window.getSize().y * 0.5));
  sf::View view;
  view.setSize(w_width, w_height);
  view.setCenter(w_width / 2.0f, w_height / 2.0f);
  window.setView(view);

  // GRID

  Board board(w_width, w_height, t_size);

  std::vector<sf::Vertex> lines = get_lines(board, t_size);

  // TOWER
  sf::Vector2f tower_pos =
      sf::Vector2f(board.margin + (board.cols - 1) * t_size + (t_size / 4),
                   board.margin + (board.rows / 2) * t_size + (t_size / 4));
  auto tower = sf::CircleShape(30);
  tower.setPosition(tower_pos);
  tower.setFillColor(sf::Color(100, 100, 100));

  // Main loop
  while (window.isOpen()) {
    sf::Event event;
    // sf::Vector2i mouse_pos = sf::Mouse::getPosition(window);

    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) window.close();
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
      window.close();
    }

    window.clear();

    window.draw(&lines[0], lines.size(), sf::Lines);
    window.draw(tower);

    window.display();  // Show what was drawn
  }
}
