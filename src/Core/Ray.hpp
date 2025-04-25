/**
 * @file Ray.hpp
 * @brief Defines the Ray class for ray tracing.
 */

#pragma once

#include "Math/Point.hpp"
#include "Math/Vector.hpp"

namespace Raytracer::Core {

/**
 * @class Ray
 * @brief Represents a ray with origin, direction, and distance bounds.
 */
class Ray final {
public:
  /**
   * @brief Default constructor.
   */
  constexpr Ray() noexcept = default;

  /**
   * @brief Construct ray with origin and direction.
   * @param origin Ray origin.
   * @param direction Ray direction.
   */
  Ray(const Math::Point<3> &origin, const Math::Vector<3> &direction) noexcept
      : m_origin(origin), m_direction(direction) {}

  /**
   * @brief Construct ray with origin, direction, and distance range.
   * @param origin Ray origin.
   * @param direction Ray direction.
   * @param minDistance Minimum valid distance along the ray.
   * @param maxDistance Maximum valid distance along the ray.
   */
  Ray(const Math::Point<3> &origin, const Math::Vector<3> &direction,
      double minDistance, double maxDistance) noexcept
      : m_origin(origin), m_direction(direction), m_minDistance(minDistance),
        m_maxDistance(maxDistance) {}

  /**
   * @brief Get ray origin.
   * @return Origin point.
   */
  [[nodiscard]] Math::Point<3> getOrigin() const noexcept { return m_origin; }

  /**
   * @brief Get ray direction.
   * @return Direction vector.
   */
  [[nodiscard]] Math::Vector<3> getDirection() const noexcept {
    return m_direction;
  }

  /**
   * @brief Get minimum distance.
   * @return Minimum distance along ray.
   */
  [[nodiscard]] double getMinDistance() const noexcept { return m_minDistance; }

  /**
   * @brief Get maximum distance.
   * @return Maximum distance along ray.
   */
  [[nodiscard]] double getMaxDistance() const noexcept { return m_maxDistance; }

  /**
   * @brief Compute point at parameter t along the ray.
   * @param t Parameter value.
   * @return Point at origin + t * direction.
   */
  [[nodiscard]] Math::Point<3> at(double t) const noexcept {
    return m_origin + m_direction * t;
  }

  /**
   * @brief Normalize the direction vector.
   */
  void normalizeDirection() noexcept {
    double length = m_direction.length();
    if (length > 0) {
      m_direction = m_direction / length;
    }
  }

private:
  Math::Point<3> m_origin{};
  Math::Vector<3> m_direction{};
  double m_minDistance{0.0};
  double m_maxDistance{100.0};
};

} // namespace Raytracer::Core
