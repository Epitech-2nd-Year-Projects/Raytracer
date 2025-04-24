/**
 * @file Intersection.hpp
 * @brief Defines intersection data for ray-primitive tests.
 */

#pragma once

#include "Math/Point.hpp"
#include "Math/Vector.hpp"
#include <memory>

namespace Raytracer::Core {
class IMaterial;

/**
 * @class Intersection
 * @brief Contains information about a ray-primitive intersection.
 */
class Intersection final {
public:
  /**
   * @brief Default constructor, creates an empty intersection.
   */
  constexpr Intersection() noexcept = default;

  /**
   * @brief Construct intersection with specified parameters.
   * @param point Point of intersection in world space.
   * @param normal Surface normal at the intersection point.
   * @param material Material of the intersected primitive.
   * @param distance Distance from ray origin to intersection.
   * @param isInside True if the ray origin was inside the primitive.
   * @param uv Texture coordinates at the intersection.
   */
  Intersection(const Math::Point<3> &point, const Math::Vector<3> &normal,
               std::unique_ptr<IMaterial> material, double distance,
               bool isInside, const Math::Point<2> &uv) noexcept;

  /**
   * @brief Get the intersection point.
   * @return Point of intersection.
   */
  [[nodiscard]] constexpr const Math::Point<3> &getPoint() const noexcept;

  /**
   * @brief Get the surface normal at the intersection.
   * @return Normal vector.
   */
  [[nodiscard]] constexpr const Math::Vector<3> &getNormal() const noexcept;

  /**
   * @brief Get the material at the intersection.
   * @return Unique pointer to the material.
   */
  [[nodiscard]] std::unique_ptr<IMaterial> getMaterial() const noexcept;

  /**
   * @brief Get distance from ray origin.
   * @return Distance value.
   */
  [[nodiscard]] constexpr double getDistance() const noexcept;

  /**
   * @brief Determine if intersection was from inside the primitive.
   * @return True if inside.
   */
  [[nodiscard]] constexpr bool getIsInside() const noexcept;

  /**
   * @brief Get texture coordinates at intersection.
   * @return UV coordinates.
   */
  [[nodiscard]] constexpr const Math::Point<2> &getUv() const noexcept;

private:
  Math::Point<3> m_point{};
  Math::Vector<3> m_normal{};
  std::unique_ptr<IMaterial> m_material{nullptr};
  double m_distance{0.0};
  bool m_isInside{false};
  Math::Point<2> m_uv{0.0, 0.0};
};
} // namespace Raytracer::Core
