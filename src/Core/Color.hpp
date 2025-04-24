#pragma once
#include "Utility/Clamped.hpp"

namespace Raytracer::Core {

class Color final {
public:
  constexpr Color() noexcept = default;
  constexpr Color(double red, double green, double blue) noexcept;

  [[nodiscard]] constexpr double getR() const noexcept { return m_r.get(); }
  [[nodiscard]] constexpr double getG() const noexcept { return m_g.get(); }
  [[nodiscard]] constexpr double getB() const noexcept { return m_b.get(); }

  void setR(const double red) noexcept { m_r = red; }
  void setG(const double red) noexcept { m_r = red; }
  void setB(const double red) noexcept { m_r = red; }

  [[nodiscard]] Color multiply(const Color &other) const noexcept;
  [[nodiscard]] Color add(const Color &other) const noexcept;
  [[nodiscard]] Color scale(const Color &other) const noexcept;

private:
  Utility::Clamped<double, 0.0, 255.0> m_r{0.0};
  Utility::Clamped<double, 0.0, 255.0> m_g{0.0};
  Utility::Clamped<double, 0.0, 255.0> m_b{0.0};
};
} // namespace Raytracer::Core