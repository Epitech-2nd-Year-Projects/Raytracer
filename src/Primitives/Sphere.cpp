/**
 * @file Sphere.cpp
 * @brief Implementation of the Sphere class, providing functionality for sphere
 * primitive operations in ray tracing
 */

#include "Primitives/Sphere.hpp"

namespace Raytracer::Primitives {

Sphere::Sphere(const Math::Point<3> &center, double radius) noexcept
    : m_radius(radius), m_center(center) {}

void Sphere::setRadius(double r) noexcept { m_radius = r; }
void Sphere::setCenter(const Math::Point<3> &center) noexcept {
  m_center = center;
}

double Sphere::getRadius() const noexcept { return m_radius; }
const Math::Point<3> &Sphere::getCenter() const noexcept { return m_center; }

[[nodiscard]] std::optional<Core::Intersection>
Sphere::intersect(const Core::Ray &ray) const noexcept {
  Core::Ray localRay = getTransform().inverseTransformRay(ray);
  Math::Vector<3> oc = localRay.getOrigin() - m_center;

  double a = localRay.getDirection().dot(localRay.getDirection());
  double b = 2.0 * oc.dot(localRay.getDirection());
  double c = oc.dot(oc) - m_radius * m_radius;

  double delta = b * b - 4 * a * c;
  if (delta < 0) {
    return std::nullopt;
  }
  double t1 = (-b - std::sqrt(delta)) / (2.0 * a);
  double t2 = (-b + std::sqrt(delta)) / (2.0 * a);

  bool t1Valid =
      t1 >= localRay.getMinDistance() && t1 <= localRay.getMaxDistance();
  bool t2Valid =
      t2 >= localRay.getMinDistance() && t2 <= localRay.getMaxDistance();
  if (!t1Valid && !t2Valid) {
    return std::nullopt;
  }

  double t = 0.0;
  if (t1Valid && t2Valid) {
    t = std::min(t1, t2);
  } else if (t1Valid) {
    t = t1;
  } else {
    t = t2;
  }

  Math::Point<3> localIntersectionPoint = localRay.at(t);
  Math::Vector<3> localDirToCenter =
      Math::Vector<3>(localIntersectionPoint - m_center);
  Math::Vector<3> localNormal = localDirToCenter / m_radius;

  bool isInside = false;
  if (localNormal.dot(localRay.getDirection()) > 0) {
    localNormal = localNormal * -1.0;
    isInside = true;
  }

  double phi =
      std::atan2(localNormal.m_components[2], localNormal.m_components[0]);
  double theta = std::acos(localNormal.m_components[1]);
  double u = (phi + M_PI) / (2 * M_PI);
  double v = theta / M_PI;

  Math::Point<3> worldIntersectionPoint =
      getTransform().transformPoint(localIntersectionPoint);
  Math::Vector<3> worldNormal = getTransform().transformNormal(localNormal);
  double worldDistance = (worldIntersectionPoint - ray.getOrigin()).length();

  return Core::Intersection(worldIntersectionPoint, worldNormal,
                            this->getMaterial(), worldDistance, isInside,
                            Math::Point<2>{u, v});
}

[[nodiscard]] Core::BoundingBox Sphere::getBoundingBox() const noexcept {
  Math::Vector<3> radiusVec(m_radius, m_radius, m_radius);
  Math::Point<3> min = m_center - radiusVec;
  Math::Point<3> max = m_center + radiusVec;
  return Core::BoundingBox(min, max);
}

[[nodiscard]] Math::Point<3> Sphere::getLocalCenter() const noexcept {
  return m_center;
}

} // namespace Raytracer::Primitives