#pragma once

#include "IPrimitive.hpp"

namespace Raytracer::Core {

class APrimitive : public IPrimitive {
public:
  APrimitive() = default;
  ~APrimitive() override = default;

  APrimitive(const APrimitive &) = delete;
  APrimitive(APrimitive &&) = delete;
  APrimitive &operator=(const APrimitive &) = delete;
  APrimitive &operator=(APrimitive &&) = delete;

  void setPosition(const Math::Point<3> &p) noexcept override {
    m_position = p;
  }
  void setRotation(const Math::Vector<3> &r) noexcept override {
    m_rotation = r;
  }
  void setScale(const Math::Vector<3> &s) noexcept override { m_scale = s; }
  void setMaterial(std::unique_ptr<IMaterial> m) noexcept override {
    m_material = std::move(m);
  }

  [[nodiscard]] constexpr const Math::Point<3> &
  getPosition() const noexcept override {
    return m_position;
  }
  [[nodiscard]] constexpr const Math::Vector<3> &
  getRotation() const noexcept override {
    return m_rotation;
  }
  [[nodiscard]] constexpr const Math::Vector<3> &
  getScale() const noexcept override {
    return m_scale;
  }
  [[nodiscard]] const std::unique_ptr<IMaterial> &
  getMaterial() const noexcept override {
    return m_material;
  }

  void transform(const Math::Point<3> &p, const Math::Vector<3> &r,
                 const Math::Vector<3> &s) noexcept override {
    setPosition(p);
    setRotation(r);
    setScale(s);
  }

  [[nodiscard]] std::optional<Intersection>
  intersect(const Ray &ray) const noexcept override = 0;

  [[nodiscard]] BoundingBox getBoundingBox() const noexcept override = 0;

private:
  Math::Point<3> m_position{};
  Math::Vector<3> m_rotation{};
  Math::Vector<3> m_scale{1, 1, 1};
  std::unique_ptr<IMaterial> m_material{nullptr};
};
} // namespace Raytracer::Core
