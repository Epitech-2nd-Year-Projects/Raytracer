#include "Core/Color.hpp"

namespace Raytracer::Core {

Color::Color(double red, double green, double blue) noexcept
  : m_r(red), m_g(green), m_b(blue) {}

constexpr double Color::getB() const noexcept {
  return m_b.get();
}

constexpr double Color::getG() const noexcept {
  return m_g.get();
}

constexpr double Color::getR() const noexcept {
  return m_r.get();
}

void Color::setB(double blue) noexcept {
  m_b = blue;
}

void Color::setG(double green) noexcept {
  m_g = green;
}

void Color::setR(double red) noexcept {
  m_r = red;
}


Color Color::multiply(const Color &other) const noexcept {
  return Color(getR() * other.getR() / 255.0,
               getG() * other.getG() / 255.0,
               getB() * other.getB() / 255.0);
}

Color Color::add(const Color &other) const noexcept {
  return Color(getR() + other.getR(),
               getG() + other.getG(),
               getB() + other.getB());
}

Color Color::scale(const Color &other) const noexcept {
  return Color(getR() * other.getR(),
               getG() * other.getG(),
               getB() * other.getB());
}
}