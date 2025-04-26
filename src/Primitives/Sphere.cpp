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
  Math::Vector<3> oc = ray.getOrigin() - m_center;

  double a = ray.getDirection().dot(ray.getDirection());
  double b = 2.0 * oc.dot(ray.getDirection());
  double c = oc.dot(oc) - m_radius * m_radius;

  double delta = b * b - 4 * a * c;
  if (delta < 0) {
    return std::nullopt;
  }
  double t1 = (-b - std::sqrt(delta)) / (2.0 * a);
  double t2 = (-b + std::sqrt(delta)) / (2.0 * a);

  bool t1Valid = t1 >= ray.getMinDistance() && t1 <= ray.getMaxDistance();
  bool t2Valid = t2 >= ray.getMinDistance() && t2 <= ray.getMaxDistance();
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

  Math::Point<3> intersectionPoint = ray.at(t);
  Math::Vector<3> dirToCenter = Math::Vector<3>(intersectionPoint - m_center);
  Math::Vector<3> normal = dirToCenter / m_radius;

  bool isInside = false;
  if (normal.dot(ray.getDirection()) > 0) {
    normal = normal * -1.0;
    isInside = true;
  }

  double phi = std::atan2(normal.m_components[2], normal.m_components[0]);
  double theta = std::acos(normal.m_components[1]);

  double u = (phi + M_PI) / (2 * M_PI);
  double v = theta / M_PI;

  return Core::Intersection(intersectionPoint, normal, this->getMaterial(), t,
                            isInside, Math::Point<2>{u, v});
}

[[nodiscard]] Core::BoundingBox Sphere::getBoundingBox() const noexcept {
  Math::Vector<3> radiusVec(m_radius, m_radius, m_radius);
  Math::Point<3> min = m_center - radiusVec;
  Math::Point<3> max = m_center + radiusVec;
  return Core::BoundingBox(min, max);
}

} // namespace Raytracer::Primitives