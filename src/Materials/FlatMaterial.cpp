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
  Core::Color finalColor = getAmbientColor();
  finalColor = Core::Color(finalColor.getR() * getAmbientCoefficient(),
                           finalColor.getG() * getAmbientCoefficient(),
                           finalColor.getB() * getAmbientCoefficient());

  for (const auto &light : lights) {
    const Math::Vector<3> &normal = intersection.getNormal();

    if (auto directionalLight =
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