#ifndef VEC2D_H
#define VEC2D_H

struct Vec2D {
  Vec2D();
  Vec2D(float x, float y);

  float magSquared(const Vec2D &other) const;
  void print() const;

  Vec2D operator+(const Vec2D &other) const;
  Vec2D operator*(const float scalar) const;
  Vec2D operator*(const Vec2D &other) const;
  Vec2D operator/(float scalar) const;
  Vec2D &operator+=(const Vec2D &other);
  Vec2D &operator-=(const Vec2D &other);
  Vec2D operator-(const Vec2D &other) const;
  Vec2D operator/(const Vec2D &other) const;

  float x, y, z;
};

#endif
