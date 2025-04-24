#pragma once

#include "Core/Color.hpp"
#include "Core/ILight.hpp"
#include "Core/Intersection.hpp"
#include "Core/Ray.hpp"

#include <vector>

namespace Raytracer::Core {
class IMaterial {
public:
  virtual ~IMaterial() = default;

  [[nodiscard]] virtual Color computeColor(
      const Intersection &intersection, const Ray &ray,
      const std::vector<std::shared_ptr<ILightBase>> &lights) const = 0;

  [[nodiscard]] virtual const Color &getDiffuseColor() const noexcept = 0;
  [[nodiscard]] virtual const Color &getAmbientColor() const noexcept = 0;
  [[nodiscard]] virtual double getAmbientCoefficient() const noexcept = 0;
  [[nodiscard]] virtual double getDiffuseCoefficient() const noexcept = 0;

  virtual void setDiffuseColor(const Color &color) noexcept = 0;
  virtual void setAmbientColor(const Color &color) noexcept = 0;
  virtual void setAmbientCoefficient(double coefficient) noexcept = 0;
  virtual void setDiffuseCoefficient(double coefficient) noexcept = 0;
};
} // namespace Raytracer::Core