/**
 * @file Cylinder.hpp
 * @brief Defines the Cylinder class, representing a 3D Cylinder primitive
 * object for ray tracing calculations
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
 * @class Cylinder
 * @brief Represents an infinite Cylinder primitive in 3D space
 */
class Cylinder : public Core::APrimitive {
public:
  /**
   * @brief Default constructor
   */
  Cylinder() noexcept = default;

  /**
   * @brief Construct a Cylinder with specified axis and position
   * @param axis The axis perpendicular to the Cylinder ("X", "Y", or "Z")
   * @param position The position along the axis where the Cylinder intersects
   * @param radius The radius of the Cylinder around the axis
   * @param height The height of the Cylinder
   */
  Cylinder(const std::string &axis, Math::Point<3> position, double radius,
           double height) noexcept;

  /**
   * @brief Virtual destructor
   */
  ~Cylinder() noexcept override = default;

  /**
   * @brief Set the axis and position of the Cylinder
   * @param axis The axis perpendicular to the Cylinder ("X", "Y", or "Z")
   * @param position The position along the axis where the Cylinder intersects
   * @param radius The radius of the Cylinder around the axis
   * @param height The height of the Cylinder
   */
  void setAxisPositionRadiusAndHeight(const std::string &axis,
                                      Math::Point<3> position, double radius,
                                      double height) noexcept;

  /**
   * @brief Get the normal vector of the Cylinder
   * @return The normalized normal vector
   */
  [[nodiscard]] const Math::Vector<3> &getNormal() const noexcept;

  /**
   * @brief Get the position of the Cylinder along its axis
   * @return The position value
   */
  [[nodiscard]] const Math::Point<3> &getPosition() const noexcept override;

  /**
   * @brief Calculate intersection between a ray and this Cylinder
   * @param ray The ray to test for intersection
   * @return An optional Intersection object, containing intersection data if
   * the ray hits the Cylinder
   */
  [[nodiscard]] std::optional<Core::Intersection>
  intersect(const Core::Ray &ray) const noexcept override;

  /**
   * @brief Calculate the axis-aligned bounding box for this Cylinder
   * @return The bounding box that represents this infinite Cylinder
   */
  [[nodiscard]] Core::BoundingBox getBoundingBox() const noexcept override;

  [[nodiscard]] Math::Point<3> getLocalCenter() const noexcept override;

private:
  Math::Vector<3> m_normal{0.0, 0.0, 0.0};
  Math::Point<3> m_position{0.0, 0.0, 0.0};
  double m_radius{1.0};
  double m_height{1.0};
};

} // namespace Raytracer::Primitives