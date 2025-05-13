#pragma once
#include "Core/ALight.hpp"
#include "IPlugin.hpp"
#include <memory>

namespace Raytracer::Plugin {

/**
 * @class LightPlugin
 * @brief Base class for all light plugins
 */
class LightPlugin : public IPlugin, public virtual Core::ALight {
public:
  /**
   * @brief Default destructor
   */
  virtual ~LightPlugin() = default;

  /**
   * @brief Create a new instance of the material plugin
   * @return A shared pointer to the new material plugin
   */
  virtual std::unique_ptr<LightPlugin> create() = 0;

  /**
   * @brief Get the type of this plugin
   * @return The type of the plugin
   */
  PluginType getType() const override { return PluginType::Light; }

  /**
   * @brief Configure the plugin with a libconfig setting
   * @param config The libconfig setting to configure the plugin
   * @return True if the configuration was successful, false otherwise
   */
  virtual bool
  configure([[maybe_unused]] const libconfig::Setting &config) override {
    return true;
  }
};
} // namespace Raytracer::Plugin
