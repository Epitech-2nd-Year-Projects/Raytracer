/**
 * @file Cone.hpp
 * @brief Defines the Cone class, representing a 3D Cone primitive
 * object for ray tracing calculations
 */

#pragma once

#include "Core/APrimitive.hpp"
#include "Math/Point.hpp"
#include "Math/Vector.hpp"
#include <optional>

namespace Raytracer::Primitives {

/**
 * @class Cone
 * @brief Represents a right circular Cone primitive in 3D space.
 *
 * The apex of the cone is at m_position. The cone's axis is aligned with
 * the X, Y, or Z axis as defined by the axis string. The base of the cone is
 * located at a distance m_height from the apex along the axis, with a radius
 * of m_radius.
 */
class Cone : public Core::APrimitive {
public:
  /**
   * @brief Construct a Cone with specified axis and position
   * @param axis The axis direction of the cone ("X", "Y", or "Z")
   * @param position The apex position of the Cone
   * @param radius The radius of the base of the Cone
   * @param height The height from apex to base of the Cone
   */
  Cone(const std::string &axis, Math::Point<3> position, double radius,
       double height) noexcept;

  /**
   * @brief Calculate the axis-aligned bounding box for this Cone
   * @return The bounding box that represents this Cone
   */
  [[nodiscard]] Core::BoundingBox getBoundingBox() const noexcept override;

  /**
   * @brief Calculate intersection between a ray and this Cone
   * @param ray The ray to test for intersection
   * @return An optional Intersection object, containing intersection data if
   * the ray hits the Cone
   */
  [[nodiscard]] std::optional<Core::Intersection>
  intersect(const Core::Ray &ray) const noexcept override;

  /**
   * @brief Get the axis vector of the Cone
   * @return The normalized axis vector
   */
  [[nodiscard]] const Math::Vector<3> &getAxis() const noexcept {
    return m_axis;
  }

  [[nodiscard]] Math::Point<3> getLocalCenter() const noexcept override;

private:
  /**
   * @brief Set the axis, apex position, radius and height of the Cone
   * @param axis The axis direction ("X", "Y", or "Z")
   * @param position The apex position
   * @param radius The radius of the base
   * @param height The height from apex to base
   */
  void setAxisPositionRadiusHeight(const std::string &axis,
                                   const Math::Point<3> &position,
                                   double radius, double height) noexcept;

  Math::Vector<3> m_axis{0.0, 1.0, 0.0};
  Math::Point<3> m_position{};
  double m_radius{1.0};
  double m_height{1.0};
};

} // namespace Raytracer::Primitives
