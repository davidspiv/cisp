// Include important C++ libraries here
#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include <vector>

struct Board {
  float margin;
  size_t cols;
  size_t rows;

  Board(unsigned w_width, unsigned w_height, float t_size)
      : margin(2.0f * t_size),
        cols(static_cast<size_t>((w_width - 4.0f * t_size) / t_size)),
        rows(static_cast<size_t>((w_height - 4.0f * t_size) / t_size)) {}
};

sf::VertexArray createRoundedRect(sf::Vector2f pos, sf::Vector2f size,
                                  float radius,
                                  std::size_t cornerResolution = 8) {
  sf::VertexArray vertices(sf::TriangleFan);

  sf::Vector2f center = pos + size * 0.5f;
  vertices.append(sf::Vertex(center, sf::Color::White));  // Center of fan

  std::vector<sf::Vector2f> arcPoints;

  // Helper lambda to generate arc points for each corner
  auto arc = [&](sf::Vector2f cornerCenter, float startAngle) {
    for (std::size_t i = 0; i <= cornerResolution; ++i) {
      float angle = startAngle + (90.0f * i / cornerResolution);
      float rad = angle * 3.14159265f / 180.0f;
      float x = cornerCenter.x + std::cos(rad) * radius;
      float y = cornerCenter.y + std::sin(rad) * radius;
      arcPoints.emplace_back(x, y);
    }
  };

  // Generate corner arcs clockwise
  arc(pos + sf::Vector2f(radius, radius), 180.f);           // Top-left
  arc(pos + sf::Vector2f(size.x - radius, radius), 270.f);  // Top-right
  arc(pos + sf::Vector2f(size.x - radius, size.y - radius),
      0.f);                                                // Bottom-right
  arc(pos + sf::Vector2f(radius, size.y - radius), 90.f);  // Bottom-left

  // Append arc points to vertex array
  for (const auto& pt : arcPoints)
    vertices.append(sf::Vertex(pt, sf::Color::White));

  // 👇 Close the fan by repeating the first outer point
  vertices.append(sf::Vertex(arcPoints.front(), sf::Color::White));

  return vertices;
}

std::vector<sf::Vertex> build_grid(const Board& board, float t_size) {
  std::vector<sf::Vertex> lines;

  const float right = board.margin + board.cols * t_size;
  const float bottom = board.margin + board.rows * t_size;

  // Vertical lines
  for (size_t col = 0; col <= board.cols; ++col) {
    float x = board.margin + col * t_size;
    lines.emplace_back(sf::Vector2f(x, board.margin), sf::Color::White);
    lines.emplace_back(sf::Vector2f(x, bottom), sf::Color::White);
  }

  // Horizontal lines
  for (size_t row = 0; row <= board.rows; ++row) {
    float y = board.margin + row * t_size;
    lines.emplace_back(sf::Vector2f(board.margin, y), sf::Color::White);
    lines.emplace_back(sf::Vector2f(right, y), sf::Color::White);
  }

  return lines;
}

int main() {
  unsigned w_width = 1920;
  unsigned w_height = 1080;
  constexpr size_t t_size = 100;

  // WINDOW SETUP
  sf::RenderWindow window(sf::VideoMode(w_width, w_height), "TD_test");
  auto desktop = sf::VideoMode::getDesktopMode();
  window.setPosition({static_cast<int>(desktop.width / 2 - w_width / 2),
                      static_cast<int>(desktop.height / 2 - w_height / 2)});

  // BOARD SETUP
  Board board(w_width, w_height, t_size);
  std::vector<sf::Vertex> grid_line = build_grid(board, t_size);

  // TOWER
  sf::Vector2f tower_pos =
      sf::Vector2f(board.margin + (board.cols - 1) * t_size + (t_size / 4),
                   board.margin + (board.rows / 2) * t_size + (t_size / 4));
  auto tower = sf::CircleShape(30);
  tower.setPosition(tower_pos);
  tower.setFillColor(sf::Color(100, 100, 100));

  // GAMEPLAY LOOP
  while (window.isOpen()) {
    /*
    ****************************************
    Handle the players input
    ****************************************
    */
    sf::Event event;

    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) window.close();
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
      window.close();
    }

    const sf::Vector2i mouse_pos = sf::Mouse::getPosition(window);

    /*
    ****************************************
    Update the scene
    ****************************************
    */

    /*
    ****************************************
    Draw the scene
    ****************************************
    */
    window.clear();

    auto rounded = createRoundedRect({100.f, 100.f}, {300.f, 150.f}, 20.f);
    window.draw(&rounded[0], rounded.getVertexCount(), sf::TriangleFan);

    window.draw(&grid_line[0], grid_line.size(), sf::Lines);

    if (mouse_pos.x > board.margin &&
        mouse_pos.x < board.cols * t_size + board.margin &&
        mouse_pos.y > board.margin &&
        mouse_pos.y < board.rows * t_size + board.margin) {
      const sf::Vector2f focused_tile_pos =
          sf::Vector2f(static_cast<int>(mouse_pos.x / t_size) * t_size,
                       static_cast<int>(mouse_pos.y / t_size) * t_size);
      sf::RectangleShape focused_tile(sf::Vector2f(t_size, t_size));
      focused_tile.setPosition(focused_tile_pos);
      focused_tile.setFillColor(sf::Color(100, 100, 100, 100));

      window.draw(focused_tile);
    }
    window.draw(tower);

    window.display();  // Show what was drawn
  }
}
