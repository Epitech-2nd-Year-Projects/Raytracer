#pragma once

#include "Core/Color.hpp"
#include "Core/ILight.hpp"
#include "Math/Point.hpp"
#include "Math/Vector.hpp"
#include "Utility/Clamped.hpp"

namespace Raytracer::Core {

class ALightBase : public virtual ILightBase {
public:
  ALightBase() noexcept = default;
  ALightBase(double intensity, const Color &color) noexcept
      : m_intensity(intensity), m_color(color) {}
  ~ALightBase() noexcept override = default;

  ALightBase(const ALightBase &) = delete;
  ALightBase(ALightBase &&) = delete;
  ALightBase &operator=(const ALightBase &) = delete;
  ALightBase &operator=(ALightBase &&) = delete;

  void setIntensity(double i) noexcept override { m_intensity = i; }
  void setColor(const Color &c) noexcept override { m_color = c; }

  [[nodiscard]] constexpr double getIntensity() const noexcept override {
    return m_intensity.get();
  }
  [[nodiscard]] constexpr const Color &getColor() const noexcept override {
    return m_color;
  }

  [[nodiscard]] bool castsShadow() const noexcept override = 0;

private:
  Utility::Clamped<double, 0.0, 1.0> m_intensity{1.0};
  Color m_color;
};

class ADirectionalLight : public IDirectionalLight, public ALightBase {
public:
  ADirectionalLight() noexcept = default;
  explicit ADirectionalLight(const Math::Vector<3> &direction) noexcept {
    ADirectionalLight::setDirection(direction);
  }

  void setDirection(const Math::Vector<3> &direction) noexcept override {
    m_direction = direction.length() > 0 ? direction / direction.length()
                                         : Math::Vector<3>{0, -1, 0};
  }

  [[nodiscard]] const Math::Vector<3> &getDirection() const noexcept override {
    return m_direction;
  }

  [[nodiscard]] Math::Vector<3> getDirectionFrom(
      [[maybe_unused]] const Math::Point<3> &point) const noexcept override {
    return m_direction;
  }

  [[nodiscard]] double
  computeIllumination(const Math::Point<3> &intersectionPoint,
                      const Math::Vector<3> &normal) const noexcept override {
    double dot = normal.dot(m_direction * -1.0);
    return dot > 0.0 ? dot * getIntensity() : 0.0;
  }

private:
  Math::Vector<3> m_direction{};
};

class APositionalLight : public IPositionalLight, public ALightBase {
public:
  APositionalLight() noexcept = default;
  explicit APositionalLight(const Math::Point<3> &position) noexcept
      : m_position(position) {}

  void setPosition(const Math::Point<3> &p) noexcept override {
    m_position = p;
  }
  [[nodiscard]] constexpr const Math::Point<3> &
  getPosition() const noexcept override {
    return m_position;
  }

  [[nodiscard]] double
  getDistance(const Math::Point<3> &point) const noexcept override {
    const Math::Vector<3> diff = point - m_position;
    return diff.length();
  }

  [[nodiscard]] Math::Vector<3>
  getDirectionFrom(const Math::Point<3> &point) const noexcept override {
    Math::Vector<3> direction = m_position - point;
    double length = direction.length();

    if (length > 0.0) {
      return direction / length;
    }
    return Math::Vector<3>{0, 0, 1};
  }

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

private:
  Math::Point<3> m_position{};
};
} // namespace Raytracer::Core
