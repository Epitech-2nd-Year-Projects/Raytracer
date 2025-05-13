#pragma once
#include "Core/ALight.hpp"
#include "Plugin/LightPlugin.hpp"

namespace Raytracer::Plugins {

/**
 * @class PointLightPlugin
 * @brief Plugin for creating a light plugin
 */
class PointLightPlugin : public Plugin::LightPlugin,
                         public Core::APositionalLight {
public:
  /**
   * @brief Default constructor
   */
  PointLightPlugin() noexcept = default;

  /**
   * @brief Virtual destructor
   */
  ~PointLightPlugin() noexcept override = default;

  /**
   * @brief Get the name of this plugin
   * @return The name of the plugin
   */
  [[nodiscard]]
  std::string getName() const override {
    return "PointLight";
  }

  /**
   * @brief Create a new instance of the sphere primitive plugin
   * @return A unique pointer to the new sphere primitive
   */
  std::unique_ptr<LightPlugin> create() override;

  /**
   * @brief Configure the sphere plugin with a libconfig setting
   * @param config The libconfig setting to configure the sphere
   * @return True if the configuration was successful, false otherwise
   */
  bool configure(const libconfig::Setting &config) override;

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
                      const Core::Scene &scene) const noexcept override;

  /**
   * @brief Determines whether this light casts shadows
   * @return Always returns true for point lights
   */
  [[nodiscard]] bool castsShadow() const noexcept override { return true; }
};

} // namespace Raytracer::Plugins
