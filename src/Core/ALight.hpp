/**
 * @file ALight.hpp
 * @brief Defines abstract base and concrete light classes.
 */

#pragma once

#include "Core/Color.hpp"
#include "Core/ILight.hpp"
#include "Core/Scene.hpp"
#include "Math/Point.hpp"
#include "Math/Vector.hpp"
#include "Utility/Clamped.hpp"

namespace Raytracer::Core {
/**
 * @class ALight
 * @brief Abstract base class for all lights.
 */
class ALight : public virtual ILight {
public:
  /**
   * @brief Default constructor.
   */
  ALight() noexcept = default;

  /**
   * @brief Construct with intensity and color.
   * @param intensity Light intensity [0.0, 1.0].
   * @param color Light color.
   */
  ALight(double intensity, const Color &color) noexcept
      : m_intensity(intensity), m_color(color) {}

  /**
   * @brief Destructor.
   */
  ~ALight() noexcept override = default;

  ALight(const ALight &) = delete;
  ALight(ALight &&) = delete;
  ALight &operator=(const ALight &) = delete;
  ALight &operator=(ALight &&) = delete;

  /**
   * @brief Set light intensity.
   * @param i New intensity [0.0, 1.0].
   */
  void setIntensity(double i) noexcept override { m_intensity = i; }

  /**
   * @brief Set light color.
   * @param c New color.
   */
  void setColor(const Color &c) noexcept override { m_color = c; }

  /**
   * @brief Get light intensity.
   * @return Current intensity.
   */
  [[nodiscard]] constexpr double getIntensity() const noexcept override {
    return m_intensity.get();
  }

  /**
   * @brief Get light color.
   * @return Current color.
   */
  [[nodiscard]] constexpr const Color &getColor() const noexcept override {
    return m_color;
  }

  /**
   * @brief Whether this light casts shadows.
   * @return true if casts shadow, false otherwise.
   */
  [[nodiscard]] bool castsShadow() const noexcept override = 0;

private:
  Utility::Clamped<double, 0.0, 1.0> m_intensity{1.0};
  Color m_color;
};

/**
 * @class ADirectionalLight
 * @brief Directional light source with parallel rays.
 */
class ADirectionalLight : public IDirectionalLight, public ALight {
public:
  /**
   * @brief Default constructor.
   */
  ADirectionalLight() noexcept = default;

  /**
   * @brief Construct with given direction.
   * @param direction Initial light direction.
   */
  explicit ADirectionalLight(const Math::Vector<3> &direction) noexcept {
    setDirection(direction);
  }

  /**
   * @brief Set the light direction (normalized).
   * @param direction New direction vector.
   */
  void setDirection(const Math::Vector<3> &direction) noexcept override {
    m_direction = direction.length() > 0 ? direction / direction.length()
                                         : Math::Vector<3>{0.0, -1.0, 0.0};
  }

  /**
   * @brief Get the light direction.
   * @return Normalized direction vector.
   */
  [[nodiscard]] const Math::Vector<3> &getDirection() const noexcept override {
    return m_direction;
  }

  /**
   * @brief Get direction from a point (same for all points).
   * @param point Ignored.
   * @return Normalized direction vector.
   */
  [[nodiscard]] Math::Vector<3> getDirectionFrom(
      [[maybe_unused]] const Math::Point<3> &point) const noexcept override {
    return m_direction;
  }

  /**
   * @brief Compute illumination at a point with surface normal.
   * @param intersectionPoint Surface point (ignored).
   * @param normal Surface normal at the point.
   * @return Illumination factor [0.0, intensity].
   */
  [[nodiscard]] double
  computeIllumination([[maybe_unused]] const Math::Point<3> &intersectionPoint,
                      const Math::Vector<3> &normal) const noexcept override {
    double dot = normal.dot(m_direction * -1.0);
    return dot > 0.0 ? dot * getIntensity() : 0.0;
  }

private:
  Math::Vector<3> m_direction{}; /**< Normalized direction. */
};

/**
 * @class APositionalLight
 * @brief Point light source with attenuation.
 */
class APositionalLight : public IPositionalLight, public ALight {
public:
  /**
   * @brief Default constructor.
   */
  APositionalLight() noexcept = default;

  /**
   * @brief Construct with given position.
   * @param position Initial light position.
   */
  explicit APositionalLight(const Math::Point<3> &position) noexcept
      : m_position(position) {}

  /**
   * @brief Set light position.
   * @param p New position.
   */
  void setPosition(const Math::Point<3> &p) noexcept override {
    m_position = p;
  }

  /**
   * @brief Get light position.
   * @return Current position.
   */
  [[nodiscard]] constexpr const Math::Point<3> &
  getPosition() const noexcept override {
    return m_position;
  }

  /**
   * @brief Compute distance from light to a point.
   * @param point Target point.
   * @return Euclidean distance.
   */
  [[nodiscard]] double
  getDistance(const Math::Point<3> &point) const noexcept override {
    const Math::Vector<3> diff = point - m_position;
    return diff.length();
  }

  /**
   * @brief Get direction from a point to the light.
   * @param point Target surface point.
   * @return Normalized vector pointing to light.
   */
  [[nodiscard]] Math::Vector<3>
  getDirectionFrom(const Math::Point<3> &point) const noexcept override {
    Math::Vector<3> direction = m_position - point;
    double length = direction.length();
    if (length > 0.0) {
      return direction / length;
    }
    return Math::Vector<3>{0.0, 0.0, 1.0};
  }

  /**
   * @brief Compute illumination with attenuation.
   * @param intersectionPoint Surface point.
   * @param normal Surface normal at the point.
   * @return Illumination factor [0.0, intensity].
   */
  [[nodiscard]] double
  computeIllumination(const Math::Point<3> &intersectionPoint,
                      const Math::Vector<3> &normal) const noexcept override {
    Math::Vector<3> lightDir = getDirectionFrom(intersectionPoint);
    double dot = normal.dot(lightDir);
    double distance = getDistance(intersectionPoint);
    double attenuation =
        1.0 / (1.0 + 0.1 * distance + 0.01 * distance * distance);
    return dot > 0.0 ? dot * getIntensity() * attenuation : 0.0;
  }

  /**
   * @brief Computes the illumination considering shadows
   * @param intersectionPoint The point of intersection
   * @param normal The surface normal at the intersection point
   * @param scene The scene containing all objects
   * @return The illumination value considering shadows
   */
  [[nodiscard]] virtual double
  computeIllumination(const Math::Point<3> &intersectionPoint,
                      const Math::Vector<3> &normal,
                      const Core::Scene &scene) const noexcept = 0;

private:
  Math::Point<3> m_position{};
};

} // namespace Raytracer::Core
