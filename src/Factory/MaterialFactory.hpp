#pragma once

#include "Plugin/MaterialPlugin.hpp"
#include "Plugin/PluginManager.hpp"
namespace Raytracer::Factory {

/**
 * @class MaterialFactory
 * @brief Factory for creating different types of materials
 */
class MaterialFactory {
public:
  /**
   * @brief Create a material based on the type
   * @param type Type of the material to create
   * @return A unique pointer to the created material
   */
  [[nodiscard]] static std::shared_ptr<Plugin::MaterialPlugin>
  createMaterial(const std::string &type) {
    auto &manager = Plugin::PluginManager::getInstance();
    auto plugins = manager.getPlugins<Plugin::MaterialPlugin>();

    for (auto *plugin : plugins) {
      if (plugin->getName() == type) {
        return plugin->create();
      }
    }
    return nullptr;
  }
};
} // namespace Raytracer::Factory
