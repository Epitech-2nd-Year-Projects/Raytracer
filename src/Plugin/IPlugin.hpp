#pragma once
#include <libconfig.h++>
#include <string>

namespace Raytracer::Plugin {

/**
 * @enum PluginType
 * @brief Enum for different types of plugins
 */
enum class PluginType { Primitive, Light, Material, Renderer };

/**
 * @class IPlugin
 * @brief Interface for all plugins
 */
class IPlugin {
public:
  /**
   * @brief Default destructor
   */
  virtual ~IPlugin() = default;

  /**
   * @brief Get the name of this plugin
   * @return The name of the plugin
   */
  [[nodiscard]] virtual std::string getName() const = 0;

  /**
   * @brief Get the type of this plugin
   * @return The type of the plugin
   */
  [[nodiscard]] virtual PluginType getType() const = 0;

  /**
   * @brief Configure the plugin with a libconfig setting
   * @param config The libconfig setting to configure the plugin
   * @return True if the configuration was successful, false otherwise
   */
  virtual bool configure(const libconfig::Setting &config) = 0;
};
} // namespace Raytracer::Plugin
