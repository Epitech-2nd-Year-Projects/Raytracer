/**
 * @file FlatMaterial.hpp
 * @brief Defines the FlatMaterial class for basic surface shading.
 */

#pragma once

#include "Core/AMaterial.hpp"

namespace Raytracer::Materials {

/**
 * @class FlatMaterial
 * @brief Material implementing simple diffuse and ambient shading model.
 */
class FlatMaterial final : public Core::AMaterial {
public:
  /**
   * @brief Default constructor.
   */
  FlatMaterial() noexcept = default;

  /**
   * @brief Construct a flat material with specified properties.
   * @param diffuseColor Diffuse reflectance color.
   * @param ambientColor Ambient reflectance color.
   * @param ambientCoef Ambient coefficient [0.0, 1.0].
   * @param diffuseCoef Diffuse coefficient [0.0, 1.0].
   */
  FlatMaterial(const Core::Color &diffuseColor, const Core::Color &ambientColor,
               double ambientCoef, double diffuseCoef) noexcept;

  /**
   * @brief Compute the color resulting from lighting at an intersection.
   * @param intersection Intersection information.
   * @param ray Incoming ray.
   * @param lights Lights affecting the intersection.
   * @return Computed color.
   */
  [[nodiscard]] Core::Color computeColor(
      const Core::Intersection &intersection, const Core::Ray &ray,
      const std::vector<std::shared_ptr<Core::ILight>> &lights) const override;
};
} // namespace Raytracer::Materials