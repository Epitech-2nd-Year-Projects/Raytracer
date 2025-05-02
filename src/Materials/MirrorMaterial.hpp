/**
* @file MirrorMaterial.hpp
 * @brief Defines the MirrorMaterial class for Mirror surface shading.
 */

#pragma once

#include "Core/AMaterial.hpp"

namespace Raytracer::Materials {

/**
 * @class MirrorMaterial
 * @brief Material implementing simple diffuse and ambient shading model.
 */
class MirrorMaterial final : public Core::AMaterial {
public:
  /**
   * @brief Default constructor.
   */
  MirrorMaterial() noexcept = default;

  /**
   * @brief Construct a flat material with specified properties.
   * @param diffuseColor Diffuse reflectance color.
   * @param ambientColor Ambient reflectance color.
   * @param ambientCoef Ambient coefficient [0.0, 1.0].
   * @param diffuseCoef Diffuse coefficient [0.0, 1.0].
   */
  MirrorMaterial(const Core::Color &diffuseColor, const Core::Color &ambientColor,
               double ambientCoef, double diffuseCoef) noexcept;

  /**
   * @brief Compute the color resulting from lighting at an intersection.
   * @param intersection Intersection information.
   * @param ray Incoming ray.
   * @param lights Lights affecting the intersection.
   * @param scene Scene containing all objects.
   * @return Computed color.
   */
  [[nodiscard]] Core::Color
  computeColor(const Core::Intersection &intersection, const Core::Ray &ray,
               const std::vector<std::shared_ptr<Core::ILight>> &lights,
               const Core::Scene &scene) const override;
};
} // namespace Raytracer::Materials
