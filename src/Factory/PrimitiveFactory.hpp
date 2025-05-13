#pragma once

#include "Plugin/PluginManager.hpp"
#include "Plugin/PrimitivePlugin.hpp"
namespace Raytracer::Factory {

/**
 * @class PrimitiveFactory
 * @brief Factory for creating different types of primitives
 */
class PrimitiveFactory {
public:
  /**
   * @brief Create a primitive based on the type
   * @param type Type of the primitive to create
   * @return A unique pointer to the created primitive
   */
  [[nodiscard]] static std::unique_ptr<Plugin::PrimitivePlugin>
  createPrimitive(const std::string &type) {
    auto &manager = Plugin::PluginManager::getInstance();
    auto plugins = manager.getPlugins<Plugin::PrimitivePlugin>();

    for (auto *plugin : plugins) {
      if (plugin->getName() == type) {
        return plugin->create();
      }
    }
    return nullptr;
  }
};

} // namespace Raytracer::Factory
