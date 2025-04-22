#pragma once

#include <optional>
#include <memory>

#include "IMaterial.hpp"
#include "Intersection.hpp"
#include "Math/Point.hpp"
#include "Math/Vector.hpp"
#include "Ray.hpp"
#include "BoundingBox.hpp"

namespace Raytracer::Core {

class IPrimitive {

public:
  virtual ~IPrimitive() = default;

  [[nodiscard]] virtual std::optional<Intersection> intersect(const Ray &ray) const noexcept = 0;

  [[nodiscard]] virtual BoundingBox getBoundingBox() const noexcept = 0;

  virtual void transform(const Math::Point<3> &position,
                       const Math::Vector<3> &rotation,
                       const Math::Vector<3> &scale) noexcept = 0;

  void setPosition(const Math::Point<3> &position) noexcept {
    m_position = position;
  }
  void setRotation(const Math::Vector<3> &rotation) noexcept {
    m_rotation = rotation;
  }
  void setScale(const Math::Vector<3> &scale) noexcept {
    m_scale = scale;
  }
  void setMaterial(std::shared_ptr<IMaterial> material) noexcept {
    m_material = std::move(material);
  }

  [[nodiscard]] constexpr const Math::Point<3> &getPosition() const noexcept {
    return m_position;
  }
  [[nodiscard]] constexpr const Math::Vector<3> &getRotation() const noexcept {
    return m_rotation;
  }
  [[nodiscard]] constexpr const Math::Vector<3> &getScale() const noexcept {
    return m_scale;
  }
  [[nodiscard]] const std::shared_ptr<IMaterial> &getMaterial() const noexcept {
    return m_material;
  }

private:
  Math::Point<3> m_position{};
  Math::Vector<3> m_rotation{};
  Math::Vector<3> m_scale{1.0, 1.0, 1.0};
  std::shared_ptr<IMaterial> m_material{nullptr};
};
} // namespace Raytracer::Core