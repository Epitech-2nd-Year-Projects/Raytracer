#include "PlanePlugin.hpp"
#include "Parser/SceneParser.hpp"

namespace Raytracer::Plugins {

std::unique_ptr<Plugin::PrimitivePlugin> PlanePlugin::create() {
  return std::make_unique<PlanePlugin>();
}

bool PlanePlugin::configure(const libconfig::Setting &config) {
  try {
    std::string axis = config.lookup("axis");
    double position = config.lookup("position");

    if (position) {
      this->setAxisAndPosition(axis, Math::Point<3>(0.0, 0.0, position));

      Parser::SceneParser::applyTransformations(config, this);
    }
  } catch (const libconfig::SettingNotFoundException &) {
  }
  return true;
}

void PlanePlugin::setAxisAndPosition(const std::string &axis,
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

const Math::Vector<3> &PlanePlugin::getNormal() const noexcept {
  return m_normal;
}

const Math::Point<3> &PlanePlugin::getPosition() const noexcept {
  return m_position;
}

[[nodiscard]] std::optional<Core::Intersection>
PlanePlugin::intersect(const Core::Ray &ray) const noexcept {
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

[[nodiscard]] Core::BoundingBox PlanePlugin::getBoundingBox() const noexcept {
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

} // namespace Raytracer::Plugins

extern "C" {
Raytracer::Plugin::IPlugin *createPlugin() {
  return new Raytracer::Plugins::PlanePlugin();
}

void destroyPlugin(Raytracer::Plugin::IPlugin *plugin) { delete plugin; }
}
