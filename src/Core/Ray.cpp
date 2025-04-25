#include "Core/Ray.hpp"

namespace Raytracer::Core {

Ray::Ray(const Math::Point<3> &origin,
         const Math::Vector<3> &direction) noexcept
    : m_origin(origin), m_direction(direction) {}

Ray::Ray(const Math::Point<3> &origin, const Math::Vector<3> &direction,
         double minDistance, double maxDistance) noexcept
    : m_origin(origin), m_direction(direction), m_minDistance(minDistance),
      m_maxDistance(maxDistance) {}

Math::Point<3> Ray::getOrigin() const noexcept { return m_origin; }

Math::Vector<3> Ray::getDirection() const noexcept { return m_direction; }

double Ray::getMinDistance() const noexcept { return m_minDistance; }

double Ray::getMaxDistance() const noexcept { return m_maxDistance; }

Math::Point<3> Ray::at(double t) const noexcept {
  return m_origin + m_direction * t;
}

void Ray::normalizeDirection() noexcept {
  double length = m_direction.length();
  if (length > 0) {
    m_direction = m_direction / length;
  }
}
} // namespace Raytracer::Core