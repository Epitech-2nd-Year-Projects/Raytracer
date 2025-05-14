#include "MirrorMaterialPlugin.hpp"
#include "Parser/SceneParser.hpp"
#include "Plugin/MaterialPlugin.hpp"

namespace Raytracer::Plugins {

std::shared_ptr<Plugin::MaterialPlugin> MirrorMaterialPlugin::create() {
  return std::make_shared<MirrorMaterialPlugin>();
}

bool MirrorMaterialPlugin::configure(const libconfig::Setting &config) {
  try {
    std::optional<Core::Color> color = Parser::SceneParser::parseColor(config);

    if (!color) {
      return false;
    }

    const double ambiantCoefficient = config.lookup("ambientCoefficient");
    const double diffuseCoefficient = config.lookup("diffuseCoefficient");
    const double reflCoef = config.lookup("reflectionCoefficient");
    const double refrCoef = config.lookup("refractionCoefficient");
    const double refrIndex = config.lookup("refractionIndex");

    this->setAmbientCoefficient(ambiantCoefficient);
    this->setDiffuseCoefficient(diffuseCoefficient);
    this->setAmbientColor(*color);
    this->setDiffuseColor(*color);
    this->setReflectionCoefficient(reflCoef);
    this->setRefractionCoefficient(refrCoef);
    this->setRefractiveIndex(refrIndex);
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
  return (computeReflectedColor(intersection, ray, lights, scene) * m_reflectionCoefficient.get())
      .add(computeRefractedColor(intersection, ray, lights, scene) * m_refractionCoefficient.get());
}

Core::Color MirrorMaterialPlugin::computeReflectedColor(
    const Core::Intersection &intersection, const Core::Ray &ray,
    const std::vector<std::shared_ptr<Core::ILight>> &lights,
    const Core::Scene &scene) const {

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

Core::Color MirrorMaterialPlugin::computeRefractedColor(
    const Core::Intersection &intersection, const Core::Ray &ray,
    const std::vector<std::shared_ptr<Core::ILight>> &lights,
    const Core::Scene &scene) const {
  double eta = 1.0 / m_refractiveIndex;

  Math::Vector<3> normal = intersection.getNormal();

  double cosi = -normal.dot(ray.getDirection());

  if (cosi < 0) {
    normal = normal * -1;
    cosi = -normal.dot(ray.getDirection());
    eta = 1 / eta;
  }

  double k = 1 - eta * eta * (1 - cosi * cosi);

  if (k < 0) {
    return {0, 0, 0};
  }

  Math::Vector<3> refractDir =
      ray.getDirection() * eta + normal * (eta * cosi - std::sqrt(k));
  refractDir = refractDir.normalize();

  double epsilon = 0.001;
  Math::Point<3> offsetPoint = intersection.getPoint() - normal * epsilon;

  Core::Ray refractRay(offsetPoint, refractDir, epsilon, ray.getMaxDistance());
  refractRay.setDepth(ray.getDepth() + 1);

  std::optional<Core::Intersection> newIntersection =
      scene.findNearestIntersection(refractRay);

  if (!newIntersection) {
    return getAmbientColor() * getAmbientCoefficient();
  }

  Core::Color refractedColor = newIntersection->getMaterial()->computeColor(
      newIntersection.value(), refractRay, lights, scene);

  double transmissionCoef = 0.8;
  return refractedColor * transmissionCoef;
}

} // namespace Raytracer::Plugins

extern "C" {
Raytracer::Plugin::IPlugin *createPlugin() {
  return new Raytracer::Plugins::MirrorMaterialPlugin();
}

void destroyPlugin(Raytracer::Plugin::IPlugin *plugin) { delete plugin; }
}
