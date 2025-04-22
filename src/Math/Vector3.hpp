/**
 * @file Vector3.hpp
 * @brief Defines the Vector3 class for three‐dimensional vector operations.
 */

#pragma once
#include <array>

namespace Raytracer::Math {

/**
 * @class Vector3
 * @brief Represents a three‐dimensional vector.
 */
class Vector3 {
public:
  /**
   * @brief Default constructor initializes to (0,0,0).
   */
  constexpr Vector3() noexcept : m_data({0, 0, 0}) {}

  /**
   * @brief Construct a vector with given components.
   * @param x The X‐component.
   * @param y The Y‐component.
   * @param z The Z‐component.
   */
  constexpr Vector3(double x, double y, double z) noexcept
      : m_data({x, y, z}) {}

  /**
   * @brief Get the X‐component.
   * @return The X‐coordinate.
   */
  [[nodiscard]] constexpr double x() const noexcept { return m_data[0]; }

  /**
   * @brief Get the Y‐component.
   * @return The Y‐coordinate.
   */
  [[nodiscard]] constexpr double y() const noexcept { return m_data[1]; }

  /**
   * @brief Get the Z‐component.
   * @return The Z‐coordinate.
   */
  [[nodiscard]] constexpr double z() const noexcept { return m_data[2]; }

  /**
   * @brief Access the X‐component by reference.
   * @return Reference to the X‐coordinate.
   */
  constexpr double &x() noexcept { return m_data[0]; }

  /**
   * @brief Access the Y‐component by reference.
   * @return Reference to the Y‐coordinate.
   */
  constexpr double &y() noexcept { return m_data[1]; }

  /**
   * @brief Access the Z‐component by reference.
   * @return Reference to the Z‐coordinate.
   */
  constexpr double &z() noexcept { return m_data[2]; }

  /**
   * @brief Vector addition.
   * @param other The vector to add.
   * @return A new Vector3 equal to the mathematical sum.
   */
  [[nodiscard]] constexpr Vector3
  operator+(const Vector3 &other) const noexcept;

  /**
   * @brief Vector subtraction.
   * @param other The vector to subtract.
   * @return A new Vector3 equal to the mathematical difference.
   */
  [[nodiscard]] constexpr Vector3
  operator-(const Vector3 &other) const noexcept;

  /**
   * @brief Dot product of two vectors.
   * @param other The vector to dot with.
   * @return The scalar dot product.
   */
  [[nodiscard]] constexpr double dot(const Vector3 &other) const noexcept;

  /**
   * @brief Cross product of two vectors.
   * @param other The vector to cross with.
   * @return A new Vector3 equal to the mathematical cross product.
   */
  [[nodiscard]] constexpr Vector3 cross(const Vector3 &other) const noexcept;

private:
  std::array<double, 3> m_data;
};

} // namespace Raytracer::Math
