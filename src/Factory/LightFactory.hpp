#pragma once

#include "Plugin/LightPlugin.hpp"
#include "Plugin/PluginManager.hpp"
namespace Raytracer::Factory {

/**
 * @class LightFactory
 * @brief Factory for creating different types of lights
 */
class LightFactory {
public:
  /**
   * @brief Create a primitive based on the type
   * @param type Type of the primitive to create
   * @return A unique pointer to the created primitive
   */
  [[nodiscard]] static std::unique_ptr<Plugin::LightPlugin>
  createLight(const std::string &type) {
    auto &manager = Plugin::PluginManager::getInstance();
    auto plugins = manager.getPlugins<Plugin::LightPlugin>();

    for (auto *plugin : plugins) {
      if (plugin->getName() == type) {
        return plugin->create();
      }
    }
    return nullptr;
  }
};
;
} // namespace Raytracer::Factory
