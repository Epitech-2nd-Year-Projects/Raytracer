#include "Core/Intersection.hpp"

namespace Raytracer::Core {
Intersection::Intersection(const Math::Point<3> &point,
                           const Math::Vector<3> &normal,
                           std::shared_ptr<IMaterial> material, double distance,
                           bool isInside, const Math::Point<2> &uv) noexcept
    : m_point(point), m_normal(normal), m_material(std::move(material)),
      m_distance(distance), m_isInside(isInside), m_uv(uv) {}

const Math::Point<3> &Intersection::getPoint() const noexcept {
  return m_point;
}

std::shared_ptr<IMaterial> Intersection::getMaterial() const noexcept {
  return m_material;
}

double Intersection::getDistance() const noexcept { return m_distance; }

bool Intersection::getIsInside() const noexcept { return m_isInside; }

const Math::Vector<3> &Intersection::getNormal() const noexcept {
  return m_normal;
}

const Math::Point<2> &Intersection::getUv() const noexcept { return m_uv; }
} // namespace Raytracer::Core