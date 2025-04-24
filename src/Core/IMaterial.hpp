/**
 * @file IMaterial.hpp
 * @brief Defines the material interface for computing surface shading.
 */

#pragma once

#include "Core/Color.hpp"
#include "Core/ILight.hpp"
#include "Core/Intersection.hpp"
#include "Core/Ray.hpp"

#include <vector>

namespace Raytracer::Core {

/**
 * @class IMaterial
 * @brief Interface for materials defining shading behavior.
 */
class IMaterial {
public:
  /**
   * @brief Virtual destructor.
   */
  virtual ~IMaterial() = default;

  /**
   * @brief Compute the color resulting from lighting at an intersection.
   * @param intersection Intersection information.
   * @param ray Incoming ray.
   * @param lights Lights affecting the intersection.
   * @return Computed shading color.
   */
  [[nodiscard]] virtual Color computeColor(
      const Intersection &intersection,
      const Ray &ray,
      const std::vector<std::shared_ptr<ILight>> &lights) const = 0;

  /**
   * @brief Get the diffuse reflectance color.
   * @return Diffuse color.
   */
  [[nodiscard]] virtual const Color &getDiffuseColor() const noexcept = 0;

  /**
   * @brief Get the ambient reflectance color.
   * @return Ambient color.
   */
  [[nodiscard]] virtual const Color &getAmbientColor() const noexcept = 0;

  /**
   * @brief Get the ambient coefficient.
   * @return Ambient coefficient.
   */
  [[nodiscard]] virtual double getAmbientCoefficient() const noexcept = 0;

  /**
   * @brief Get the diffuse coefficient.
   * @return Diffuse coefficient.
   */
  [[nodiscard]] virtual double getDiffuseCoefficient() const noexcept = 0;

  /**
   * @brief Set the diffuse reflectance color.
   * @param color New diffuse color.
   */
  virtual void setDiffuseColor(const Color &color) noexcept = 0;

  /**
   * @brief Set the ambient reflectance color.
   * @param color New ambient color.
   */
  virtual void setAmbientColor(const Color &color) noexcept = 0;

  /**
   * @brief Set the ambient coefficient.
   * @param coefficient New ambient coefficient.
   */
  virtual void setAmbientCoefficient(double coefficient) noexcept = 0;

  /**
   * @brief Set the diffuse coefficient.
   * @param coefficient New diffuse coefficient.
   */
  virtual void setDiffuseCoefficient(double coefficient) noexcept = 0;
};

} // namespace Raytracer::Core
