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
  constexpr Ray(const Math::Point<3> &origin,
                const Math::Vector<3> &direction) noexcept;

  /**
   * @brief Construct ray with origin, direction, and distance range.
   * @param origin Ray origin.
   * @param direction Ray direction.
   * @param minDistance Minimum valid distance along the ray.
   * @param maxDistance Maximum valid distance along the ray.
   */
  constexpr Ray(const Math::Point<3> &origin,
                const Math::Vector<3> &direction,
                double minDistance, double maxDistance) noexcept;

  /**
   * @brief Get ray origin.
   * @return Origin point.
   */
  [[nodiscard]] constexpr Math::Point<3> getOrigin() const noexcept;

  /**
   * @brief Get ray direction.
   * @return Direction vector.
   */
  [[nodiscard]] constexpr Math::Vector<3> getDirection() const noexcept;

  /**
   * @brief Get minimum distance.
   * @return Minimum distance along ray.
   */
  [[nodiscard]] constexpr double getMinDistance() const noexcept;

  /**
   * @brief Get maximum distance.
   * @return Maximum distance along ray.
   */
  [[nodiscard]] constexpr double getMaxDistance() const noexcept;

  /**
   * @brief Compute point at parameter t along the ray.
   * @param t Parameter value.
   * @return Point at origin + t * direction.
   */
  [[nodiscard]] constexpr Math::Point<3> at(double t) const noexcept;

  /**
   * @brief Normalize the direction vector.
   */
  constexpr void normalizeDirection() noexcept;

private:
  Math::Point<3> m_origin{};
  Math::Vector<3> m_direction{};
  double m_minDistance{0.0};
  double m_maxDistance{100.0};
};

} // namespace Raytracer::Core
