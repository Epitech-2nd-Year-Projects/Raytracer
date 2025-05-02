#include "Core/Scene.hpp"
#include "Core/Intersection.hpp"
#include "Core/Ray.hpp"

namespace Raytracer::Core {

bool Scene::hasIntersection(const Ray &ray) const {
  for (const auto &[id, primitive] : m_primitives) {
    if (primitive->intersect(ray).has_value()) {
      return true;
    }
  }

  for (const auto &[id, childScene] : m_childScenes) {
    if (childScene->hasIntersection(ray)) {
      return true;
    }
  }
  return false;
}

std::optional<Intersection>
Scene::findNearestIntersection(const Ray &ray) const {
  std::optional<Intersection> nearestHit;
  double nearestDistance = std::numeric_limits<double>::infinity();

  for (const auto &[id, primitive] : m_primitives) {
    if (auto hit = primitive->intersect(ray)) {
      if (hit->getDistance() < nearestDistance) {
        nearestDistance = hit->getDistance();
        nearestHit = hit;
      }
    }
  }

  for (const auto &[id, childScene] : m_childScenes) {
    if (auto childHit = childScene->findNearestIntersection(ray)) {
      if (childHit->getDistance() < nearestDistance) {
        nearestDistance = childHit->getDistance();
        nearestHit = childHit;
      }
    }
  }
  return nearestHit;
}

} // namespace Raytracer::Core
