/**
 * @file FlatMaterial.cpp
 * @brief Implementation of the FlatMaterial class, providing diffuse and
 * ambient lighting calculations.
 */

#include "Materials/FlatMaterial.hpp"

namespace Raytracer::Materials {

FlatMaterial::FlatMaterial(const Core::Color &diffuseColor,
                           const Core::Color &ambientColor, double ambientCoef,
                           double diffuseCoef) noexcept
    : AMaterial(diffuseColor, ambientColor, ambientCoef, diffuseCoef) {}

Core::Color FlatMaterial::computeColor(
    const Core::Intersection &intersection,
    [[maybe_unused]] const Core::Ray &ray,
    const std::vector<std::shared_ptr<Core::ILight>> &lights) const {
  Core::Color finalColor = Core::Black;

  for (const auto &light : lights) {
    const Math::Vector<3> &normal = intersection.getNormal();

    if (auto ambientLight = std::dynamic_pointer_cast<Core::IAmbientLight>(light)) {
      double ambientIntensity = ambientLight->getIntensity();
      const Core::Color &lightColor = ambientLight->getColor();
      Core::Color ambientColor = getAmbientColor();
      Core::Color ambComp{
        ambientColor.getR() * getAmbientCoefficient() * ambientIntensity * lightColor.getR() / 255.0,
        ambientColor.getG() * getAmbientCoefficient() * ambientIntensity * lightColor.getG() / 255.0,
        ambientColor.getB() * getAmbientCoefficient() * ambientIntensity * lightColor.getB() / 255.0};
      finalColor = finalColor.add(ambComp);
    } else if (auto directionalLight =
            std::dynamic_pointer_cast<Core::IDirectionalLight>(light)) {
      Math::Vector<3> lightDir = directionalLight->getDirection() * -1.0;
      double dotResult = normal.dot(lightDir);

      if (dotResult > 0.0) {
        double lightIntensity = directionalLight->getIntensity();
        const Core::Color &lightColor = directionalLight->getColor();
        double diffuseFactor =
            getDiffuseCoefficient() * dotResult * lightIntensity;

        Core::Color diffuseComponent = getDiffuseColor();
        diffuseComponent = Core::Color(
            diffuseComponent.getR() * diffuseFactor * lightColor.getR() / 255.0,
            diffuseComponent.getG() * diffuseFactor * lightColor.getG() / 255.0,
            diffuseComponent.getB() * diffuseFactor * lightColor.getB() /
                255.0);

        finalColor = finalColor.add(diffuseComponent);
      }
    } else if (auto positionalLight =
                   std::dynamic_pointer_cast<Core::IPositionalLight>(light)) {
      Math::Vector<3> lightDir =
          positionalLight->getDirectionFrom(intersection.getPoint());
      double dotResult = normal.dot(lightDir);

      if (dotResult > 0.0) {
        double lightIntensity = positionalLight->getIntensity();
        const Core::Color &lightColor = positionalLight->getColor();
        double diffuseFactor =
            getDiffuseCoefficient() * dotResult * lightIntensity;

        Core::Color diffuseComponent = getDiffuseColor();
        diffuseComponent = Core::Color(
            diffuseComponent.getR() * diffuseFactor * lightColor.getR() / 255.0,
            diffuseComponent.getG() * diffuseFactor * lightColor.getG() / 255.0,
            diffuseComponent.getB() * diffuseFactor * lightColor.getB() /
                255.0);
        finalColor = finalColor.add(diffuseComponent);
      }
    }
  }
  return finalColor;
}

} // namespace Raytracer::Materials