/**
 * @file APrimitive.hpp
 * @brief Defines the abstract base primitive with transform and material.
 */

#pragma once

#include "Core/IPrimitive.hpp"

namespace Raytracer::Core {

/**
 * @class APrimitive
 * @brief Abstract base class for scene primitives.
 */
class APrimitive : public IPrimitive {
public:
  /**
   * @brief Default constructor.
   */
  APrimitive() = default;

  /**
   * @brief Destructor.
   */
  ~APrimitive() override = default;

  APrimitive(const APrimitive &) = delete;
  APrimitive(APrimitive &&) = delete;
  APrimitive &operator=(const APrimitive &) = delete;
  APrimitive &operator=(APrimitive &&) = delete;

  /**
   * @brief Set primitive position.
   * @param p New position.
   */
  void setPosition(const Math::Point<3> &p) noexcept override {
    m_position = p;
  }

  /**
   * @brief Set primitive rotation.
   * @param r New rotation.
   */
  void setRotation(const Math::Vector<3> &r) noexcept override {
    m_rotation = r;
  }

  /**
   * @brief Set primitive scale.
   * @param s New scale.
   */
  void setScale(const Math::Vector<3> &s) noexcept override { m_scale = s; }

  /**
   * @brief Set primitive material.
   * @param m Material to apply.
   */
  void setMaterial(std::shared_ptr<IMaterial> m) noexcept override {
    m_material = std::move(m);
  }

  /**
   * @brief Get primitive position.
   * @return Current position.
   */
  [[nodiscard]] constexpr const Math::Point<3> &
  getPosition() const noexcept override {
    return m_position;
  }

  /**
   * @brief Get primitive rotation.
   * @return Current rotation.
   */
  [[nodiscard]] constexpr const Math::Vector<3> &
  getRotation() const noexcept override {
    return m_rotation;
  }

  /**
   * @brief Get primitive scale.
   * @return Current scale.
   */
  [[nodiscard]] constexpr const Math::Vector<3> &
  getScale() const noexcept override {
    return m_scale;
  }

  /**
   * @brief Get primitive material.
   * @return Material pointer.
   */
  [[nodiscard]] const std::shared_ptr<IMaterial> &
  getMaterial() const noexcept override {
    return m_material;
  }

  /**
   * @brief Apply position, rotation, and scale to primitive.
   * @param p Position to set.
   * @param r Rotation to set.
   * @param s Scale to set.
   */
  void transform(const Math::Point<3> &p, const Math::Vector<3> &r,
                 const Math::Vector<3> &s) noexcept override {
    setPosition(p);
    setRotation(r);
    setScale(s);
  }

  /**
   * @brief Compute intersection with a ray.
   * @param ray Ray to test.
   * @return Intersection info if hit.
   */
  [[nodiscard]] std::optional<Intersection>
  intersect(const Ray &ray) const noexcept override = 0;

  /**
   * @brief Get the axis-aligned bounding box.
   * @return Bounding box of the primitive.
   */
  [[nodiscard]] BoundingBox getBoundingBox() const noexcept override = 0;

private:
  Math::Point<3> m_position{};
  Math::Vector<3> m_rotation{};
  Math::Vector<3> m_scale{1.0, 1.0, 1.0};
  std::shared_ptr<IMaterial> m_material{nullptr};
};

} // namespace Raytracer::Core
