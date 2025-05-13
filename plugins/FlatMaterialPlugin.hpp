#pragma once
#include "Plugin/MaterialPlugin.hpp"

namespace Raytracer::Plugins {

/**
 * @class FlatMaterialPlugin
 * @brief Plugin for creating a flat material
 */
class FlatMaterialPlugin : public Plugin::MaterialPlugin {
public:
  /**
   * @brief Default constructor
   */
  FlatMaterialPlugin() noexcept = default;

  /**
   * @brief Virtual destructor
   */
  ~FlatMaterialPlugin() noexcept override = default;

  /**
   * @brief Get the name of this plugin
   * @return The name of the plugin
   */
  [[nodiscard]]
  std::string getName() const override {
    return "FlatMaterial";
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
};

} // namespace Raytracer::Plugins
