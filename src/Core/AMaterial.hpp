#pragma once

#include "Core/Color.hpp"
#include "Core/ILight.hpp"
#include "Core/IMaterial.hpp"
#include "Core/Intersection.hpp"
#include "Core/Ray.hpp"

#include <vector>

namespace Raytracer::Core {

class AMaterial : public IMaterial {
public:
  AMaterial() noexcept = default;
  AMaterial(const Color &diffuseColor, const Color &ambientColor,
            double ambientCoef, double diffuseCoef)
      : m_diffuseColor(diffuseColor), m_ambientColor(ambientColor),
        m_ambientCoefficient(ambientCoef), m_diffuseCoefficient(diffuseCoef) {}
  ~AMaterial() noexcept override = default;

  AMaterial(const AMaterial &) = delete;
  AMaterial(AMaterial &&) = delete;
  AMaterial &operator=(const AMaterial &) = delete;
  AMaterial &operator=(AMaterial &&) = delete;

  [[nodiscard]] Color
  computeColor(const Intersection &intersection, const Ray &ray,
               const std::vector<std::shared_ptr<ILight>> &lights)
      const override = 0;

  [[nodiscard]] const Color &getDiffuseColor() const noexcept override {
    return m_diffuseColor;
  }
  [[nodiscard]] const Color &getAmbientColor() const noexcept override {
    return m_ambientColor;
  }
  [[nodiscard]] double getAmbientCoefficient() const noexcept override {
    return m_ambientCoefficient;
  }
  [[nodiscard]] double getDiffuseCoefficient() const noexcept override {
    return m_diffuseCoefficient;
  }

  void setDiffuseColor(const Color &color) noexcept override {
    m_diffuseColor = color;
  }
  void setAmbientColor(const Color &color) noexcept override {
    m_ambientColor = color;
  }
  void setAmbientCoefficient(double coefficient) noexcept override {
    m_ambientCoefficient = coefficient;
  }
  void setDiffuseCoefficient(double coefficient) noexcept override {
    m_diffuseCoefficient = coefficient;
  }

private:
  Color m_diffuseColor{};
  Color m_ambientColor{};
  double m_ambientCoefficient{0.0};
  double m_diffuseCoefficient{0.0};
};
} // namespace Raytracer::Core