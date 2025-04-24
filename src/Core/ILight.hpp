#pragma once

#include "Core/Color.hpp"
#include "Math/Point.hpp"
#include "Math/Vector.hpp"

namespace Raytracer::Core {
class ILightBase {
protected:
  ILightBase() noexcept = default;

public:
  virtual ~ILightBase() noexcept = default;

  [[nodiscard]] virtual double
  computeIllumination(const Math::Point<3> &intersectionPoint,
                      const Math::Vector<3> &normal) const noexcept = 0;

  [[nodiscard]] virtual bool castsShadow() const noexcept = 0;

  virtual void setIntensity(double intensity) noexcept = 0;
  virtual void setColor(const Color &color) noexcept = 0;

  [[nodiscard]] virtual double getIntensity() const noexcept = 0;
  [[nodiscard]] virtual const Color &getColor() const noexcept = 0;
};

class IDirectionalLight : public virtual ILightBase {
protected:
  IDirectionalLight() noexcept = default;

public:
  virtual void setDirection(const Math::Vector<3> &direction) noexcept = 0;
  [[nodiscard]] virtual const Math::Vector<3> &
  getDirection() const noexcept = 0;
  [[nodiscard]] virtual Math::Vector<3>
  getDirectionFrom(const Math::Point<3> &point) const noexcept = 0;
};

class IPositionalLight : public virtual ILightBase {
protected:
  IPositionalLight() noexcept = default;

public:
  virtual void setPosition(const Math::Point<3> &position) noexcept = 0;
  [[nodiscard]] virtual const Math::Point<3> &getPosition() const noexcept = 0;
  [[nodiscard]] virtual double
  getDistance(const Math::Point<3> &point) const noexcept = 0;
  [[nodiscard]] virtual Math::Vector<3>
  getDirectionFrom(const Math::Point<3> &point) const noexcept = 0;
};
} // namespace Raytracer::Core