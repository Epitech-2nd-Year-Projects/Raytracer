#pragma once
#include "Core/AMaterial.hpp"
#include "IPlugin.hpp"
#include <memory>

namespace Raytracer::Plugin {

/**
 * @class MaterialPlugin
 * @brief Base class for all material plugins
 */
class MaterialPlugin : public IPlugin, public Core::AMaterial {
public:
  /**
   * @brief Default destructor
   */
  virtual ~MaterialPlugin() = default;

  /**
   * @brief Create a new instance of the material plugin
   * @return A shared pointer to the new material plugin
   */
  virtual std::shared_ptr<MaterialPlugin> create() = 0;

  /**
   * @brief Get the type of this plugin
   * @return The type of the plugin
   */
  PluginType getType() const override { return PluginType::Material; }

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
