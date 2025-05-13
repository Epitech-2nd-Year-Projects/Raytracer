#include "PluginManager.hpp"
#include <dlfcn.h>
#include <filesystem>
#include <iostream>

namespace Raytracer::Plugin {

PluginManager &PluginManager::getInstance() {
  static PluginManager instance;
  return instance;
}

PluginManager::~PluginManager() { unloadAllPlugins(); }

bool PluginManager::loadPlugin(const std::string &path) {
  void *handle = dlopen(path.c_str(), RTLD_LAZY);

  if (!handle) {
    std::cerr << "Failed to load plugin " << path << ": " << dlerror()
              << std::endl;
    return false;
  }

  dlerror();

  using CreatePluginFunc = IPlugin *(*)();
  CreatePluginFunc createPlugin =
      reinterpret_cast<CreatePluginFunc>(dlsym(handle, "createPlugin"));

  const char *dlsym_error = dlerror();
  if (dlsym_error) {
    std::cerr << "Failed to load symbol from " << path << ": " << dlsym_error
              << std::endl;
    dlclose(handle);
    return false;
  }

  IPlugin *plugin = createPlugin();
  if (!plugin) {
    std::cerr << "Failed to create plugin instance from " << path << std::endl;
    dlclose(handle);
    return false;
  }

  PluginHandle pluginHandle{handle, plugin};
  m_plugins[plugin->getName()] = pluginHandle;

  std::cout << "Loaded plugin: " << plugin->getName() << std::endl;
  return true;
}

void PluginManager::unloadPlugin(const std::string &name) {
  auto it = m_plugins.find(name);
  if (it == m_plugins.end()) {
    return;
  }

  using DestroyPluginFunc = void (*)(IPlugin *);
  DestroyPluginFunc destroyPlugin = reinterpret_cast<DestroyPluginFunc>(
      dlsym(it->second.handle, "destroyPlugin"));

  if (destroyPlugin) {
    destroyPlugin(it->second.plugin);
  } else {
    delete it->second.plugin;
  }

  dlclose(it->second.handle);
  m_plugins.erase(it);
}

void PluginManager::unloadAllPlugins() {
  for (auto it = m_plugins.begin(); it != m_plugins.end();) {
    std::string name = it->first;
    ++it;
    unloadPlugin(name);
  }
}

void PluginManager::loadPluginsFromDirectory(const std::string &directory) {
  if (!std::filesystem::exists(directory)) {
    std::cerr << "Plugins directory not found: " << directory << std::endl;
    return;
  }

  for (const auto &entry : std::filesystem::directory_iterator(directory)) {
    if (entry.path().extension() == ".so") {
      if (!loadPlugin(entry.path().string())) {
        std::cerr << "Failed to load plugin: " << entry.path() << std::endl;
      }
    }
  }
}

} // namespace Raytracer::Plugin
