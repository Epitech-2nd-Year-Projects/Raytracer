/**
 * @file Vector.hpp
 * @brief Defines the Vector class template for representing mathematical
 * vectors.
 */

#pragma once

#include <array>
#include <cmath>
#include <cstddef>

namespace Raytracer::Math {

template <std::size_t N> class Point;

/**
 * @class Vector
 * @brief A class template representing a mathematical vector in N dimensional
 * space.
 * @tparam N The dimension of the vector.
 */
template <std::size_t N> class Vector {
public:
  std::array<double, N> m_components;

  /**
   * @brief Default constructor initializes all components to zero.
   */
  Vector() { m_components.fill(0.0); }

  /**
   * @brief Constructor that initializes the vector with the given components.
   * @param args The components of the vector.
   */
  template <typename... Args> Vector(Args... args) : m_components{args...} {
    static_assert(sizeof...(args) == N,
                  "Number of arguments must match vector dimension");
    setComponents(0, args...);
  }

  /**
   * @brief Default destructor for the Vector class.
   */
  ~Vector() noexcept = default;

  /**
   * @brief Get the length of the vector.
   * @return The length of the vector.
   */
  double length() const {
    double result = 0.0;

    for (std::size_t i = 0; i < N; ++i) {
      result += m_components[i] * m_components[i];
    }

    return std::sqrt(result);
  }

  /**
   * @brief Adding two vectors.
   * @param other The vector to add.
   * @return A new vector that is the sum of the two vectors.
   */
  Vector<N> operator+(const Vector<N> &other) const {
    Vector<N> result;

    for (std::size_t i = 0; i < N; ++i) {
      result.m_components[i] = m_components[i] + other.m_components[i];
    }

    return result;
  }

  /**
   * @brief Adding two vectors.
   * @param other The vector to add.
   * @return A reference to the current vector after addition.
   */
  Vector<N> &operator+=(const Vector<N> &other) {
    for (std::size_t i = 0; i < N; ++i) {
      m_components[i] += other.m_components[i];
    }

    return *this;
  }

  /**
   * @brief Subtracting two vectors.
   * @param other The vector to subtract.
   * @return A new vector that is the difference of the two vectors.
   */
  Vector<N> operator-(const Vector<N> &other) const {
    Vector<N> result;

    for (std::size_t i = 0; i < N; ++i) {
      result.m_components[i] = m_components[i] - other.m_components[i];
    }

    return result;
  }

  /**
   * @brief Subtracting two vectors.
   * @param other The vector to subtract.
   * @return A reference to the current vector after subtraction.
   */
  Vector<N> &operator-=(const Vector<N> &other) {
    for (std::size_t i = 0; i < N; ++i) {
      m_components[i] -= other.m_components[i];
    }

    return *this;
  }

  /**
   * @brief Multiplying two vectors.
   * @param other The vector to multiply.
   * @return A new vector that is the product of the two vectors.
   */
  Vector<N> operator*(const Vector<N> &other) const {
    Vector<N> result;

    for (std::size_t i = 0; i < N; ++i) {
      result.m_components[i] = m_components[i] * other.m_components[i];
    }

    return result;
  }

  /**
   * @brief Multiplying two vectors.
   * @param other The vector to multiply.
   * @return A reference to the current vector after multiplication.
   */
  Vector<N> &operator*=(const Vector<N> &other) {
    for (std::size_t i = 0; i < N; ++i) {
      m_components[i] *= other.m_components[i];
    }

    return *this;
  }

  /**
   * @brief Dividing two vectors.
   * @param other The vector to divide.
   * @return A new vector that is the quotient of the two vectors.
   */
  Vector<N> operator/(const Vector<N> &other) const {
    Vector<N> result;

    for (std::size_t i = 0; i < N; ++i) {
      result.m_components[i] = m_components[i] / other.m_components[i];
    }

    return result;
  }

  /**
   * @brief Dividing two vectors.
   * @param other The vector to divide.
   * @return A reference to the current vector after division.
   */
  Vector<N> &operator/=(const Vector<N> &other) {
    for (std::size_t i = 0; i < N; ++i) {
      m_components[i] /= other.m_components[i];
    }

    return *this;
  }

  /**
   * @brief Multiplying the vector by a scalar.
   * @param scalar The scalar to multiply.
   * @return A new vector that is the product of the vector and the scalar.
   */
  Vector<N> operator*(double scalar) const {
    Vector<N> result;

    for (std::size_t i = 0; i < N; ++i) {
      result.m_components[i] = m_components[i] * scalar;
    }

    return result;
  }

  /**
   * @brief Multiplying the vector by a scalar.
   * @param scalar The scalar to multiply.
   * @return A reference to the current vector after multiplication.
   */
  Vector<N> &operator*=(double scalar) {
    for (std::size_t i = 0; i < N; ++i) {
      m_components[i] *= scalar;
    }

    return *this;
  }

  /**
   * @brief Dividing the vector by a scalar.
   * @param scalar The scalar to divide.
   * @return A new vector that is the quotient of the vector and the scalar.
   */
  Vector<N> operator/(double scalar) const {
    Vector<N> result;

    for (std::size_t i = 0; i < N; ++i) {
      result.m_components[i] = m_components[i] / scalar;
    }

    return result;
  }

  /**
   * @brief Dividing the vector by a scalar.
   * @param scalar The scalar to divide.
   * @return A reference to the current vector after division.
   */
  Vector<N> &operator/=(double scalar) {
    for (std::size_t i = 0; i < N; ++i) {
      m_components[i] /= scalar;
    }

    return *this;
  }

  /**
   * @brief Calculate the dot product of two vectors.
   * @param other The vector to calculate the dot product with.
   * @return The dot product of the two vectors.
   */
  double dot(const Vector<N> &other) const {
    double result = 0.0;

    for (std::size_t i = 0; i < N; ++i) {
      result += m_components[i] * other.m_components[i];
    }

    return result;
  }

  /**
   * @brief Normalize the vector.
   * @return A new vector that is the normalized version of the current vector.
   */
  Vector<N> normalize() const {
    double len = length();
    if (len == 0.0) {
      return *this;
    }

    Vector<N> result;

    for (std::size_t i = 0; i < N; ++i) {
      result.m_components[i] = m_components[i] / len;
    }

    return result;
  }

  /**
   * @brief Calculate the cross product of two 3D vectors.
   * @param other The vector to calculate the cross product with.
   * @return A new vector that is the cross product of the two vectors.
   */
  Vector<N> cross(const Vector<N> &other) const {
    static_assert(N == 3, "Cross product is only defined for 3D vectors");
    return Vector<N>(m_components[1] * other.m_components[2] -
                         m_components[2] * other.m_components[1],
                     m_components[2] * other.m_components[0] -
                         m_components[0] * other.m_components[2],
                     m_components[0] * other.m_components[1] -
                         m_components[1] * other.m_components[0]);
  }

private:
  /**
   * @brief Base case for setting components.
   * @param index The index of the component to set.
   */
  void setComponents([[maybe_unused]] std::size_t index) {}

  /**
   * @brief Helper function to set the components of the vector.
   * @param index The index of the component to set.
   * @param value The value to set.
   * @param args The remaining components to set.
   */
  template <typename... Args>
  void setComponents(std::size_t index, double value, Args... args) {
    m_components[index] = value;
    setComponents(index + 1, args...);
  }
};

/**
 * @brief Subtracting two points.
 * @param lhs The left-hand side point.
 * @param rhs The right-hand side point.
 * @return A new vector that is the difference of the two points.
 */
template <std::size_t N>
[[nodiscard]] Vector<N> operator-(const Point<N> &lhs, const Point<N> &rhs) {
  Vector<N> result;

  for (std::size_t i = 0; i < N; ++i) {
    result.m_components[i] = lhs.m_components[i] - rhs.m_components[i];
  }

  return result;
}
} // namespace Raytracer::Math
