#include "SteelMaterialPlugin.hpp"
#include "Parser/SceneParser.hpp"
#include "Plugin/MaterialPlugin.hpp"
#include "SteelMaterialPlugin.hpp"

namespace Raytracer::Plugins {

std::shared_ptr<Plugin::MaterialPlugin> SteelMaterialPlugin::create() {
  return std::make_shared<SteelMaterialPlugin>();
}

bool SteelMaterialPlugin::configure(const libconfig::Setting &config) {
  try {
    std::optional<Core::Color> color = Parser::SceneParser::parseColor(config, "color");

    if (!color) {
      return false;
    }

    const double ambiantCoefficient = config.lookup("ambientCoefficient");
    const double diffuseCoefficient = config.lookup("diffuseCoefficient");
    const double fuzz = 0.3;

    this->setAmbientCoefficient(ambiantCoefficient);
    this->setDiffuseCoefficient(diffuseCoefficient);
    this->setAmbientColor(*color);
    this->setDiffuseColor(*color);
    m_fuzz = fuzz;
  } catch (const libconfig::SettingNotFoundException &) {
  }
  return true;
}

Math::Vector<3> SteelMaterialPlugin::randomInUnitSphere() const {
  Math::Vector<3> brushDirection;
  brushDirection.m_components[0] = 0.8;
  brushDirection.m_components[1] = 0.1 * m_distribution(m_generator);
  brushDirection.m_components[2] = 0.1 * m_distribution(m_generator);
  double len = brushDirection.length();
  if (len > 0.0) {
    for (int i = 0; i < 3; i++) {
      brushDirection.m_components[i] /= len;
    }
  }
  Math::Vector<3> randomNoise;
  randomNoise = Math::Vector<3>(1.0, 1.0, 1.0);
  while (randomNoise.squaredNorm() >= 1.0) {
    randomNoise = Math::Vector<3>(m_distribution(m_generator) * 0.2,
                                  m_distribution(m_generator) * 0.8,
                                  m_distribution(m_generator) * 0.8);
  }

  return brushDirection + randomNoise * 0.3;
}

Core::Color SteelMaterialPlugin::computeColor(
    const Core::Intersection &intersection, const Core::Ray &ray,
    const std::vector<std::shared_ptr<Core::ILight>> &lights,
    const Core::Scene &scene) const {

  if (ray.getDepth() > 5)
    return getAmbientColor() * getAmbientCoefficient();

  Math::Vector<3> normal = intersection.getNormal();
  Math::Vector<3> reflectDir =
      ray.getDirection() - normal * 2.0 * normal.dot(ray.getDirection());

  reflectDir = reflectDir + randomInUnitSphere() * m_fuzz;
  reflectDir = reflectDir.normalize();

  double epsilon = 0.001;
  Math::Point<3> offsetPoint = intersection.getPoint() + normal * epsilon;

  Core::Ray reflectRay(offsetPoint, reflectDir, epsilon, ray.getMaxDistance());
  reflectRay.setDepth(ray.getDepth() + 1);

  std::optional<Core::Intersection> newIntersection =
      scene.findNearestIntersection(reflectRay);

  Core::Color aluminumBaseColor = getDiffuseColor() * 0.2;

  double specularIntensity =
      std::max(0.0, std::pow(-ray.getDirection().dot(reflectDir), 10.0)) * 0.4;
  Core::Color specularColor = Core::Color(255, 255, 255) * specularIntensity;

  if (!newIntersection)
    return aluminumBaseColor.add(specularColor)
        .add(getAmbientColor() * getAmbientCoefficient());

  Core::Color reflectedColor = newIntersection->getMaterial()->computeColor(
      newIntersection.value(), reflectRay, lights, scene);

  reflectedColor = reflectedColor * 0.7;

  return (reflectedColor * getDiffuseCoefficient())
      .add(specularColor)
      .add(aluminumBaseColor)
      .add(getAmbientColor() * getAmbientCoefficient());
}

} // namespace Raytracer::Plugins

extern "C" {
Raytracer::Plugin::IPlugin *createPlugin() {
  return new Raytracer::Plugins::SteelMaterialPlugin();
}

void destroyPlugin(Raytracer::Plugin::IPlugin *plugin) { delete plugin; }
}
