#pragma once

#include "Core/AMaterial.hpp"

namespace Raytracer::Materials {

class FlatMaterial final : public Core::AMaterial {
public:
  FlatMaterial() noexcept = default;

  FlatMaterial(const Core::Color &diffuseColor, const Core::Color &ambientColor,
               double ambientCoef, double diffuseCoef) noexcept;

  [[nodiscard]] Core::Color computeColor(
      const Core::Intersection &intersection, const Core::Ray &ray,
      const std::vector<std::shared_ptr<Core::ILight>> &lights) const override;
};
} // namespace Raytracer::Materials