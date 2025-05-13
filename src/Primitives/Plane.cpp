/**
 * @file Plane.cpp
 * @brief Implementation of the Plane class, providing functionality for plane
 * primitive operations in ray tracing
 */

#include "Primitives/Plane.hpp"
#include "Math/Point.hpp"
#include <limits>

namespace Raytracer::Primitives {

Plane::Plane(const std::string &axis, Math::Point<3> position) noexcept {
  setAxisAndPosition(axis, position);
}

void Plane::setAxisAndPosition(const std::string &axis,
                               Math::Point<3> position) noexcept {
  m_position = position;

  if (axis == "X") {
    m_normal = Math::Vector<3>{1.0, 0.0, 0.0};
  } else if (axis == "Y") {
    m_normal = Math::Vector<3>{0.0, 1.0, 0.0};
  } else if (axis == "Z") {
    m_normal = Math::Vector<3>{0.0, 0.0, 1.0};
  }
}

const Math::Vector<3> &Plane::getNormal() const noexcept { return m_normal; }

const Math::Point<3> &Plane::getPosition() const noexcept { return m_position; }

[[nodiscard]] std::optional<Core::Intersection>
Plane::intersect(const Core::Ray &ray) const noexcept {
  Core::Ray localRay = getTransform().inverseTransformRay(ray);
  double denominateur = m_normal.dot(localRay.getDirection());
  if (std::abs(denominateur) < 1e-8) {
    return std::nullopt;
  }

  Math::Vector<3> vec = m_position - localRay.getOrigin();
  double t = vec.dot(m_normal) / denominateur;
  if (t < localRay.getMinDistance() || t > localRay.getMaxDistance()) {
    return std::nullopt;
  }

  Math::Point<3> localIntersectionPoint = localRay.at(t);
  bool isInside = denominateur > 0;
  Math::Vector<3> localNormal = isInside ? -m_normal : m_normal;

  double u = 0.0, v = 0.0;
  if (m_normal.m_components[0] == 1.0) {
    u = localIntersectionPoint.m_components[1];
    v = localIntersectionPoint.m_components[2];
  } else if (m_normal.m_components[1] == 1.0) {
    u = localIntersectionPoint.m_components[0];
    v = localIntersectionPoint.m_components[2];
  } else {
    u = localIntersectionPoint.m_components[0];
    v = localIntersectionPoint.m_components[1];
  }

  Math::Point<3> worldIntersectionPoint =
      getTransform().transformPoint(localIntersectionPoint);
  Math::Vector<3> worldNormal = getTransform().transformNormal(localNormal);
  double worldDistance = (worldIntersectionPoint - ray.getOrigin()).length();

  return Core::Intersection(worldIntersectionPoint, worldNormal,
                            this->getMaterial(), worldDistance, isInside,
                            Math::Point<2>{u, v});
}

[[nodiscard]] Core::BoundingBox Plane::getBoundingBox() const noexcept {
  constexpr double inf = std::numeric_limits<double>::infinity();
  Math::Point<3> min, max;

  if (m_normal.m_components[0] == 1.0) {
    min = Math::Point<3>{m_position.m_components[0], -inf, -inf};
    max = Math::Point<3>{m_position.m_components[0], inf, inf};
  } else if (m_normal.m_components[1] == 1.0) {
    min = Math::Point<3>{-inf, m_position.m_components[1], -inf};
    max = Math::Point<3>{inf, m_position.m_components[1], inf};
  } else {
    min = Math::Point<3>{-inf, -inf, m_position.m_components[2]};
    max = Math::Point<3>{inf, inf, m_position.m_components[2]};
  }

  return Core::BoundingBox(min, max);
}

[[nodiscard]] Math::Point<3> Plane::getLocalCenter() const noexcept {
  return m_position;
}

} // namespace Raytracer::Primitives
