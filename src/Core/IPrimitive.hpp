/**
 * @file IPrimitive.hpp
 * @brief Defines the interface for scene primitives.
 */

#pragma once

#include <memory>
#include <optional>

#include "Core/BoundingBox.hpp"
#include "Core/Intersection.hpp"
#include "Core/Ray.hpp"
#include "Math/Point.hpp"
#include "Math/Vector.hpp"

namespace Raytracer::Core {

/**
 * @class IPrimitive
 * @brief Interface for geometric primitives in the scene.
 */
class IPrimitive {
public:
  /**
   * @brief Virtual destructor.
   */
  virtual ~IPrimitive() = default;

  /**
   * @brief Compute intersection with a ray.
   * @param ray Ray to test against.
   * @return Intersection data if hit, std::nullopt otherwise.
   */
  [[nodiscard]] virtual std::optional<Intersection>
  intersect(const Ray &ray) const noexcept = 0;

  /**
   * @brief Get the axis-aligned bounding box of the primitive.
   * @return Bounding box enclosing the primitive.
   */
  [[nodiscard]] virtual BoundingBox getBoundingBox() const noexcept = 0;

  /**
   * @brief Apply transformation to the primitive.
   * @param position New position.
   * @param rotation New rotation vector.
   * @param scale New scale vector.
   */
  virtual void transform(const Math::Point<3> &position,
                         const Math::Vector<3> &rotation,
                         const Math::Vector<3> &scale) noexcept = 0;

  /**
   * @brief Set the primitive's position.
   * @param position New position.
   */
  virtual void setPosition(const Math::Point<3> &position) noexcept = 0;

  /**
   * @brief Set the primitive's rotation.
   * @param rotation New rotation vector.
   */
  virtual void setRotation(const Math::Vector<3> &rotation) noexcept = 0;

  /**
   * @brief Set the primitive's scale.
   * @param scale New scale vector.
   */
  virtual void setScale(const Math::Vector<3> &scale) noexcept = 0;

  /**
   * @brief Assign a material to the primitive.
   * @param material Shared pointer to the material.
   */
  virtual void setMaterial(std::shared_ptr<IMaterial> material) noexcept = 0;

  /**
   * @brief Get the primitive's position.
   * @return Current position.
   */
  [[nodiscard]] virtual const Math::Point<3> &getPosition() const noexcept = 0;

  /**
   * @brief Get the primitive's rotation.
   * @return Current rotation vector.
   */
  [[nodiscard]] virtual const Math::Vector<3> &getRotation() const noexcept = 0;

  /**
   * @brief Get the primitive's scale.
   * @return Current scale vector.
   */
  [[nodiscard]] virtual const Math::Vector<3> &getScale() const noexcept = 0;

  /**
   * @brief Get the local center of this primitive.
   * @return The local center point for transformations.
   */
  [[nodiscard]] virtual Math::Point<3> getLocalCenter() const noexcept = 0;

  /**
   * @brief Get the assigned material.
   * @return Constant reference to the material pointer.
   */
  [[nodiscard]] virtual const std::shared_ptr<IMaterial> &
  getMaterial() const noexcept = 0;
};

} // namespace Raytracer::Core
