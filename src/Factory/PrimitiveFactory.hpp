#pragma once

#include "Primitives/Cone.hpp"
#include "Primitives/Cylinder.hpp"
#include "Primitives/Plane.hpp"
#include "Primitives/Sphere.hpp"

namespace Raytracer::Factory {

/**
 * @class PrimitiveFactory
 * @brief Factory for creating different types of primitives
 */
class PrimitiveFactory {
public:
  /**
   * @brief Create a sphere primitive
   */
  [[nodiscard]] static std::unique_ptr<Core::IPrimitive>
  createSphere(const Math::Point<3> &center, double radius) {
    return std::make_unique<Primitives::Sphere>(center, radius);
  }

  /**
   * @brief Create a plane primitive
   */
  [[nodiscard]] static std::unique_ptr<Core::IPrimitive>
  createPlane(const std::string &axis, const Math::Point<3> &position) {
    return std::make_unique<Primitives::Plane>(axis, position);
  }

  /**
   * @brief Create a cylinder primitive
   */
  [[nodiscard]] static std::unique_ptr<Core::IPrimitive>
  createCylinder(const std::string &axis, const Math::Point<3> &position,
                 const double radius, const double height) {
    return std::make_unique<Primitives::Cylinder>(axis, position, radius,
                                                  height);
  }

  /**
   * @brief Create a cone primitive
   *
   * @param axis     "X", "Y" ou "Z" (orientation de l’axe)
   * @param position Apex du cône
   * @param radius   Rayon de la base
   * @param height   Hauteur (distance apex → base)
   */
  [[nodiscard]] static std::unique_ptr<Core::IPrimitive>
  createCone(const std::string &axis, const Math::Point<3> &position,
             double radius, double height) {
    return std::make_unique<Primitives::Cone>(axis, position, radius, height);
  }
};

} // namespace Raytracer::Factory