/**
 * @file Color.hpp
 * @brief Defines the Color class for RGB color operations.
 */

#pragma once

#include "Utility/Clamped.hpp"

namespace Raytracer::Core {

/**
 * @class Color
 * @brief Represents an RGB color with clamped components.
 */
class Color final {
public:
  /**
   * @brief Default constructor initializes color to black.
   */
  constexpr Color() noexcept = default;

  /**
   * @brief Construct with specified RGB components.
   * @param red Red component [0, 255].
   * @param green Green component [0, 255].
   * @param blue Blue component [0, 255].
   */
  Color(double red, double green, double blue) noexcept;

  /**
   * @brief Get red component.
   * @return Red value.
   */
  [[nodiscard]] constexpr double getR() const noexcept { return m_r.get(); }

  /**
   * @brief Get green component.
   * @return Green value.
   */
  [[nodiscard]] constexpr double getG() const noexcept { return m_g.get(); }

  /**
   * @brief Get blue component.
   * @return Blue value.
   */
  [[nodiscard]] constexpr double getB() const noexcept { return m_b.get(); }

  /**
   * @brief Set red component.
   * @param red New red value.
   */
  void setR(double red) noexcept { m_r = red; }

  /**
   * @brief Set green component.
   * @param green New green value.
   */
  void setG(double green) noexcept { m_g = green; }

  /**
   * @brief Set blue component.
   * @param blue New blue value.
   */
  void setB(double blue) noexcept { m_b = blue; }

  /**
   * @brief Component-wise multiplication.
   * @param other Other color.
   * @return Resulting color.
   */
  [[nodiscard]] Color multiply(const Color &other) const noexcept;

  /**
   * @brief Component-wise addition.
   * @param other Other color.
   * @return Resulting color.
   */
  [[nodiscard]] Color add(const Color &other) const noexcept;

  /**
   * @brief Component-wise scaling.
   * @param other Other color.
   * @return Resulting color.
   */
  [[nodiscard]] Color scale(const Color &other) const noexcept;

private:
  Utility::Clamped<double, 0.0, 255.0> m_r{0.0};
  Utility::Clamped<double, 0.0, 255.0> m_g{0.0};
  Utility::Clamped<double, 0.0, 255.0> m_b{0.0};
};

} // namespace Raytracer::Core
