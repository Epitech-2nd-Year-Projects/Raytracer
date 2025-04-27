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
               std::shared_ptr<IMaterial> material, double distance,
               bool isInside, const Math::Point<2> &uv) noexcept
      : m_point(point), m_normal(normal), m_material(std::move(material)),
        m_distance(distance), m_isInside(isInside), m_uv(uv) {}

  /**
   * @brief Get the intersection point.
   * @return Point of intersection.
   */
  [[nodiscard]] const Math::Point<3> &getPoint() const noexcept {
    return m_point;
  }

  /**
   * @brief Get the surface normal at the intersection.
   * @return Normal vector.
   */
  [[nodiscard]] const Math::Vector<3> &getNormal() const noexcept {
    return m_normal;
  }

  /**
   * @brief Get the material at the intersection.
   * @return Shared pointer to the material.
   */
  [[nodiscard]] std::shared_ptr<IMaterial> getMaterial() const noexcept {
    return m_material;
  }

  /**
   * @brief Get distance from ray origin.
   * @return Distance value.
   */
  [[nodiscard]] double getDistance() const noexcept { return m_distance; }

  /**
   * @brief Determine if intersection was from inside the primitive.
   * @return True if inside.
   */
  [[nodiscard]] bool getIsInside() const noexcept { return m_isInside; }

  /**
   * @brief Get texture coordinates at intersection.
   * @return UV coordinates.
   */
  [[nodiscard]] const Math::Point<2> &getUv() const noexcept { return m_uv; }

private:
  Math::Point<3> m_point{};
  Math::Vector<3> m_normal{};
  std::shared_ptr<IMaterial> m_material{nullptr};
  double m_distance{0.0};
  bool m_isInside{false};
  Math::Point<2> m_uv{0.0, 0.0};
};
} // namespace Raytracer::Core
