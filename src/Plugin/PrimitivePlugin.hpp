#pragma once
#include "Core/APrimitive.hpp"
#include "IPlugin.hpp"
#include <memory>

namespace Raytracer::Plugin {

/**
 * @class PrimitivePlugin
 * @brief Base class for all primitive plugins
 */
class PrimitivePlugin : public IPlugin, public Core::APrimitive {
public:
  /**
   * @brief Default destructor
   */
  virtual ~PrimitivePlugin() = default;

  /**
   * @brief Create a new instance of the primitive plugin
   * @return A unique pointer to the new primitive plugin
   */
  virtual std::unique_ptr<PrimitivePlugin> create() = 0;

  /**
   * @brief Get the type of this plugin
   * @return The type of the plugin
   */
  PluginType getType() const override { return PluginType::Primitive; }

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
