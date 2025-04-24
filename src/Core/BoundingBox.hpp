/**
 * @file BoundingBox.hpp
 * @brief Defines the axis-aligned bounding box class.
 */

#pragma once

#include "Core/Ray.hpp"
#include "Math/Point.hpp"

namespace Raytracer::Core {

/**
 * @class BoundingBox
 * @brief Axis-aligned bounding box for spatial queries.
 */
class BoundingBox final {
public:
  /**
   * @brief Default constructor.
   */
  constexpr BoundingBox() noexcept = default;

  /**
   * @brief Construct with given bounds.
   * @param min Minimum corner of the box.
   * @param max Maximum corner of the box.
   */
  constexpr BoundingBox(const Math::Point<3> &min,
                        const Math::Point<3> &max) noexcept;

  /**
   * @brief Test intersection with a ray.
   * @param ray Ray to test against.
   * @return true if the ray intersects the box.
   */
  [[nodiscard]] bool intersect(const Ray &ray) const noexcept;

  /**
   * @brief Compute the union of this box and another.
   * @param other Other bounding box to unite with.
   * @return Bounding box that encloses both.
   */
  [[nodiscard]] BoundingBox unite(const BoundingBox &other) const noexcept;

  /**
   * @brief Check if a point is inside the box.
   * @param point Point to test.
   * @return true if the point lies within the bounds.
   */
  [[nodiscard]] constexpr bool
  contains(const Math::Point<3> &point) const noexcept;

  /**
   * @brief Get the minimum corner.
   * @return Reference to the minimum point.
   */
  [[nodiscard]] constexpr Math::Point<3> &getMin() const noexcept;

  /**
   * @brief Get the maximum corner.
   * @return Reference to the maximum point.
   */
  [[nodiscard]] constexpr Math::Point<3> &getMax() const noexcept;

private:
  Math::Point<3> m_min{};
  Math::Point<3> m_max{};
};

} // namespace Raytracer::Core
