/**
 * @file PointLight.hpp
 * @brief Defines the PointLight class for point source illumination
 */

#pragma once

#include "Core/ALight.hpp"
#include "Core/Scene.hpp"

namespace Raytracer::Lights {

/**
 * @class PointLight
 * @brief Point light source that emits light in all directions from a position
 */
class PointLight final : public Core::APositionalLight {
public:
  /**
   * @brief Default constructor
   */
  PointLight() noexcept = default;

  /**
   * @brief Constructor with specified position
   * @param position The position of the point light
   */
  explicit PointLight(const Math::Point<3> &position) noexcept
      : APositionalLight(position) {}

  /**
   * @brief Constructor with specified position and intensity
   * @param position The position of the point light
   * @param intensity The intensity of the light [0.0, 1.0]
   */
  PointLight(const Math::Point<3> &position, double intensity) noexcept
      : APositionalLight(position) {
    setIntensity(intensity);
  }

  /**
   * @brief Constructor with specified position, intensity, and color
   * @param position The position of the point light
   * @param intensity The intensity of the light [0.0, 1.0]
   * @param color The color of the light
   */
  PointLight(const Math::Point<3> &position, double intensity,
             const Core::Color &color) noexcept
      : APositionalLight(position) {
    setIntensity(intensity);
    setColor(color);
  }

  /**
   * @brief Computes the illumination considering shadows
   * @param intersectionPoint The point of intersection
   * @param normal The surface normal at the intersection point
   * @param scene The scene containing all objects
   * @return The illumination value considering shadows
   */
  [[nodiscard]] double
  computeIllumination(const Math::Point<3> &intersectionPoint,
                      const Math::Vector<3> &normal,
                      const Core::Scene &scene) const noexcept override {
    Math::Vector<3> lightDirection = getDirectionFrom(intersectionPoint);

    const double epsilon = 1e-4;
    Math::Point<3> shadowRayOrigin = intersectionPoint + normal * epsilon;
    double distanceToLight = getDistance(intersectionPoint);

    Core::Ray shadowRay(shadowRayOrigin, lightDirection, epsilon,
                        distanceToLight);

    if (scene.hasIntersection(shadowRay)) {
      return 0.0;
    }

    return APositionalLight::computeIllumination(intersectionPoint, normal);
  }

  /**
   * @brief Determines whether this light casts shadows
   * @return Always returns true for point lights
   */
  [[nodiscard]] bool castsShadow() const noexcept override { return true; }
};

} // namespace Raytracer::Lights
