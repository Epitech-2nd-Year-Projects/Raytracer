#include "DiffuseLightPlugin.hpp"
#include "Parser/SceneParser.hpp"
#include "Plugin/LightPlugin.hpp"

namespace Raytracer::Plugins {

std::unique_ptr<Plugin::LightPlugin> DiffuseLightPlugin::create() {
  return std::make_unique<DiffuseLightPlugin>();
}

bool DiffuseLightPlugin::configure(const libconfig::Setting &config) {
  try {
    double intensity = config.lookup("intensity");

    if (intensity) {
      this->setIntensity(intensity);
    }
  } catch (const libconfig::SettingNotFoundException &) {
  }
  return true;
}

double DiffuseLightPlugin::computeIllumination(
    [[maybe_unused]] const Math::Point<3> &intersectionPoint,
    [[maybe_unused]] const Math::Vector<3> &normal,
    [[maybe_unused]] const Core::Scene &scene) const noexcept {
  return getIntensity();
}

} // namespace Raytracer::Plugins

extern "C" {
Raytracer::Plugin::IPlugin *createPlugin() {
  return new Raytracer::Plugins::DiffuseLightPlugin();
}

void destroyPlugin(Raytracer::Plugin::IPlugin *plugin) { delete plugin; }
}
