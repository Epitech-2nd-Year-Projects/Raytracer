#include "MirrorMaterialPlugin.hpp"
#include "Parser/SceneParser.hpp"
#include "Plugin/MaterialPlugin.hpp"

namespace Raytracer::Plugins {

std::shared_ptr<Plugin::MaterialPlugin> MirrorMaterialPlugin::create() {
  return std::make_shared<MirrorMaterialPlugin>();
}

bool MirrorMaterialPlugin::configure(const libconfig::Setting &config) {
  try {
    const libconfig::Setting &color = config.lookup("color");
    auto r = Parser::SceneParser::getSetting<int>(color, "r");
    auto g = Parser::SceneParser::getSetting<int>(color, "g");
    auto b = Parser::SceneParser::getSetting<int>(color, "b");

    if (!r || !g || !b) {
      return false;
    }

    const double ambiantCoefficient = config.lookup("ambientCoefficient");
    const double diffuseCoefficient = config.lookup("diffuseCoefficient");

    this->setAmbientCoefficient(ambiantCoefficient);
    this->setDiffuseCoefficient(diffuseCoefficient);
    this->setAmbientColor(Core::Color(*r, *g, *b));
    this->setDiffuseColor(Core::Color(*r, *g, *b));
  } catch (const libconfig::SettingNotFoundException &) {
  }
  return true;
}

Core::Color MirrorMaterialPlugin::computeColor(
    const Core::Intersection &intersection, const Core::Ray &ray,
    const std::vector<std::shared_ptr<Core::ILight>> &lights,
    const Core::Scene &scene) const {

  if (ray.getDepth() > 5) {
    return getAmbientColor() * getAmbientCoefficient();
  }

  Math::Vector<3> normal = intersection.getNormal();
  Math::Vector<3> reflectDir =
      ray.getDirection() - normal * 2.0 * normal.dot(ray.getDirection());

  double epsilon = 0.001;
  Math::Point<3> offsetPoint = intersection.getPoint() + normal * epsilon;

  Core::Ray reflectRay(offsetPoint, reflectDir, epsilon, ray.getMaxDistance());
  reflectRay.setDepth(ray.getDepth() + 1);

  std::optional<Core::Intersection> newIntersection =
      scene.findNearestIntersection(reflectRay);

  if (!newIntersection) {
    return getAmbientColor() * getAmbientCoefficient();
  }

  Core::Color reflectedColor = newIntersection->getMaterial()->computeColor(
      newIntersection.value(), reflectRay, lights, scene);

  return (reflectedColor * getDiffuseCoefficient())
      .add(getAmbientColor() * getAmbientCoefficient());
}

} // namespace Raytracer::Plugins

extern "C" {
Raytracer::Plugin::IPlugin *createPlugin() {
  return new Raytracer::Plugins::MirrorMaterialPlugin();
}

void destroyPlugin(Raytracer::Plugin::IPlugin *plugin) { delete plugin; }
}
