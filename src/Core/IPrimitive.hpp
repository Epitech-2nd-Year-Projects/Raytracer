#pragma once

#include <memory>
#include <optional>

#include "Core/BoundingBox.hpp"
#include "Core/IMaterial.hpp"
#include "Core/Intersection.hpp"
#include "Math/Point.hpp"
#include "Math/Vector.hpp"
#include "Core/Ray.hpp"

namespace Raytracer::Core {

class IPrimitive {

public:
  virtual ~IPrimitive() = default;

  [[nodiscard]] virtual std::optional<Intersection>
  intersect(const Ray &ray) const noexcept = 0;

  [[nodiscard]] virtual BoundingBox getBoundingBox() const noexcept = 0;

  virtual void transform(const Math::Point<3> &position,
                         const Math::Vector<3> &rotation,
                         const Math::Vector<3> &scale) noexcept = 0;

  virtual void setPosition(const Math::Point<3> &position) noexcept = 0;
  virtual void setRotation(const Math::Vector<3> &rotation) noexcept = 0;
  virtual void setScale(const Math::Vector<3> &scale) noexcept = 0;
  virtual void setMaterial(std::unique_ptr<IMaterial> material) noexcept = 0;

  [[nodiscard]] virtual const Math::Point<3> &
  getPosition() const noexcept = 0;
  [[nodiscard]] virtual const Math::Vector<3> &
  getRotation() const noexcept = 0;
  [[nodiscard]] virtual const Math::Vector<3> &
  getScale() const noexcept = 0;
  [[nodiscard]] virtual const std::unique_ptr<IMaterial> &
  getMaterial() const noexcept = 0;
};
} // namespace Raytracer::Core