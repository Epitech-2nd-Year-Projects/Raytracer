
/**
 * @file AMaterial.hpp
 * @brief Defines the abstract base material class with diffuse and ambient
 * properties.
 */

#pragma once

#include "Core/Color.hpp"
#include "Core/ILight.hpp"
#include "Core/IMaterial.hpp"
#include "Core/Intersection.hpp"
#include "Core/Ray.hpp"
#include "Utility/Clamped.hpp"

#include <vector>

namespace Raytracer::Core {

/**
 * @class AMaterial
 * @brief Abstract base material class providing diffuse and ambient components.
 */
class AMaterial : public IMaterial {
public:
  /**
   * @brief Default constructor.
   */
  AMaterial() noexcept = default;

  /**
   * @brief Construct material with specified colors and coefficients.
   * @param diffuseColor Diffuse reflectance color.
   * @param ambientColor Ambient reflectance color.
   * @param ambientCoef Ambient coefficient.
   * @param diffuseCoef Diffuse coefficient.
   */
  AMaterial(const Color &diffuseColor, const Color &ambientColor,
            double ambientCoef, double diffuseCoef) noexcept
      : m_diffuseColor(diffuseColor), m_ambientColor(ambientColor),
        m_ambientCoefficient(ambientCoef), m_diffuseCoefficient(diffuseCoef) {}

  /**
   * @brief Destructor.
   */
  ~AMaterial() noexcept override = default;

  AMaterial(const AMaterial &) = delete;
  AMaterial(AMaterial &&) = delete;
  AMaterial &operator=(const AMaterial &) = delete;
  AMaterial &operator=(AMaterial &&) = delete;

  /**
   * @brief Compute the color resulting from lighting at an intersection.
   * @param intersection Intersection information.
   * @param ray Incoming ray.
   * @param lights Lights affecting the intersection.
   * @param scene Scene containing all objects.
   * @return Computed color.
   */
  [[nodiscard]] Color
  computeColor(const Intersection &intersection, const Ray &ray,
               const std::vector<std::shared_ptr<ILight>> &lights,
               const Scene &scene) const override = 0;

  /**
   * @brief Get the diffuse color.
   * @return Diffuse color.
   */
  [[nodiscard]] const Color &getDiffuseColor() const noexcept override {
    return m_diffuseColor;
  }

  /**
   * @brief Get the ambient color.
   * @return Ambient color.
   */
  [[nodiscard]] const Color &getAmbientColor() const noexcept override {
    return m_ambientColor;
  }

  /**
   * @brief Get the ambient coefficient.
   * @return Ambient coefficient.
   */
  [[nodiscard]] double getAmbientCoefficient() const noexcept override {
    return m_ambientCoefficient.get();
  }

  /**
   * @brief Get the diffuse coefficient.
   * @return Diffuse coefficient.
   */
  [[nodiscard]] double getDiffuseCoefficient() const noexcept override {
    return m_diffuseCoefficient.get();
  }

  /**
   * @brief Set the diffuse color.
   * @param color New diffuse color.
   */
  void setDiffuseColor(const Color &color) noexcept override {
    m_diffuseColor = color;
  }

  /**
   * @brief Set the ambient color.
   * @param color New ambient color.
   */
  void setAmbientColor(const Color &color) noexcept override {
    m_ambientColor = color;
  }

  /**
   * @brief Set the ambient coefficient.
   * @param coefficient New ambient coefficient.
   */
  void setAmbientCoefficient(double coefficient) noexcept override {
    m_ambientCoefficient = coefficient;
  }

  /**
   * @brief Set the diffuse coefficient.
   * @param coefficient New diffuse coefficient.
   */
  void setDiffuseCoefficient(double coefficient) noexcept override {
    m_diffuseCoefficient = coefficient;
  }

private:
  Color m_diffuseColor{};
  Color m_ambientColor{};
  Utility::Clamped<double, 0.0, 1.0> m_ambientCoefficient{0.0};
  Utility::Clamped<double, 0.0, 1.0> m_diffuseCoefficient{0.0};
};

} // namespace Raytracer::Core
