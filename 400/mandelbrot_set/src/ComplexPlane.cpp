#include "../include/ComplexPlane.h"
#include "../include/Color.h"
#include "../include/timer.h"
#include "../include/window.h"

#include <cmath>
#include <iostream>
#include <thread>

ComplexPlane::ComplexPlane(int pixelWidth, int pixelHeight)
    : m_pixel_size(sf::Vector2i(pixelWidth, pixelHeight)),
      m_aspectRatio((double)pixelHeight / pixelWidth),
      m_plane_center(sf::Vector2f(0.f, 0.f)),
      m_plane_size(BASE_WIDTH, BASE_HEIGHT * m_aspectRatio), m_zoomCount(0),
      m_vArray(
          sf::VertexArray(sf::PrimitiveType::Points, pixelWidth * pixelHeight)),
      m_state(CALCULATING) {};


void ComplexPlane::draw(sf::RenderTarget &target,
                        sf::RenderStates states) const {
  target.draw(m_vArray);
};


void ComplexPlane::zoom() {
  const float scalar = std::pow(BASE_ZOOM, m_zoomCount);

  const float x = BASE_WIDTH * scalar;
  const float y = BASE_HEIGHT * m_aspectRatio * scalar;

  m_plane_size = sf::Vector2f(x, y);
  m_state = CALCULATING;
}


void ComplexPlane::zoomIn() {
  ++m_zoomCount;
  zoom();
};


void ComplexPlane::zoomOut() {
  --m_zoomCount;
  zoom();
};


void ComplexPlane::setCenter(sf::Vector2i mousePixel) {
  m_plane_center = mapPixelToCoords(mousePixel);
  m_state = CALCULATING;
};


void ComplexPlane::setMouseLocation(sf::Vector2i mousePixel) {
  m_mouseLocation = mapPixelToCoords(mousePixel);
};


void ComplexPlane::loadText(sf::Text &text) {
  static sf::Font font;
  if (!font.loadFromFile("Cascadia.ttf")) {
    throw std::runtime_error("unable to open font file");
  }

  std::stringstream ss;
  ss << "Mandelbrot Set\nCenter: (" << m_plane_center.x << ','
     << m_plane_center.y << ")\n"
     << "Cursor: (" << m_mouseLocation.x << ',' << m_mouseLocation.y << ")\n"
     << "Left-click to Zoom in\nRight-click to Zoom out";

  text.setFont(font);
  text.setCharacterSize(24); // Pixels
  text.setFillColor(sf::Color(255, 255, 255));
  text.setString(ss.str());
}


void ComplexPlane::updateRender(const int threadCount) {
  if (m_state == DISPLAYING)
    return;

  //   Timer timer;

  const int chunkSize = std::ceil((float)m_pixel_size.x / threadCount);

  std::vector<std::thread> threads;
  threads.resize(threadCount);

  for (int i = 0; i < m_pixel_size.x; i += chunkSize) {
    int actualChunkSize =
        (chunkSize < m_pixel_size.x - i) ? chunkSize : m_pixel_size.x - i;

    threads.emplace_back(
        [i, actualChunkSize, this]() { calcChunk(i, actualChunkSize); });
  }

  // Join all threads
  for (auto &t : threads) {
    if (t.joinable()) {
      t.join();
    }
  }

  m_state = DISPLAYING;
}


sf::Vector2f ComplexPlane::mapPixelToCoords(sf::Vector2i mousePixel) {

  const float xMag = m_plane_size.x;
  const float yMag = m_plane_size.y;

  const float xOffset = (m_plane_center.x - m_plane_size.x / 2.0);
  const float yOffset = (m_plane_center.y - m_plane_size.y / 2.0);

  const float x = ((float)(mousePixel.x) / (m_pixel_size.x)) * xMag + xOffset;
  const float y =
      (1.0f - ((float)(mousePixel.y) / SCREEN_HEIGHT)) * yMag + yOffset; // flip

  return sf::Vector2f(x, y);
};


int ComplexPlane::countIterations(sf::Vector2f coord) {
  std::complex<double> c = {coord.x, coord.y};
  std::complex<double> z = c;
  int i = 0;
  while (abs(z) < 2.0 && i < int(MAX_ITER)) {
    z = z * z + c;
    i++;
  }

  return i;
};


