/**
 * @file Sphere.hpp
 * @brief Defines the Sphere class, representing a 3D spherical primitive object
 * for ray tracing calculations
 */

#pragma once

#include "Core/APrimitive.hpp"
#include "Core/BoundingBox.hpp"
#include "Core/Intersection.hpp"
#include "Core/Ray.hpp"

namespace Raytracer::Primitives {

/**
 * @class Sphere
 * @brief Represents a sphere primitive in 3D space
 */
class Sphere : public Core::APrimitive {
public:
  /**
   * @brief Default constructor
   */
  Sphere() noexcept = default;

  /**
   * @brief Construct a sphere with specified center and radius
   * @param center The center point of the sphere
   * @param radius The radius of the sphere
   */
  Sphere(const Math::Point<3> &center, double radius) noexcept;

  /**
   * @brief Virtual destructor
   */
  ~Sphere() noexcept override = default;

  /**
   * @brief Set the radius of the sphere
   * @param r The new radius value
   */
  void setRadius(double r) noexcept;

  /**
   * @brief Set the center point of the sphere
   * @param center The new center point
   */
  void setCenter(const Math::Point<3> &center) noexcept;

  /**
   * @brief Get the current radius of the sphere
   * @return The radius value
   */
  [[nodiscard]] double getRadius() const noexcept;

  /**
   * @brief Get the current center point of the sphere
   * @return The center point
   */
  [[nodiscard]] const Math::Point<3> &getCenter() const noexcept;

  /**
   * @brief Calculate intersection between a ray and this sphere
   * @param ray The ray to test for intersection
   * @return An optional Intersection object, containing intersection data if
   * the ray hits the sphere
   */
  [[nodiscard]] std::optional<Core::Intersection>
  intersect(const Core::Ray &ray) const noexcept override;

  /**
   * @brief Calculate the axis-aligned bounding box for this sphere
   * @return The bounding box that completely contains this sphere
   */
  [[nodiscard]] Core::BoundingBox getBoundingBox() const noexcept override;

  [[nodiscard]] Math::Point<3> getLocalCenter() const noexcept override;

private:
  double m_radius{1.0};
  Math::Point<3> m_center{};
};
} // namespace Raytracer::Primitives