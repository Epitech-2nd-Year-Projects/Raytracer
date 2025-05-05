/**
 * @file Cone.hpp
 * @brief Definition of the Cone primitive.
 */

#pragma once

#include "Core/APrimitive.hpp"
#include "Math/Point.hpp"
#include "Math/Vector.hpp"
#include <optional>

namespace Raytracer::Primitives {

/**
 * @class Cone
 * @brief Right circular cone (apex at m_position, base at +height × axis).
 *
 * m_position est l’apex du cône.
 * L’axe est déterminé par la chaîne "X", "Y" ou "Z".
 * La base se trouve à une distance m_height de l’apex sur cet axe,
 * avec un rayon m_radius.
 */
class Cone : public Core::APrimitive {
public:
  Cone(const std::string &axis, Math::Point<3> position, double radius,
    double height) noexcept;

  [[nodiscard]] Core::BoundingBox getBoundingBox() const noexcept override;
  [[nodiscard]] std::optional<Core::Intersection>
  intersect(const Core::Ray &ray) const noexcept override;

  [[nodiscard]] const Math::Vector<3> &getAxis() const noexcept { return m_axis; }

private:
  void setAxisPositionRadiusHeight(const std::string &axis,
    const Math::Point<3> &position, double radius, double height) noexcept;

  Math::Vector<3> m_axis{0.0, 1.0, 0.0};
  Math::Point<3> m_position{};
  double m_radius{1.0};
  double m_height{1.0};
};

} // namespace Raytracer::Primitives