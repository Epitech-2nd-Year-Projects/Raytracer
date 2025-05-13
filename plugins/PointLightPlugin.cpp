#include "PointLightPlugin.hpp"
#include "Parser/SceneParser.hpp"
#include "Plugin/LightPlugin.hpp"

namespace Raytracer::Plugins {

std::unique_ptr<Plugin::LightPlugin> PointLightPlugin::create() {
  return std::make_unique<PointLightPlugin>();
}

bool PointLightPlugin::configure(const libconfig::Setting &config) {
  try {
    auto position = Parser::SceneParser::parsePoint3(config.lookup("position"));

    if (position) {
      this->setPosition(*position);
    }
  } catch (const libconfig::SettingNotFoundException &) {
  }
  return true;
}

double
PointLightPlugin::computeIllumination(const Math::Point<3> &intersectionPoint,
                                      const Math::Vector<3> &normal,
                                      const Core::Scene &scene) const noexcept {
  Math::Vector<3> lightDirection = getDirectionFrom(intersectionPoint);

  const double epsilon = 1e-4;
  Math::Point<3> shadowRayOrigin = intersectionPoint + normal * epsilon;
  double distanceToLight = getDistance(intersectionPoint);

  Core::Ray shadowRay(shadowRayOrigin, lightDirection, epsilon,
                      distanceToLight);

  if (scene.hasIntersection(shadowRay)) {
    return 0.0;
  }

  return APositionalLight::computeIllumination(intersectionPoint, normal,
                                               scene);
}

} // namespace Raytracer::Plugins

extern "C" {
Raytracer::Plugin::IPlugin *createPlugin() {
  return new Raytracer::Plugins::PointLightPlugin();
}

void destroyPlugin(Raytracer::Plugin::IPlugin *plugin) { delete plugin; }
}
