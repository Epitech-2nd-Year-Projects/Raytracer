#pragma once
#include "Core/ALight.hpp"
#include "Core/ILight.hpp"
#include "Plugin/LightPlugin.hpp"

namespace Raytracer::Plugins {

/**
 * @class AmbientLightPlugin
 * @brief Plugin for creating a light plugin
 */
class AmbientLightPlugin : public Plugin::LightPlugin,
                           public Core::IAmbientLight {
public:
  /**
   * @brief Default constructor
   */
  AmbientLightPlugin() noexcept = default;

  /**
   * @brief Virtual destructor
   */
  ~AmbientLightPlugin() noexcept override = default;

  /**
   * @brief Get the name of this plugin
   * @return The name of the plugin
   */
  [[nodiscard]]
  std::string getName() const override {
    return "AmbientLight";
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
   * @brief Computes the illumination at the specified intersection point
   * @param intersectionPoint The point of intersection (unused for ambient
   * light)
   * @param normal The surface normal at the intersection point (unused for
   * ambient light)
   * @return The illumination value (equal to the light's intensity)
   */
  [[nodiscard]] double computeIllumination(
      [[maybe_unused]] const Math::Point<3> &intersectionPoint,
      [[maybe_unused]] const Math::Vector<3> &normal,
      [[maybe_unused]] const Core::Scene &scene) const noexcept override;

  /**
   * @brief Gets the direction from a point to the light source
   * @param point The reference point (unused for ambient light)
   * @return A fixed directional vector (0, 0, 1)
   */
  [[nodiscard]] Math::Vector<3> getDirectionFrom(
      [[maybe_unused]] const Math::Point<3> &point) const noexcept {
    return Math::Vector<3>(0.0, 0.0, 1.0);
  }

  /**
   * @brief Determines whether this light casts shadows
   * @return Always returns false
   */
  [[nodiscard]] bool castsShadow() const noexcept override { return false; }
};

} // namespace Raytracer::Plugins
