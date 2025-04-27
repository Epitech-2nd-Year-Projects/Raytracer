/**
 * @file Plane.hpp
 * @brief Defines the Plane class, representing a 3D plane primitive object
 * for ray tracing calculations
 */

#pragma once

#include "Core/APrimitive.hpp"
#include "Core/BoundingBox.hpp"
#include "Core/Intersection.hpp"
#include "Core/Ray.hpp"
#include "Math/Point.hpp"
#include <string>

namespace Raytracer::Primitives {

/**
 * @class Plane
 * @brief Represents an infinite plane primitive in 3D space
 */
class Plane : public Core::APrimitive {
public:
  /**
   * @brief Default constructor
   */
  Plane() noexcept = default;

  /**
   * @brief Construct a plane with specified axis and position
   * @param axis The axis perpendicular to the plane ("X", "Y", or "Z")
   * @param position The position along the axis where the plane intersects
   */
  Plane(const std::string &axis, Math::Point<3> position) noexcept;

  /**
   * @brief Virtual destructor
   */
  ~Plane() noexcept override = default;

  /**
   * @brief Set the axis and position of the plane
   * @param axis The axis perpendicular to the plane ("X", "Y", or "Z")
   * @param position The position along the axis where the plane intersects
   */
  void setAxisAndPosition(const std::string &axis,
                          Math::Point<3> position) noexcept;

  /**
   * @brief Get the normal vector of the plane
   * @return The normalized normal vector
   */
  [[nodiscard]] const Math::Vector<3> &getNormal() const noexcept;

  /**
   * @brief Get the position of the plane along its axis
   * @return The position value
   */
  [[nodiscard]] const Math::Point<3> &getPosition() const noexcept override;

  /**
   * @brief Calculate intersection between a ray and this plane
   * @param ray The ray to test for intersection
   * @return An optional Intersection object, containing intersection data if
   * the ray hits the plane
   */
  [[nodiscard]] std::optional<Core::Intersection>
  intersect(const Core::Ray &ray) const noexcept override;

  /**
   * @brief Calculate the axis-aligned bounding box for this plane
   * @return The bounding box that represents this infinite plane
   */
  [[nodiscard]] Core::BoundingBox getBoundingBox() const noexcept override;

private:
  Math::Vector<3> m_normal{0.0, 0.0, 0.0};
  Math::Point<3> m_position{0.0, 0.0, 0.0};
};
} // namespace Raytracer::Primitives