std::vector<sf::Color>
get_rainbow_colors(int sample_count, sf::Color start_color = sf::Color::Red,
                   float rainbow_percent = 100.f) {
  if (sample_count < 2) {
    throw std::domain_error(
        "sample count must be >= 2 for correct interpolation.");
  }

  const float LIGHTNESS = 70.f;
  const float CHROMA = 60.f;
  const float sample_degrees = (360.0f * rainbow_percent) / 100.0f;

  auto [l, c, start_hue] =
      Color_Space::Rgb(start_color.r, start_color.g, start_color.b)
          .to_xyz()
          .to_lab()
          .to_lch_ab()
          .get_values();

  start_hue -= 20.f; // offset to match perceived color

  std::vector<sf::Color> colors;
  colors.reserve(sample_count);

  for (int i = 0; i < sample_count; ++i) {

    const float hue = Color_Space::normalize_degrees(
        start_hue + (sample_degrees * i) / (sample_count - 1));

    const Color_Space::Lch_Ab lch_ab(LIGHTNESS, CHROMA, hue);

    const auto [r, g, b] = lch_ab.to_lab().to_xyz().to_rgb().get_values();

    colors.push_back(sf::Color(static_cast<uint8_t>(r), static_cast<uint8_t>(g),
                               static_cast<uint8_t>(b)));
  }

  return colors;
}


float interpolate_angle(float a, float b, float t) {
  float delta = fmodf(b - a + 540.0f, 360.0f) - 180.0f;
  return fmodf(a + delta * t + 360.0f, 360.0f);
}


// Interpolate between two LCHab colors
Color_Space::Lch_Ab interpolate_lchab(const Color_Space::Lch_Ab &a,
                                      const Color_Space::Lch_Ab &b, float t) {

  const auto [a_l, a_c, a_h] = a.get_values();
  const auto [b_l, b_c, b_h] = b.get_values();

  float L = a_l * (1.0f - t) + b_l * t;
  float C = a_c * (1.0f - t) + b_c * t;
  float H = interpolate_angle(a_h, b_h, t);
  return Color_Space::Lch_Ab(L, C, H);
}


// A color curve segment
struct ColorSegment {
  Color_Space::Lch_Ab start;
  Color_Space::Lch_Ab end;
  float t_start; // global t where this segment begins (0.0 - 1.0)
  float t_end;   // global t where this segment ends (0.0 - 1.0)

  // Linear interpolation inside this segment
  Color_Space::Lch_Ab sample(float t) const {
    float local_t = (t - t_start) / (t_end - t_start);
    local_t = std::clamp(local_t, 0.f, 1.f);
    return interpolate_lchab(start, end, local_t);
  }
};

// The whole color curve
class ColorCurve {
public:
  std::vector<ColorSegment> segments;

  // Sample the full curve at [0,1]
  Color_Space::Lch_Ab sample(float t) const {
    t = std::clamp(t, 0.f, 1.f);
    for (const auto &seg : segments) {
      if (t >= seg.t_start && t <= seg.t_end) {
        return seg.sample(t);
      }
    }
    // Edge case: if t == 1.0 exactly
    return segments.back().end;
  }
};

// Catmull-Rom interpolation for scalar values
float catmull_rom(float p0, float p1, float p2, float p3, float t) {
  return 0.5f * ((2.0f * p1) + (-p0 + p2) * t +
                 (2.0f * p0 - 5.0f * p1 + 4.0f * p2 - p3) * t * t +
                 (-p0 + 3.0f * p1 - 3.0f * p2 + p3) * t * t * t);
}

// For hue, we need to handle circular interpolation
float catmull_rom_hue(float h0, float h1, float h2, float h3, float t) {
  auto unwrap = [](float a, float ref) {
    while (a - ref > 180.f)
      a -= 360.f;
    while (a - ref < -180.f)
      a += 360.f;
    return a;
  };
  // unwrap all hues relative to h1
  h0 = unwrap(h0, h1);
  h2 = unwrap(h2, h1);
  h3 = unwrap(h3, h2); // slightly different: h3 relative to h2

  float hue = catmull_rom(h0, h1, h2, h3, t);
  hue = fmodf(hue + 360.f, 360.f);
  return hue;
}

// Curve based on control points
class SplineColorCurve {
public:
  std::vector<Color_Space::Lch_Ab> points;

