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

template <std::size_t N> class Vector {
public:
  std::array<double, N> m_components;

  Vector() { m_components.fill(0.0); }

  template <typename... Args> Vector(Args... args) : m_components{args...} {
    static_assert(sizeof...(args) == N,
                  "Number of arguments must match vector size");
    setComponents(0, args...);
  }

  double length() const {
    double result = 0.0;

    for (std::size_t i = 0; i < N; ++i) {
      result += m_components[i] * m_components[i];
    }

    return std::sqrt(result);
  }

  Vector<N> operator+(const Vector<N> &other) const {
    Vector<N> result;

    for (std::size_t i = 0; i < N; ++i) {
      result.m_components[i] = m_components[i] + other.m_components[i];
    }

    return result;
  }

  Vector<N> &operator+=(const Vector<N> &other) {
    for (std::size_t i = 0; i < N; ++i) {
      m_components[i] += other.m_components[i];
    }

    return *this;
  }

  Vector<N> operator-(const Vector<N> &other) const {
    Vector<N> result;

    for (std::size_t i = 0; i < N; ++i) {
      result.m_components[i] = m_components[i] - other.m_components[i];
    }

    return result;
  }

  Vector<N> &operator-=(const Vector<N> &other) {
    for (std::size_t i = 0; i < N; ++i) {
      m_components[i] -= other.m_components[i];
    }

    return *this;
  }

  Vector<N> operator*(const Vector<N> &other) const {
    Vector<N> result;

    for (std::size_t i = 0; i < N; ++i) {
      result.m_components[i] = m_components[i] * other.m_components[i];
    }

    return result;
  }

  Vector<N> &operator*=(const Vector<N> &other) {
    for (std::size_t i = 0; i < N; ++i) {
      m_components[i] *= other.m_components[i];
    }

    return *this;
  }

  Vector<N> operator/(const Vector<N> &other) const {
    Vector<N> result;

    for (std::size_t i = 0; i < N; ++i) {
      result.m_components[i] = m_components[i] / other.m_components[i];
    }

    return result;
  }

  Vector<N> &operator/=(const Vector<N> &other) {
    for (std::size_t i = 0; i < N; ++i) {
      m_components[i] /= other.m_components[i];
    }

    return *this;
  }

  Vector<N> operator*(double scalar) const {
    Vector<N> result;

    for (std::size_t i = 0; i < N; ++i) {
      result.m_components[i] = m_components[i] * scalar;
    }

    return result;
  }

  Vector<N> &operator*=(double scalar) {
    for (std::size_t i = 0; i < N; ++i) {
      m_components[i] *= scalar;
    }

    return *this;
  }

  Vector<N> operator/(double scalar) const {
    Vector<N> result;

    for (std::size_t i = 0; i < N; ++i) {
      result.m_components[i] = m_components[i] / scalar;
    }

    return result;
  }

  Vector<N> &operator/=(double scalar) {
    for (std::size_t i = 0; i < N; ++i) {
      m_components[i] /= scalar;
    }

    return *this;
  }

  double dot(const Vector<N> &other) const {
    double result = 0.0;

    for (std::size_t i = 0; i < N; ++i) {
      result += m_components[i] * other.m_components[i];
    }

    return result;
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
