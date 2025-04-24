#pragma once

#include "Math/Point.hpp"
#include "Math/Vector.hpp"

namespace Raytracer::Core {

class Ray final {
public:
  constexpr Ray() noexcept = default;
  constexpr Ray(const Math::Point<3> &origin,
                const Math::Vector<3> &direction) noexcept;
  constexpr Ray(const Math::Point<3> &origin, const Math::Vector<3> &direction,
                double minDistance, double maxDistance) noexcept;

  [[nodiscard]] constexpr Math::Point<3> getOrigin() const noexcept;
  [[nodiscard]] constexpr Math::Vector<3> getDirection() const noexcept;
  [[nodiscard]] constexpr double getMinDistance() const noexcept;
  [[nodiscard]] constexpr double getMaxDistance() const noexcept;

  [[nodiscard]] constexpr Math::Point<3> at(double t) const noexcept;
  constexpr void normalizeDirection() noexcept;

private:
  Math::Point<3> m_origin{};
  Math::Vector<3> m_direction{};
  double m_minDistance{0.0};
  double m_maxDistance{100.0};
};
} // namespace Raytracer::Core