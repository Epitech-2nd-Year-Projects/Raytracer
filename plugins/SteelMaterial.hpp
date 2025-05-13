/**
 * @file SteelMaterial.hpp
 * @brief Defines the SteelMaterial class for brushed steel surface shading.
 */

#pragma once

#include "Core/AMaterial.hpp"
#include <random>

namespace Raytracer::Materials {

/**
 * @class SteelMaterial
 * @brief Material implementing brushed steel effect with fuzzy reflections.
 */
class SteelMaterial final : public Core::AMaterial {
public:
  /**
   * @brief Default constructor.
   */
  SteelMaterial() noexcept = default;

  /**
   * @brief Construct a steel material with specified properties.
   * @param diffuseColor Diffuse reflectance color.
   * @param ambientColor Ambient reflectance color.
   * @param ambientCoef Ambient coefficient [0.0, 1.0].
   * @param diffuseCoef Diffuse coefficient [0.0, 1.0].
   * @param fuzz Fuzziness factor for reflection [0.0, 1.0].
   */
  SteelMaterial(const Core::Color &diffuseColor,
                const Core::Color &ambientColor, double ambientCoef,
                double diffuseCoef, double fuzz = 0.3) noexcept;

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

private:
  /**
   * @brief Generate a random point in unit sphere for fuzz calculations.
   * @return A random vector inside unit sphere.
   */
  [[nodiscard]] Math::Vector<3> randomInUnitSphere() const;

  double m_fuzz; // Fuzziness factor for the brushed steel effect
  mutable std::mt19937 m_generator;
  mutable std::uniform_real_distribution<double> m_distribution;
};
} // namespace Raytracer::Materials