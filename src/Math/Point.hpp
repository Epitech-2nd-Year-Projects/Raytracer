/**
 * @file Point.hpp
 * @brief Defines the Point class template for representing mathematical
 * points.
 */

#pragma once

#include "Vector.hpp"
#include <array>
#include <cmath>
namespace Raytracer::Math {

/**
 * @class Point
 * @brief A class template representing a mathematical point in N-dimensional
 * space.
 * @tparam N The dimension of the point.
 */
template <std::size_t N> class Point {
public:
  std::array<double, N> m_components;

  /**
   * @brief Default constructor initializes all components to zero.
   */
  Point() { m_components.fill(0.0); }

  /**
   * @brief Constructor that initializes the point with the given components.
   * @param args The components of the point.
   */
  template <typename... Args> Point(Args... args) : m_components{args...} {
    static_assert(sizeof...(args) == N,
                  "Number of arguments must match point dimension");
    setComponents(0, args...);
  }

  /**
   * @brief Subtracting one point from another.
   * @param other The point to subtract from this point.
   * @return A vector representing the difference between the two points.
   */
  Vector<N> operator-(const Point<N> &other) const {
    Vector<N> result;
    for (size_t i = 0; i < N; ++i) {
      result.m_components[i] = m_components[i] - other.m_components[i];
    }
    return result;
  }
  /**
   * @brief Adding a point and a vector.
   * @param vec The vector to add.
   * @return A new point that is the sum of the point and the vector.
   */
  Point<N> operator+(const Vector<N> &vec) const {
    Point<N> result;
    for (size_t i = 0; i < N; ++i) {
      result.m_components[i] = m_components[i] + vec.m_components[i];
    }
    return result;
  }

  /**
   * @brief Adding a point and a vector.
   * @param vec The vector to add.
   * @return A reference to the current point after addition.
   */
  Point<N> &operator+=(const Vector<N> &vec) {
    for (size_t i = 0; i < N; ++i) {
      m_components[i] += vec.m_components[i];
    }
    return *this;
  }

  /**
   * @brief Subtracting a vector from a point.
   * @param vec The vector to subtract.
   * @return A new point that is the difference of the point and the vector.
   */
  Point<N> operator-(const Vector<N> &vec) const {
    Point<N> result;
    for (size_t i = 0; i < N; ++i) {
      result.m_components[i] = m_components[i] - vec.m_components[i];
    }
    return result;
  }

  /**
   * @brief Subtracting a vector from a point.
   * @param vec The vector to subtract.
   * @return A reference to the current point after subtraction.
   */
  Point<N> &operator-=(const Vector<N> &vec) {
    for (size_t i = 0; i < N; ++i) {
      m_components[i] -= vec.m_components[i];
    }
    return *this;
  }

private:
  /**
   * @brief Base case for setting components.
   * @param index The index of the component to set.
   */
  void setComponents([[maybe_unused]] size_t index) {}

  /**
   * @brief Helper function to set the components of the point.
   * @param index The index of the component to set.
   * @param value The value to set.
   * @param args The remaining components to set.
   */
  template <typename T, typename... Args>
  void setComponents(size_t index, T value, Args... args) {
    m_components[index] = static_cast<double>(value);
    setComponents(index + 1, args...);
  }
};
} // namespace Raytracer::Math