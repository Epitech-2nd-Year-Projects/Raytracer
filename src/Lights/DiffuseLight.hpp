/**
 * @file DiffuseLight.hpp
 * @brief Defines the DiffuseLight class for diffuse illumination
 */

#pragma once

#include "Core/ALight.hpp"

namespace Raytracer::Lights {

/**
 * @class DiffuseLight
 * @brief Diffuse light that provides directional illumination
 */
class DiffuseLight final : public Core::ALight {
public:
  /**
   * @brief Default constructor
   */
  DiffuseLight() noexcept : ALight(1.0, Core::Color(255.0, 255.0, 255.0)) {}

  /**
   * @brief Constructor with specified intensity
   * @param intensity The intensity value for the diffuse light
   */
  explicit DiffuseLight(double intensity) noexcept
      : ALight(intensity, Core::Color(255.0, 255.0, 255.0)) {}

  /**
   * @brief Computes the illumination at the specified intersection point
   * @param intersectionPoint The point of intersection
   * @param normal The surface normal at the intersection point
   * @return The illumination value (equal to the light's intensity)
   */
  [[nodiscard]] double computeIllumination(
      [[maybe_unused]] const Math::Point<3> &intersectionPoint,
      [[maybe_unused]] const Math::Vector<3> &normal) const noexcept override {
    return getIntensity();
  }

  /**
   * @brief Gets the direction from a point to the light source
   * @param point The reference point (unused for diffuse light)
   * @return A fixed directional vector (0, -1, 0)
   */
  [[nodiscard]] Math::Vector<3> getDirectionFrom(
      [[maybe_unused]] const Math::Point<3> &point) const noexcept {
    return Math::Vector<3>(0.0, -1.0, 0.0);
  }

  /**
   * @brief Determines whether this light casts shadows
   * @return Always returns false
   */
  [[nodiscard]] bool castsShadow() const noexcept override { return false; }
};

} // namespace Raytracer::Lights
