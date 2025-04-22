/**
 * @file Vector.hpp
 * @brief Defines the Vector class template for representing mathematical
 * vectors.
 */

#pragma once

#include <array>
#include <cstddef>
namespace Raytracer::Math {

template <std::size_t N> class Vector {
public:
  std::array<double, N> m_components;

  Vector() { m_components.fill(0.0); }

  template <typename... Args> Vector(Args... args) : m_components{args...} {
    static_assert(sizeof...(args) == N,
                  "Number of arguments must match vector size");
    setComponents(0, args...);
  }

private:
  void setComponents(std::size_t index) {}

  template <typename... Args>
  void setComponents(std::size_t index, double value, Args... args) {
    m_components[index] = value;
    setComponents(index + 1, args...);
  }
};
} // namespace Raytracer::Math
