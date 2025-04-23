#pragma once

#include <memory>
#include <optional>

#include "BoundingBox.hpp"
#include "IMaterial.hpp"
#include "Intersection.hpp"
#include "Math/Point.hpp"
#include "Math/Vector.hpp"
#include "Ray.hpp"

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
  virtual void setMaterial(std::shared_ptr<IMaterial> material) noexcept = 0;

  virtual [[nodiscard]] constexpr const Math::Point<3> &
  getPosition() const noexcept = 0;
  virtual [[nodiscard]] constexpr const Math::Vector<3> &
  getRotation() const noexcept = 0;
  virtual [[nodiscard]] constexpr const Math::Vector<3> &
  getScale() const noexcept = 0;
  virtual [[nodiscard]] const std::shared_ptr<IMaterial> &
  getMaterial() const noexcept = 0;

private:
  Math::Point<3> m_position{};
  Math::Vector<3> m_rotation{};
  Math::Vector<3> m_scale{1.0, 1.0, 1.0};
  std::shared_ptr<IMaterial> m_material{nullptr};
};
} // namespace Raytracer::Core