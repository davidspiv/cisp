#pragma once

#include <SFML/Graphics.hpp>

constexpr char WINDOW_TITLE[] = "Mandelbrot Set";
constexpr int SCREEN_WIDTH = 1920 ;
constexpr int SCREEN_HEIGHT = 1080;
constexpr int TARGET_FPS = 60;
constexpr int ANTIALIAS_LEVEL = 8;

void setup_window(sf::RenderWindow &window, const int width, const int height);
