#pragma once
#include "IPlugin.hpp"
#include <map>
#include <string>
#include <vector>

namespace Raytracer::Plugin {

/**
 * @class PluginManager
 * @brief Manages loading and unloading of plugins
 */
class PluginManager {
public:
  /**
   * @brief Get the singleton instance of the PluginManager
   * @return Reference to the PluginManager instance
   */
  static PluginManager &getInstance();

  /**
   * @brief Load a plugin from a shared library
   * @param path Path to the shared library
   * @return True if the plugin was loaded successfully, false otherwise
   */
  [[nodiscard]] bool loadPlugin(const std::string &path);

  /**
   * @brief Unload a plugin by name
   * @param name Name of the plugin to unload
   */
  void unloadPlugin(const std::string &name);

  /**
   * @brief Unload all loaded plugins
   */
  void unloadAllPlugins();

  /**
   * @brief Load all plugins from a specified directory
   * @param directory Directory containing the plugin shared libraries
   */
  void loadPluginsFromDirectory(const std::string &directory);

  /**
   * @brief Get a list of all loaded plugins of a specific type
   * @tparam T Type of the plugin to retrieve
   * @return Vector of pointers to the loaded plugins of the specified type
   */
  template <typename T> std::vector<T *> getPlugins() const {
    std::vector<T *> result;

    for (const auto &[name, handle] : m_plugins) {
      if (T *plugin = dynamic_cast<T *>(handle.plugin)) {
        result.push_back(plugin);
      }
    }

    return result;
  }

  /**
   * @brief Get a specific plugin by name
   * @param name Name of the plugin to retrieve
   * @return Pointer to the plugin if found, nullptr otherwise
   */
  [[nodiscard]] IPlugin *getPlugin(const std::string &name) const;

private:
  /**
   * @brief Private constructor for singleton pattern
   */
  PluginManager() = default;

  ~PluginManager();

  /**
   * @struct PluginHandle
   * @brief Structure to hold the plugin handle and the plugin instance
   */
  struct PluginHandle {
    void *handle;
    IPlugin *plugin;
  };

  std::map<std::string, PluginHandle> m_plugins;
};

} // namespace Raytracer::Plugin
