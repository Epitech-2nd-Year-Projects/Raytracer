/**
 * @file Clamped.hpp
 * @brief Defines the Clamped template for constraining arithmetic values within a range.
 */

#pragma once

#include <algorithm>

namespace Raytracer::Utility {

/**
 * @concept Arithmetic
 * @brief Concept for arithmetic types.
 */
template <typename T>
concept Arithmetic = std::is_arithmetic_v<T>;

/**
 * @class Clamped
 * @brief Wrapper that clamps a value between compile-time bounds.
 * @tparam T Arithmetic type to clamp.
 * @tparam Min Minimum allowed value.
 * @tparam Max Maximum allowed value.
 * @note Requires Min <= Max.
 */
template <Arithmetic T, T Min, T Max>
  requires(Min <= Max)
class Clamped {
public:
  /**
   * @brief Default constructs with the minimum value.
   */
  constexpr Clamped() noexcept = default;

  /**
   * @brief Constructs and clamps the given value.
   * @param v Initial value to clamp.
   */
  explicit constexpr Clamped(T v) noexcept : m_value{std::clamp(v, Min, Max)} {}

  /**
   * @brief Assign and clamp a new value.
   * @param v Value to clamp and assign.
   * @return Reference to this.
   */
  constexpr Clamped &operator=(T v) noexcept {
    m_value = std::clamp(v, Min, Max);
    return *this;
  }

  /**
   * @brief Add and clamp.
   * @param v Value to add.
   * @return Reference to this.
   */
  constexpr Clamped &operator+=(T v) noexcept { return *this = m_value + v; }

  /**
   * @brief Subtract and clamp.
   * @param v Value to subtract.
   * @return Reference to this.
   */
  constexpr Clamped &operator-=(T v) noexcept { return *this = m_value - v; }

  /**
   * @brief Multiply and clamp.
   * @param v Value to multiply.
   * @return Reference to this.
   */
  constexpr Clamped &operator*=(T v) noexcept { return *this = m_value * v; }

  /**
   * @brief Divide and clamp.
   * @param v Value to divide.
   * @return Reference to this.
   */
  constexpr Clamped &operator/=(T v) noexcept { return *this = m_value / v; }

  /**
   * @brief Implicit conversion to the underlying type.
   */
  explicit constexpr operator T() const noexcept { return m_value; }

  /**
   * @brief Get the current clamped value.
   * @return The clamped value.
   */
  [[nodiscard]] constexpr T get() const noexcept { return m_value; }

private:
  T m_value{Min};
};

} // namespace Raytracer::Utility
