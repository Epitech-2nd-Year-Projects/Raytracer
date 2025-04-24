#pragma once

#include <algorithm>

namespace Raytracer::Utility {

template <typename T>
concept Arithmetic = std::is_arithmetic_v<T>;

template <Arithmetic T, T Min, T Max>
  requires (Min <= Max)
class Clamped {
public:
  constexpr Clamped() noexcept = default;
  explicit constexpr Clamped(T v) noexcept : m_value{std::clamp(v, Min, Max)} {}

  constexpr Clamped &operator=(T v) noexcept {
    m_value = std::clamp(v, Min, Max);
    return *this;
  }

  constexpr Clamped &operator+=(T v) noexcept { return *this = m_value + v; }
  constexpr Clamped &operator-=(T v) noexcept { return *this = m_value - v; }
  constexpr Clamped &operator*=(T v) noexcept { return *this = m_value * v; }
  constexpr Clamped &operator/=(T v) noexcept { return *this = m_value / v; }

  explicit constexpr operator T() const noexcept { return m_value; }

  [[nodiscard]] constexpr T get() const noexcept { return m_value; }

private:
  T m_value{Min};
};

} // namespace Raytracer::Utility