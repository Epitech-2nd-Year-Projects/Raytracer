#pragma once
#include "Plugin/MaterialPlugin.hpp"

namespace Raytracer::Plugins {

/**
 * @class MirrorMaterialPlugin
 * @brief Plugin for creating a mirror material
 */
class MirrorMaterialPlugin : public Plugin::MaterialPlugin {
public:
  /**
   * @brief Default constructor
   */
  MirrorMaterialPlugin() noexcept = default;

  /**
   * @brief Virtual destructor
   */
  ~MirrorMaterialPlugin() noexcept override = default;

  /**
   * @brief Get the name of this plugin
   * @return The name of the plugin
   */
  [[nodiscard]]
  std::string getName() const override {
    return "MirrorMaterial";
  }

  /**
   * @brief Create a new instance of the flat material primitive plugin
   * @return A unique pointer to the new flat material primitive
   */
  std::shared_ptr<MaterialPlugin> create() override;

  /**
   * @brief Configure the flat material plugin with a libconfig setting
   * @param config The libconfig setting to configure the flat material
   * @return True if the configuration was successful, false otherwise
   */
  bool configure(const libconfig::Setting &config) override;

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
   * @brief Compute the color resulting from reflection at an intersection.
   * @param intersection Intersection information.
   * @param ray Incoming ray.
   * @param lights Lights affecting the intersection.
   * @param scene Scene containing all objects.
   * @return Computed color.
   */
  [[nodiscard]] Core::Color computeReflectedColor(
      const Core::Intersection &intersection, const Core::Ray &ray,
      const std::vector<std::shared_ptr<Core::ILight>> &lights,
      const Core::Scene &scene) const;

  /**
   * @brief Compute the color resulting from refraction at an intersection.
   * @param intersection Intersection information.
   * @param ray Incoming ray.
   * @param lights Lights affecting the intersection.
   * @param scene Scene containing all objects.
   * @return Computed color.
   */
  [[nodiscard]] Core::Color computeRefractedColor(
      const Core::Intersection &intersection, const Core::Ray &ray,
      const std::vector<std::shared_ptr<Core::ILight>> &lights,
      const Core::Scene &scene) const;

  /**
   * @brief Set the reflection coefficient.
   * @param coefficient New ambient coefficient.
   */
  void setReflectionCoefficient(double coefficient) noexcept {
    m_reflectionCoefficient = coefficient;
  }

  /**
   * @brief Set the refraction coefficient.
   * @param coefficient New ambient coefficient.
   */
  void setRefractionCoefficient(double coefficient) noexcept {
    m_refractionCoefficient = coefficient;
  }

  /**
   * @brief Set the refractive index.
   * @param index New refractive index.
   */
  void setRefractiveIndex(double index) noexcept {
    m_refractiveIndex = index;
  }

  Utility::Clamped<double, 0.0, 1.0> m_reflectionCoefficient{0.0};
  Utility::Clamped<double, 0.0, 1.0> m_refractionCoefficient{0.0};
  double m_refractiveIndex{1.0};
};

} // namespace Raytracer::Plugins
