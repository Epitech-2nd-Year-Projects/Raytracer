/**
 * @file Cone.cpp
 * @brief Implementation of the Cone class, providing functionality for cone
 * primitive operations in ray tracing
 */

#include "Primitives/Cone.hpp"
#include "Math/Point.hpp"
#include <limits>

using Raytracer::Math::Point;
using Raytracer::Math::Vector;

namespace Raytracer::Primitives {

Cone::Cone(const std::string &axis, Point<3> position, double radius,
  double height) noexcept
{
  setAxisPositionRadiusHeight(axis, position, radius, height);
}

void Cone::setAxisPositionRadiusHeight(const std::string &axis,
  const Point<3> &position, double radius, double height) noexcept
{
  m_position = position;
  m_radius = radius;
  m_height = height;

  if (axis == "X")
    m_axis = Vector<3>{1.0, 0.0, 0.0};
  else if (axis == "Y")
    m_axis = Vector<3>{0.0, 1.0, 0.0};
  else
    m_axis = Vector<3>{0.0, 0.0, 1.0};
}

std::optional<Core::Intersection>Cone::intersect(const Core::Ray &ray) const noexcept
{
  Core::Ray localRay = getTransform().inverseTransformRay(ray);
  Point<3> o = localRay.getOrigin();
  Vector<3> d = localRay.getDirection();

  int axisIdx = m_axis.m_components[0] == 1.0 ? 0 :
    (m_axis.m_components[1] == 1.0 ? 1 : 2);

  const auto &oa = o.m_components[axisIdx];
  const auto &da = d.m_components[axisIdx];
  double op1 = o.m_components[(axisIdx + 1) % 3];
  double op2 = o.m_components[(axisIdx + 2) % 3];
  double dp1 = d.m_components[(axisIdx + 1) % 3];
  double dp2 = d.m_components[(axisIdx + 2) % 3];
  double k = m_radius / m_height;
  double k2 = k * k;

  double A = dp1 * dp1 + dp2 * dp2 - k2 * da * da;
  double B = 2.0 * (op1 * dp1 + op2 * dp2 - k2 * oa * da);
  double C = op1 * op1 + op2 * op2 - k2 * oa * oa;

  double bestT = std::numeric_limits<double>::infinity();
  Point<3> bestPt;
  Vector<3> bestNormal;
  Point<2> bestUV;
  bool isInside = false;

  if (std::abs(A) > 1e-10) {
    double disc = B * B - 4.0 * A * C;
    if (disc >= 0.0) {
      double sqrtDisc = std::sqrt(disc);
      double tVals[2] = {(-B - sqrtDisc) / (2.0 * A), (-B + sqrtDisc) / (2.0 * A)};
      for (double t : tVals) {
        if (t < localRay.getMinDistance() || t > localRay.getMaxDistance() ||
            t >= bestT)
          continue;
        Point<3> p = localRay.at(t);
        double axVal = p.m_components[axisIdx];
        if (axVal < 0.0 || axVal > m_height)
          continue;

        bestT = t;
        bestPt = p;

        Vector<3> n{0.0, 0.0, 0.0};
        double nz = -k2 * p.m_components[axisIdx];
        n.m_components[axisIdx] = nz;
        n.m_components[(axisIdx + 1) % 3] = p.m_components[(axisIdx + 1) % 3];
        n.m_components[(axisIdx + 2) % 3] = p.m_components[(axisIdx + 2) % 3];
        n /= n.length();
        bestNormal = n;

        double u;
        if (axisIdx == 0)
          u = std::atan2(p.m_components[2], p.m_components[1]) / (2 * M_PI);
        else if (axisIdx == 1)
          u = std::atan2(p.m_components[0], p.m_components[2]) / (2 * M_PI);
        else
          u = std::atan2(p.m_components[1], p.m_components[0]) / (2 * M_PI);
        if (u < 0.0)
          u += 1.0;

        double v = axVal / m_height;
        bestUV = Point<2>{u, v};
        isInside = false;
      }
    }
  }

  if (std::abs(da) > 1e-10) {
    double t = (m_height - oa) / da;

    if (t >= localRay.getMinDistance() && t <= localRay.getMaxDistance() &&
        t < bestT) {
      Point<3> p = localRay.at(t);

      double dx = p.m_components[(axisIdx + 1) % 3];
      double dy = p.m_components[(axisIdx + 2) % 3];
      if (dx * dx + dy * dy <= m_radius * m_radius) {
        bestT = t;
        bestPt = p;

        bestNormal = Vector<3>{0.0, 0.0, 0.0};
        bestNormal.m_components[axisIdx] = 1.0;

        double r = std::sqrt(dx * dx + dy * dy) / m_radius;
        double ang = std::atan2(dy, dx) / (2 * M_PI);
        if (ang < 0.0)
          ang += 1.0;
        bestUV = Point<2>{r * std::cos(ang * 2 * M_PI), r * std::sin(ang * 2 * M_PI)};
        isInside = da > 0.0;
      }
    }
  }

  if (bestT < std::numeric_limits<double>::infinity()) {
    Point<3> worldPt = getTransform().transformPoint(bestPt);
    Vector<3> worldN = getTransform().transformNormal(bestNormal);
    worldN /= worldN.length();
    double worldDist = (worldPt - ray.getOrigin()).length();

    return Core::Intersection(worldPt, worldN, getMaterial(), worldDist,
      isInside, bestUV);
  }
  return std::nullopt;
}

Core::BoundingBox Cone::getBoundingBox() const noexcept
{
  Point<3> baseCenter = m_position + m_axis * m_height;

  Point<3> min, max;
  if (m_axis.m_components[0] == 1.0) {
    min = Point<3>{m_position.m_components[0],
                   baseCenter.m_components[1] - m_radius,
                   baseCenter.m_components[2] - m_radius};
    max = Point<3>{baseCenter.m_components[0],
                   baseCenter.m_components[1] + m_radius,
                   baseCenter.m_components[2] + m_radius};
  } else if (m_axis.m_components[1] == 1.0) {
    min = Point<3>{baseCenter.m_components[0] - m_radius,
                   m_position.m_components[1],
                   baseCenter.m_components[2] - m_radius};
    max = Point<3>{baseCenter.m_components[0] + m_radius,
                   baseCenter.m_components[1],
                   baseCenter.m_components[2] + m_radius};
  } else {
    min = Point<3>{baseCenter.m_components[0] - m_radius,
                   baseCenter.m_components[1] - m_radius,
                   m_position.m_components[2]};
    max = Point<3>{baseCenter.m_components[0] + m_radius,
                   baseCenter.m_components[1] + m_radius,
                   baseCenter.m_components[2]};
  }
  return Core::BoundingBox(min, max);
}

}