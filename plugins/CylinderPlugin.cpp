#include "CylinderPlugin.hpp"
#include "Parser/SceneParser.hpp"

namespace Raytracer::Plugins {

std::unique_ptr<Plugin::PrimitivePlugin> CylinderPlugin::create() {
  return std::make_unique<CylinderPlugin>();
}

bool CylinderPlugin::configure(const libconfig::Setting &config) {
  try {
    auto position = Parser::SceneParser::parsePoint3(config.lookup("position"));
    double radius = config.lookup("radius");
    double height = config.lookup("height");
    std::string axis = config.lookup("axis");

    if (position) {
      this->setAxisPositionRadiusAndHeight(axis, *position, radius, height);

      Parser::SceneParser::applyTransformations(config, this);
    }
  } catch (const libconfig::SettingNotFoundException &) {
  }
  return true;
}

void CylinderPlugin::setAxisPositionRadiusAndHeight(const std::string &axis,
                                                    Math::Point<3> position,
                                                    double radius,
                                                    double height) noexcept {
  m_position = position;
  m_radius = radius;
  m_height = height;

  if (axis == "X") {
    m_normal = Math::Vector<3>{1.0, 0.0, 0.0};
  } else if (axis == "Y") {
    m_normal = Math::Vector<3>{0.0, 1.0, 0.0};
  } else if (axis == "Z") {
    m_normal = Math::Vector<3>{0.0, 0.0, 1.0};
  }
}

const Math::Vector<3> &CylinderPlugin::getNormal() const noexcept {
  return m_normal;
}

const Math::Point<3> &CylinderPlugin::getPosition() const noexcept {
  return m_position;
}

[[nodiscard]] std::optional<Core::Intersection>
CylinderPlugin::intersect(const Core::Ray &ray) const noexcept {
  Core::Ray localRay = getTransform().inverseTransformRay(ray);
  Math::Point<3> origin = localRay.getOrigin();
  Math::Vector<3> direction = localRay.getDirection();

  double bestT = std::numeric_limits<double>::infinity();
  Math::Point<3> localIntersectionPoint;
  Math::Vector<3> localNormal;
  Math::Point<2> uvCoords;
  bool isInside = false;

  int axisIndex = -1;
  if (m_normal.m_components[0] == 1.0)
    axisIndex = 0;
  else if (m_normal.m_components[1] == 1.0)
    axisIndex = 1;
  else
    axisIndex = 2;

  double a, b, c;
  if (axisIndex == 0) {
    a = direction.m_components[1] * direction.m_components[1] +
        direction.m_components[2] * direction.m_components[2];
    b = 2.0 * (origin.m_components[1] * direction.m_components[1] +
               origin.m_components[2] * direction.m_components[2]);
    c = origin.m_components[1] * origin.m_components[1] +
        origin.m_components[2] * origin.m_components[2] - m_radius * m_radius;
  } else if (axisIndex == 1) {
    a = direction.m_components[0] * direction.m_components[0] +
        direction.m_components[2] * direction.m_components[2];
    b = 2.0 * (origin.m_components[0] * direction.m_components[0] +
               origin.m_components[2] * direction.m_components[2]);
    c = origin.m_components[0] * origin.m_components[0] +
        origin.m_components[2] * origin.m_components[2] - m_radius * m_radius;
  } else {
    a = direction.m_components[0] * direction.m_components[0] +
        direction.m_components[1] * direction.m_components[1];
    b = 2.0 * (origin.m_components[0] * direction.m_components[0] +
               origin.m_components[1] * direction.m_components[1]);
    c = origin.m_components[0] * origin.m_components[0] +
        origin.m_components[1] * origin.m_components[1] - m_radius * m_radius;
  }

  double delta = b * b - 4 * a * c;
  if (delta >= 0 && std::abs(a) > 1e-10) {
    double t1 = (-b - std::sqrt(delta)) / (2.0 * a);
    double t2 = (-b + std::sqrt(delta)) / (2.0 * a);

    double candidateT[2] = {t1, t2};
    for (int i = 0; i < 2; i++) {
      double t = candidateT[i];

      if (t >= localRay.getMinDistance() && t <= localRay.getMaxDistance() &&
          t < bestT) {
        Math::Point<3> point = localRay.at(t);
        double axisValue =
            point.m_components[axisIndex] - m_position.m_components[axisIndex];

        if (axisValue >= -m_height / 2 && axisValue <= m_height / 2) {
          bestT = t;
          localIntersectionPoint = point;

          localNormal = Math::Vector<3>{0.0, 0.0, 0.0};
          for (int j = 0; j < 3; j++) {
            if (j != axisIndex) {
              localNormal.m_components[j] =
                  localIntersectionPoint.m_components[j] -
                  m_position.m_components[j];
            }
          }
          localNormal /= localNormal.length();

          double u;
          if (axisIndex == 0) {
            u = std::atan2(localNormal.m_components[2],
                           localNormal.m_components[1]) /
                (2 * M_PI);
          } else if (axisIndex == 1) {
            u = std::atan2(localNormal.m_components[0],
                           localNormal.m_components[2]) /
                (2 * M_PI);
          } else {
            u = std::atan2(localNormal.m_components[1],
                           localNormal.m_components[0]) /
                (2 * M_PI);
          }
          if (u < 0)
            u += 1.0;

          double v = (axisValue + m_height / 2) / m_height;

          uvCoords = Math::Point<2>{u, v};
          isInside = (i == 1);
        }
      }
    }
  }

  for (int i = 0; i < 2; i++) {
    double discPosition = m_position.m_components[axisIndex] +
                          (i == 0 ? -m_height / 2 : m_height / 2);

    if (std::abs(direction.m_components[axisIndex]) < 1e-10)
      continue;

    double t = (discPosition - origin.m_components[axisIndex]) /
               direction.m_components[axisIndex];

    if (t >= localRay.getMinDistance() && t <= localRay.getMaxDistance() &&
        t < bestT) {
      Math::Point<3> point = localRay.at(t);

      double dx = point.m_components[(axisIndex + 1) % 3] -
                  m_position.m_components[(axisIndex + 1) % 3];
      double dy = point.m_components[(axisIndex + 2) % 3] -
                  m_position.m_components[(axisIndex + 2) % 3];
      double distanceSquared = dx * dx + dy * dy;

      if (distanceSquared <= m_radius * m_radius) {
        bestT = t;
        localIntersectionPoint = point;

        localNormal = Math::Vector<3>{0.0, 0.0, 0.0};
        localNormal.m_components[axisIndex] = (i == 0) ? -1.0 : 1.0;

        double r = std::sqrt(distanceSquared) / m_radius;
        double theta = std::atan2(dy, dx) / (2 * M_PI);
        if (theta < 0)
          theta += 1.0;

        uvCoords = Math::Point<2>{r * std::cos(theta * 2 * M_PI),
                                  r * std::sin(theta * 2 * M_PI)};
        isInside = (direction.m_components[axisIndex] *
                        localNormal.m_components[axisIndex] >
                    0);
      }
    }
  }

  if (bestT < std::numeric_limits<double>::infinity()) {
    Math::Point<3> worldIntersectionPoint =
        getTransform().transformPoint(localIntersectionPoint);
    Math::Vector<3> worldNormal = getTransform().transformNormal(localNormal);
    worldNormal /= worldNormal.length();
    double worldDistance = (worldIntersectionPoint - ray.getOrigin()).length();

    return Core::Intersection(worldIntersectionPoint, worldNormal,
                              this->getMaterial(), worldDistance, isInside,
                              uvCoords);
  }

  return std::nullopt;
}

[[nodiscard]] Core::BoundingBox
CylinderPlugin::getBoundingBox() const noexcept {
  Math::Point<3> min, max;

  if (m_normal.m_components[0] == 1.0) {
    min = Math::Point<3>{m_position.m_components[0] - m_height / 2,
                         m_position.m_components[1] - m_radius,
                         m_position.m_components[2] - m_radius};
    max = Math::Point<3>{m_position.m_components[0] + m_height / 2,
                         m_position.m_components[1] + m_radius,
                         m_position.m_components[2] + m_radius};
  } else if (m_normal.m_components[1] == 1.0) {
    min = Math::Point<3>{m_position.m_components[0] - m_radius,
                         m_position.m_components[1] - m_height / 2,
                         m_position.m_components[2] - m_radius};
    max = Math::Point<3>{m_position.m_components[0] + m_radius,
                         m_position.m_components[1] + m_height / 2,
                         m_position.m_components[2] + m_radius};
  } else {
    min = Math::Point<3>{m_position.m_components[0] - m_radius,
                         m_position.m_components[1] - m_radius,
                         m_position.m_components[2] - m_height / 2};
    max = Math::Point<3>{m_position.m_components[0] + m_radius,
                         m_position.m_components[1] + m_radius,
                         m_position.m_components[2] + m_height / 2};
  }

  return Core::BoundingBox(min, max);
}

} // namespace Raytracer::Plugins

extern "C" {
Raytracer::Plugin::IPlugin *createPlugin() {
  return new Raytracer::Plugins::CylinderPlugin();
}

void destroyPlugin(Raytracer::Plugin::IPlugin *plugin) { delete plugin; }
}
