/**
 * @file ILight.hpp
 * @brief Defines light interface classes.
 */

#pragma once

#include "Core/Color.hpp"
#include "Math/Vector.hpp"

namespace Raytracer::Core {

/**
 * @class ILight
 * @brief Base interface for light sources.
 */
class ILight {
protected:
  /**
   * @brief Protected default constructor.
   */
  ILight() noexcept = default;

public:
  /**
   * @brief Virtual destructor.
   */
  virtual ~ILight() noexcept = default;

  /**
   * @brief Compute illumination at a surface point.
   * @param intersectionPoint Point on the surface.
   * @param normal Surface normal at the point.
   * @return Illumination factor [0.0, intensity].
   */
  [[nodiscard]] virtual double
  computeIllumination(const Math::Point<3> &intersectionPoint,
                      const Math::Vector<3> &normal) const noexcept = 0;

  /**
   * @brief Check if the light casts shadows.
   * @return true if shadows are cast.
   */
  [[nodiscard]] virtual bool castsShadow() const noexcept = 0;

  /**
   * @brief Set the light intensity.
   * @param intensity New intensity value.
   */
  virtual void setIntensity(double intensity) noexcept = 0;

  /**
   * @brief Set the light color.
   * @param color New light color.
   */
  virtual void setColor(const Color &color) noexcept = 0;

  /**
   * @brief Get the light intensity.
   * @return Current intensity.
   */
  [[nodiscard]] virtual double getIntensity() const noexcept = 0;

  /**
   * @brief Get the light color.
   * @return Current color.
   */
  [[nodiscard]] virtual const Color &getColor() const noexcept = 0;
};

/**
 * @class IDirectionalLight
 * @brief Interface for directional lights (parallel rays).
 */
class IDirectionalLight : public virtual ILight {
protected:
  /**
   * @brief Protected default constructor.
   */
  IDirectionalLight() noexcept = default;

public:
  /**
   * @brief Set the light direction.
   * @param direction Normalized direction vector.
   */
  virtual void setDirection(const Math::Vector<3> &direction) noexcept = 0;

  /**
   * @brief Get the light direction.
   * @return Normalized direction vector.
   */
  [[nodiscard]] virtual const Math::Vector<3> &
  getDirection() const noexcept = 0;

  /**
   * @brief Get direction from a given point (same for all points).
   * @param point Reference point (ignored).
   * @return Direction vector.
   */
  [[nodiscard]] virtual Math::Vector<3>
  getDirectionFrom(const Math::Point<3> &point) const noexcept = 0;
};

/**
 * @class IPositionalLight
 * @brief Interface for point lights with position-based attenuation.
 */
class IPositionalLight : public virtual ILight {
protected:
  /**
   * @brief Protected default constructor.
   */
  IPositionalLight() noexcept = default;

public:
  /**
   * @brief Set the light position.
   * @param position New position in space.
   */
  virtual void setPosition(const Math::Point<3> &position) noexcept = 0;

  /**
   * @brief Get the light position.
   * @return Current position.
   */
  [[nodiscard]] virtual const Math::Point<3> &getPosition() const noexcept = 0;

  /**
   * @brief Compute distance from light to a point.
   * @param point Target point.
   * @return Euclidean distance.
   */
  [[nodiscard]] virtual double
  getDistance(const Math::Point<3> &point) const noexcept = 0;

  /**
   * @brief Get direction from a point to the light.
   * @param point Reference point on surface.
   * @return Normalized vector pointing towards the light.
   */
  [[nodiscard]] virtual Math::Vector<3>
  getDirectionFrom(const Math::Point<3> &point) const noexcept = 0;
};

class IAmbientLight : public virtual ILight {
protected:
  /**
   * @brief Protected default constructor.
   */
  IAmbientLight() noexcept = default;
};

} // namespace Raytracer::Core