  Color_Space::Lch_Ab sample(float t) const {
    t = std::clamp(t, 0.f, 1.f);
    if (points.size() < 2) {
      throw std::domain_error("Need at least 2 control points");
    }

    float scaled_t = t * (points.size() - 1); // t over total segments
    int i1 = static_cast<int>(scaled_t);
    float local_t = scaled_t - i1;

    // Clamp indices
    int i0 = std::max(i1 - 1, 0);
    int i2 = std::min(i1 + 1, (int)points.size() - 1);
    int i3 = std::min(i1 + 2, (int)points.size() - 1);

    const auto &p0 = points[i0];
    const auto &p1 = points[i1];
    const auto &p2 = points[i2];
    const auto &p3 = points[i3];

    float L =
        catmull_rom(p0.get_l(), p1.get_l(), p2.get_l(), p3.get_l(), local_t);
    float C =
        catmull_rom(p0.get_c(), p1.get_c(), p2.get_c(), p3.get_c(), local_t);
    float H = catmull_rom_hue(p0.get_h(), p1.get_h(), p2.get_h(), p3.get_h(),
                              local_t);

    return Color_Space::Lch_Ab(L, C, H);
  }
};


std::vector<sf::Color> make_spline_gradient(int sample_count,
                                            const SplineColorCurve &curve) {
  std::vector<sf::Color> colors;
  colors.reserve(sample_count);

  for (int i = 0; i < sample_count; ++i) {
    float t = (float)i / (sample_count - 1);
    auto lch = curve.sample(t);
    auto [r, g, b] = lch.to_lab().to_xyz().to_rgb().get_values();
    colors.push_back(sf::Color(r, g, b));
  }
  return colors;
}


std::vector<sf::Color> get_gradient_colors(const int sample_count,
                                           const ColorCurve &curve) {
  std::vector<sf::Color> colors;
  colors.reserve(sample_count);

  for (int i = 0; i < sample_count; ++i) {
    float t = (float)i / (sample_count - 1);
    auto lch = curve.sample(t);
    auto [r, g, b] = lch.to_lab().to_xyz().to_rgb().get_values();
    colors.push_back(sf::Color(r, g, b));
  }

  return colors;
}


// Map the given iteration count to an r,g,b color
void ComplexPlane::iterationsToRGB(size_t count, u_int8_t &r, u_int8_t &g,
                                   u_int8_t &b) {
  //   const static std::vector<sf::Color> colors =
  //       get_rainbow_colors(MAX_ITER, sf::Color::Blue, 300);

  static auto blue = Color_Space::Rgb(14, 7, 37).to_xyz().to_lab().to_lch_ab();
  static auto purple =
      Color_Space::Rgb(62, 25, 110).to_xyz().to_lab().to_lch_ab();
  static auto sherbert =
      Color_Space::Rgb(255, 96, 123).to_xyz().to_lab().to_lch_ab();
  static auto rose =
      Color_Space::Rgb(212, 108, 118).to_xyz().to_lab().to_lch_ab();
  static auto orange =
      Color_Space::Rgb(252, 159, 50).to_xyz().to_lab().to_lch_ab();
  static auto yellow =
      Color_Space::Rgb(255, 226, 124).to_xyz().to_lab().to_lch_ab();
  static auto white =
      Color_Space::Rgb(255, 255, 255).to_xyz().to_lab().to_lch_ab();

  SplineColorCurve curve;
  curve.points = {blue, purple, rose, orange, yellow, white};

  const static auto colors = make_spline_gradient(MAX_ITER, curve);

  if (count == MAX_ITER) {
    r = g = b = 0;
    return;
  }

  sf::Color color = colors[count];

  r = color.r;
  g = color.g;
  b = color.b;
}


void ComplexPlane::calcChunk(const int rowStart, const int rowsToCalc) {
  for (int j = rowStart; j < rowStart + rowsToCalc; j++) {
    for (int i = 0; i < m_pixel_size.y; i++) {
      m_vArray[j + i * m_pixel_size.x].position = {(float)j, (float)i};

      const sf::Vector2f complexCoord = mapPixelToCoords(sf::Vector2i(j, i));

      const int iterations = countIterations(complexCoord);

      u_int8_t r;
      u_int8_t g;
      u_int8_t b;

      iterationsToRGB(iterations, r, g, b);

      m_vArray[j + i * m_pixel_size.x].color = {r, g, b};
    }
  }
}
