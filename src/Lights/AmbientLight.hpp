/**
 * @file AmbientLight.hpp
 * @brief Defines the AmbientLight class for global ambient illumination
 */

#pragma once

#include "Core/ALight.hpp"

namespace Raytracer::Lights {
/**
 * @class AmbientLight
 * @brief Ambient light that provides constant illumination in all directions
 */
class AmbientLight final : public Core::ALight, public Core::IAmbientLight {
public:
  /**
   * @brief Default constructor
   */
  AmbientLight() noexcept : ALight(1.0, Core::Color(255.0, 255.0, 255.0)) {}

  /**
   * @brief Constructor with specified intensity
   * @param intensity The intensity value for the ambient light
   */
  explicit AmbientLight(double intensity) noexcept
      : ALight(intensity, Core::Color(255.0, 255.0, 255.0)) {}

  /**
   * @brief Computes the illumination at the specified intersection point
   * @param intersectionPoint The point of intersection (unused for ambient
   * light)
   * @param normal The surface normal at the intersection point (unused for
   * ambient light)
   * @return The illumination value (equal to the light's intensity)
   */
  [[nodiscard]] double computeIllumination(
      [[maybe_unused]] const Math::Point<3> &intersectionPoint,
      [[maybe_unused]] const Math::Vector<3> &normal) const noexcept override {
    return getIntensity();
  }

  /**
   * @brief Gets the direction from a point to the light source
   * @param point The reference point (unused for ambient light)
   * @return A fixed directional vector (0, 0, 1)
   */
  [[nodiscard]] Math::Vector<3> getDirectionFrom(
      [[maybe_unused]] const Math::Point<3> &point) const noexcept {
    return Math::Vector<3>(0.0, 0.0, 1.0);
  }

  /**
   * @brief Determines whether this light casts shadows
   * @return Always returns false
   */
  [[nodiscard]] bool castsShadow() const noexcept override { return false; }
};
} // namespace Raytracer::Lights
