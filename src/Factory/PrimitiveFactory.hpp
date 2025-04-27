#pragma once

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
};
} // namespace Raytracer::Factory
