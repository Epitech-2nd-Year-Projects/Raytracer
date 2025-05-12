/**
 * @file SteelMaterial.cpp
 * @brief Implementation of the SteelMaterial class, providing brushed aluminum
 * effect.
 */

#include "Materials/SteelMaterial.hpp"
#include <random>

namespace Raytracer::Materials {

SteelMaterial::SteelMaterial(const Core::Color &diffuseColor,
                             const Core::Color &ambientColor,
                             double ambientCoef, double diffuseCoef,
                             double fuzz) noexcept
    : AMaterial(diffuseColor, ambientColor, ambientCoef, diffuseCoef),
      m_fuzz(fuzz > 1.0 ? 1.0 : fuzz), m_generator(std::random_device()()),
      m_distribution(-1.0, 1.0) {}

Math::Vector<3> SteelMaterial::randomInUnitSphere() const {
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

Core::Color SteelMaterial::computeColor(
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

} // namespace Raytracer::Materials