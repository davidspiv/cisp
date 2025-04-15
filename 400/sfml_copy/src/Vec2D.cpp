#include "Vec2D.h"

#include <cmath>
#include <iomanip>
#include <iostream>

Vec2D::Vec2D() : x(0), y(0) {};

Vec2D::Vec2D(float x, float y) : x(x), y(y) {};

float Vec2D::magSquared(const Vec2D &other) const {
  const float xD = other.x - x;
  const float yD = other.y - y;
  const float zD = other.z - z;
  return xD * xD + yD * yD + zD * zD;
}

void Vec2D::print() const {
  std::cout << std::left << std::fixed << std::setprecision(9)
            << "X: " << std::setw(14) << x << "Y: " << std::setw(14) << y
            << std::right;
}

Vec2D Vec2D::operator+(const Vec2D &other) const {
  return {x + other.x, y + other.y};
}

Vec2D Vec2D::operator-(const Vec2D &other) const {
  return {x - other.x, y - other.y};
}

Vec2D Vec2D::operator*(const float scalar) const {
  return {x * scalar, y * scalar};
}

Vec2D Vec2D::operator*(const Vec2D &other) const {
  return {x * other.x, y * other.y};
}

Vec2D Vec2D::operator/(const Vec2D &other) const {
  return {x / other.x, y / other.y};
}

Vec2D Vec2D::operator/(float scalar) const {
  scalar = 1.0 / scalar;
  return {x * scalar, y * scalar};
}

Vec2D &Vec2D::operator+=(const Vec2D &other) {
  x += other.x;
  y += other.y;
  return *this;
}

Vec2D &Vec2D::operator-=(const Vec2D &other) {
  x -= other.x;
  y -= other.y;
  return *this;
}
