/**
 * @file Point.hpp
 * @brief Defines the Point class template for representing mathematical
 * points.
 */

#pragma once

#include "Point.hpp"
#include "Utility/Clamped.hpp"
#include "Vector.hpp"
#include <cmath>
namespace Raytracer::Math {

/**
 * @class Rectangle
 * @brief A class template representing a rectangle in N dimensional space.
 * @tparam N The dimension of the rectangle.
 */
template <std::size_t N> class Rectangle {
public:
  using PointN = Point<N>;
  using VectorN = Vector<N>;

  /**
   * @brief Default constructor initializes the rectangle with default values.
   */
  Rectangle() = default;

  /**
   * @brief Constructor that initializes the rectangle with the given origin,
   * bottom side, and left side.
   * @param origin The origin of the rectangle.
   * @param bottomSide The vector representing the bottom side of the rectangle.
   * @param leftSide The vector representing the left side of the rectangle.
   */
  Rectangle(const PointN &origin, const VectorN &bottomSide,
            const VectorN &leftSide)
      : m_origin(origin), m_bottomSide(bottomSide), m_leftSide(leftSide) {}

  /**
   * @brief Default destructor for the Rectangle class.
   */
  virtual ~Rectangle() noexcept = default;

  /**
   * @brief Get the origin of the rectangle.
   * @return The origin of the rectangle.
   */
  [[nodiscard]] const PointN &getOrigin() const { return m_origin; }

  /**
   * @brief Get the bottom side of the rectangle.
   * @return The vector representing the bottom side of the rectangle.
   */
  [[nodiscard]] const VectorN &getBottomSide() const { return m_bottomSide; }

  /**
   * @brief Get the left side of the rectangle.
   * @return The vector representing the left side of the rectangle.
   */
  [[nodiscard]] const VectorN &getLeftSide() const { return m_leftSide; }

  /**
   * @brief Set the origin of the rectangle.
   * @param newOrigin The new origin of the rectangle.
   */
  void setOrigin(const PointN &newOrigin) { m_origin = newOrigin; }

  /**
   * @brief Set the bottom side of the rectangle.
   * @param newBottomSide The new vector representing the bottom side of the
   * rectangle.
   */
  void setBottomSide(const VectorN &newBottomSide) {
    m_bottomSide = newBottomSide;
  }

  /**
   * @brief Set the left side of the rectangle.
   * @param newLeftSide The new vector representing the left side of the
   * rectangle.
   */
  void setLeftSide(const VectorN &newLeftSide) { m_leftSide = newLeftSide; }

protected:
  PointN m_origin{};
  VectorN m_bottomSide{};
  VectorN m_leftSide{};
};

/**
 * @class Rectangle3D
 * @brief A class representing a rectangle in 3D space.
 */
class Rectangle3D : public Rectangle<3> {
public:
  using Point3D = Point<3>;
  using Vector3D = Vector<3>;

  /**
   * @brief Default constructor initializes the rectangle with default values.
   */
  Rectangle3D() : Rectangle<3>() {}

  /**
   * @brief Constructor that initializes the rectangle with the given origin,
   * bottom side, and left side.
   * @param origin The origin of the rectangle.
   * @param bottomSide The vector representing the bottom side of the rectangle.
   * @param leftSide The vector representing the left side of the rectangle.
   */
  Rectangle3D(const Point3D &origin, const Vector3D &bottomSide,
              const Vector3D &leftSide)
      : Rectangle<3>(origin, bottomSide, leftSide) {}

  /**
   * @brief Default destructor for the Rectangle3D class.
   */
  ~Rectangle3D() noexcept override = default;

  /**
   * @brief Get the point at the given u and v coordinates on the rectangle.
   * @param u The u coordinate (0.0 to 1.0).
   * @param v The v coordinate (0.0 to 1.0).
   * @return The point at the given u and v coordinates on the rectangle.
   */
  [[nodiscard]] Point3D pointAt(Utility::Clamped<double, 0.0, 1.0> u,
                                Utility::Clamped<double, 0.0, 1.0> v) const {
    return m_origin + (m_bottomSide * u.get()) + (m_leftSide * v.get());
  }

  /**
   * @brief Get the normal vector of the rectangle.
   * @return The normal vector of the rectangle.
   */
  [[nodiscard]] Vector3D getNormal() const {
    return m_bottomSide.cross(m_leftSide).normalize();
  }

  /**
   * @brief Get the area of the rectangle.
   * @return The area of the rectangle.
   */
  [[nodiscard]] double getArea() const {
    return m_bottomSide.cross(m_leftSide).length();
  }

  /**
   * @brief Check if the given point is inside the rectangle.
   * @param point The point to check.
   * @return True if the point is inside the rectangle, false otherwise.
   */
  [[nodiscard]] bool contains(const Point3D &point) const {
    Vector3D p = point - m_origin;

    double u = p.dot(m_bottomSide) / m_bottomSide.dot(m_bottomSide);
    double v = p.dot(m_leftSide) / m_leftSide.dot(m_leftSide);

    return u >= 0.0 && u <= 1.0 && v >= 0.0 && v <= 1.0;
  }
};
} // namespace Raytracer::Math
