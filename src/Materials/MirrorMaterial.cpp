/**
 * @file MirrorMaterial.cpp
 * @brief Implementation of the MirrorMaterial class, providing diffuse and
 * ambient lighting calculations.
 */

#include "Materials/MirrorMaterial.hpp"

namespace Raytracer::Materials {

MirrorMaterial::MirrorMaterial(const Core::Color &diffuseColor,
                               const Core::Color &ambientColor,
                               double ambientCoef, double diffuseCoef) noexcept
    : AMaterial(diffuseColor, ambientColor, ambientCoef, diffuseCoef) {}

Core::Color MirrorMaterial::computeColor(
    const Core::Intersection &intersection,
    const Core::Ray &ray,
    const std::vector<std::shared_ptr<Core::ILight>> &lights,
    const Core::Scene &scene) const {

  if (ray.getDepth() > 5) {
    return getAmbientColor() * getAmbientCoefficient();
  }

  Math::Vector<3> normal = intersection.getNormal();
  Math::Vector<3> reflectDir = ray.getDirection() - normal * 2.0 * normal.dot(ray.getDirection());

  double epsilon = 0.001;
  Math::Point<3> offsetPoint = intersection.getPoint() + normal * epsilon;

  Core::Ray reflectRay(offsetPoint, reflectDir, epsilon, ray.getMaxDistance());
  reflectRay.setDepth(ray.getDepth() + 1);

  std::optional<Core::Intersection> newIntersection = scene.findNearestIntersection(reflectRay);

  if (!newIntersection) {
    return getAmbientColor() * getAmbientCoefficient();
  }

  Core::Color reflectedColor = newIntersection->getMaterial()->computeColor(
      newIntersection.value(), reflectRay, lights, scene);

  return (reflectedColor * getDiffuseCoefficient()).add(getAmbientColor() * getAmbientCoefficient());
}

} // namespace Raytracer::Materials