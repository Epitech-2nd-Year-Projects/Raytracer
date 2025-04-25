#pragma once

#include "Core/Intersection.hpp"
#include "Core/Ray.hpp"
#include "Core/APrimitive.hpp"
#include "Core/BoundingBox.hpp"

namespace Raytracer::Primitives {

class Sphere : public Core::APrimitive {
public:
  Sphere() noexcept = default;
  Sphere(const Math::Point<3>& center, double radius) noexcept;
  ~Sphere() noexcept override = default;

  void setRadius(double r) noexcept;
  void setCenter(const Math::Point<3> &center) noexcept;

  [[nodiscard]] double getRadius() const noexcept;
  [[nodiscard]] const Math::Point<3> &getCenter() const noexcept;

  [[nodiscard]] std::optional<Core::Intersection> intersect(const Core::Ray &ray) const noexcept override;
  [[nodiscard]] Core::BoundingBox getBoundingBox() const noexcept override;

private:
  double m_radius{1.0};
  Math::Point<3> m_center{};
};
}