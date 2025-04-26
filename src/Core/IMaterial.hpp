
/**
 * @file IMaterial.hpp
 * @brief Defines the material interface for surface shading.
 */

#pragma once

#include "Core/Color.hpp"
#include "Core/Scene.hpp"
#include <memory>
#include <vector>

namespace Raytracer::Core {

class ILight;
class Intersection;
class Ray;

/**
 * @class IMaterial
 * @brief Interface for materials that define how surfaces interact with light.
 */
class IMaterial {
public:
  virtual ~IMaterial() = default;

  /**
   * @brief Compute the color resulting from lighting at an intersection.
   * @param intersection Intersection information.
   * @param ray Incoming ray.
   * @param lights Lights affecting the intersection.
   * @param scene Scene containing all objects.
   * @return Computed color.
   */
  virtual Color computeColor(const Intersection &intersection, const Ray &ray,
                             const std::vector<std::shared_ptr<ILight>> &lights,
                             const Core::Scene &scene) const = 0;

  /**
   * @brief Get the diffuse color.
   * @return Diffuse color.
   */
  [[nodiscard]] virtual const Color &getDiffuseColor() const noexcept = 0;

  /**
   * @brief Get the ambient color.
   * @return Ambient color.
   */
  [[nodiscard]] virtual const Color &getAmbientColor() const noexcept = 0;

  /**
   * @brief Get the ambient coefficient.
   * @return Ambient coefficient [0.0, 1.0].
   */
  [[nodiscard]] virtual double getAmbientCoefficient() const noexcept = 0;

  /**
   * @brief Get the diffuse coefficient.
   * @return Diffuse coefficient [0.0, 1.0].
   */
  [[nodiscard]] virtual double getDiffuseCoefficient() const noexcept = 0;

  /**
   * @brief Set the diffuse color.
   * @param color New diffuse color.
   */
  virtual void setDiffuseColor(const Color &color) noexcept = 0;

  /**
   * @brief Set the ambient color.
   * @param color New ambient color.
   */
  virtual void setAmbientColor(const Color &color) noexcept = 0;

  /**
   * @brief Set the ambient coefficient.
   * @param coefficient New ambient coefficient [0.0, 1.0].
   */
  virtual void setAmbientCoefficient(double coefficient) noexcept = 0;

  /**
   * @brief Set the diffuse coefficient.
   * @param coefficient New diffuse coefficient [0.0, 1.0].
   */
  virtual void setDiffuseCoefficient(double coefficient) noexcept = 0;
};

} // namespace Raytracer::Core
