#include "../src/Core/Ray.hpp"
#include "../src/Math/Point.hpp"
#include "../src/Math/Vector.hpp"
#include "../src/Primitives/Cylinder.hpp"
#include <cmath>
#include <criterion/criterion.h>

using Raytracer::Core::Ray;
using Raytracer::Math::Point;
using Raytracer::Math::Vector;
using Raytracer::Primitives::Cylinder;

static constexpr double EQ_APPROX = 1e-9;

Test(CylinderSuite, DefaultConstructor) {
  Cylinder c;

  cr_assert_float_eq(c.getPosition().m_components[0], 0.0, EQ_APPROX);
  cr_assert_float_eq(c.getPosition().m_components[1], 0.0, EQ_APPROX);
  cr_assert_float_eq(c.getPosition().m_components[2], 0.0, EQ_APPROX);

  cr_assert_float_eq(c.getNormal().m_components[0], 0.0, EQ_APPROX);
  cr_assert_float_eq(c.getNormal().m_components[1], 0.0, EQ_APPROX);
  cr_assert_float_eq(c.getNormal().m_components[2], 0.0, EQ_APPROX);
}

Test(CylinderSuite, ParameterizedConstructor) {
  Cylinder c("Z", Point<3>(1.0, 2.0, 3.0), 2.5, 5.0);

  cr_assert_float_eq(c.getPosition().m_components[0], 1.0, EQ_APPROX);
  cr_assert_float_eq(c.getPosition().m_components[1], 2.0, EQ_APPROX);
  cr_assert_float_eq(c.getPosition().m_components[2], 3.0, EQ_APPROX);

  cr_assert_float_eq(c.getNormal().m_components[0], 0.0, EQ_APPROX);
  cr_assert_float_eq(c.getNormal().m_components[1], 0.0, EQ_APPROX);
  cr_assert_float_eq(c.getNormal().m_components[2], 1.0, EQ_APPROX);
}

Test(CylinderSuite, SetterMethod) {
  Cylinder c;
  c.setAxisPositionRadiusAndHeight("X", Point<3>(3.0, 4.0, 5.0), 2.0, 7.0);

  cr_assert_float_eq(c.getPosition().m_components[0], 3.0, EQ_APPROX);
  cr_assert_float_eq(c.getPosition().m_components[1], 4.0, EQ_APPROX);
  cr_assert_float_eq(c.getPosition().m_components[2], 5.0, EQ_APPROX);

  cr_assert_float_eq(c.getNormal().m_components[0], 1.0, EQ_APPROX);
  cr_assert_float_eq(c.getNormal().m_components[1], 0.0, EQ_APPROX);
  cr_assert_float_eq(c.getNormal().m_components[2], 0.0, EQ_APPROX);
}

Test(CylinderSuite, DifferentAxisSettings) {
  Cylinder cX("X", Point<3>(0.0, 0.0, 0.0), 1.0, 2.0);
  cr_assert_float_eq(cX.getNormal().m_components[0], 1.0, EQ_APPROX);
  cr_assert_float_eq(cX.getNormal().m_components[1], 0.0, EQ_APPROX);
  cr_assert_float_eq(cX.getNormal().m_components[2], 0.0, EQ_APPROX);

  Cylinder cY("Y", Point<3>(0.0, 0.0, 0.0), 1.0, 2.0);
  cr_assert_float_eq(cY.getNormal().m_components[0], 0.0, EQ_APPROX);
  cr_assert_float_eq(cY.getNormal().m_components[1], 1.0, EQ_APPROX);
  cr_assert_float_eq(cY.getNormal().m_components[2], 0.0, EQ_APPROX);

  Cylinder cZ("Z", Point<3>(0.0, 0.0, 0.0), 1.0, 2.0);
  cr_assert_float_eq(cZ.getNormal().m_components[0], 0.0, EQ_APPROX);
  cr_assert_float_eq(cZ.getNormal().m_components[1], 0.0, EQ_APPROX);
  cr_assert_float_eq(cZ.getNormal().m_components[2], 1.0, EQ_APPROX);
}

Test(CylinderSuite, BoundingBox) {
  Cylinder cZ("Z", Point<3>(0.0, 0.0, 0.0), 2.0, 4.0);
  auto bboxZ = cZ.getBoundingBox();

  cr_assert_float_eq(bboxZ.getMin().m_components[0], -2.0, EQ_APPROX);
  cr_assert_float_eq(bboxZ.getMin().m_components[1], -2.0, EQ_APPROX);
  cr_assert_float_eq(bboxZ.getMin().m_components[2], -2.0, EQ_APPROX);

  cr_assert_float_eq(bboxZ.getMax().m_components[0], 2.0, EQ_APPROX);
  cr_assert_float_eq(bboxZ.getMax().m_components[1], 2.0, EQ_APPROX);
  cr_assert_float_eq(bboxZ.getMax().m_components[2], 2.0, EQ_APPROX);

  Cylinder cY("Y", Point<3>(1.0, 2.0, 3.0), 2.0, 4.0);
  auto bboxY = cY.getBoundingBox();

  cr_assert_float_eq(bboxY.getMin().m_components[0], -1.0, EQ_APPROX);
  cr_assert_float_eq(bboxY.getMin().m_components[1], 0.0, EQ_APPROX);
  cr_assert_float_eq(bboxY.getMin().m_components[2], 1.0, EQ_APPROX);

  cr_assert_float_eq(bboxY.getMax().m_components[0], 3.0, EQ_APPROX);
  cr_assert_float_eq(bboxY.getMax().m_components[1], 4.0, EQ_APPROX);
  cr_assert_float_eq(bboxY.getMax().m_components[2], 5.0, EQ_APPROX);
}

Test(CylinderSuite, RayIntersectionSide) {
  Cylinder c("Z", Point<3>(0.0, 0.0, 0.0), 1.0, 2.0);

  Ray r1(Point<3>(5.0, 0.0, 0.0), Vector<3>(-1.0, 0.0, 0.0));
  auto i1 = c.intersect(r1);

  cr_assert(i1.has_value(), "Ray should intersect cylinder side");
  cr_assert_float_eq(i1->getDistance(), 4.0, EQ_APPROX);
  cr_assert_float_eq(i1->getPoint().m_components[0], 1.0, EQ_APPROX);
  cr_assert_float_eq(i1->getPoint().m_components[1], 0.0, EQ_APPROX);
  cr_assert_float_eq(i1->getPoint().m_components[2], 0.0, EQ_APPROX);

  cr_assert_float_eq(i1->getNormal().m_components[0], 1.0, EQ_APPROX);
  cr_assert_float_eq(i1->getNormal().m_components[1], 0.0, EQ_APPROX);
  cr_assert_float_eq(i1->getNormal().m_components[2], 0.0, EQ_APPROX);

  cr_assert(!i1->getIsInside(), "Ray should hit from outside");
}

Test(CylinderSuite, RayIntersectionCap) {
  Cylinder c("Z", Point<3>(0.0, 0.0, 0.0), 1.0, 2.0);

  Ray r1(Point<3>(0.0, 0.0, 5.0), Vector<3>(0.0, 0.0, -1.0));
  auto i1 = c.intersect(r1);

  cr_assert(i1.has_value(), "Ray should intersect cylinder top cap");
  cr_assert_float_eq(i1->getDistance(), 4.0, EQ_APPROX);
  cr_assert_float_eq(i1->getPoint().m_components[0], 0.0, EQ_APPROX);
  cr_assert_float_eq(i1->getPoint().m_components[1], 0.0, EQ_APPROX);
  cr_assert_float_eq(i1->getPoint().m_components[2], 1.0, EQ_APPROX);

  cr_assert_float_eq(i1->getNormal().m_components[0], 0.0, EQ_APPROX);
  cr_assert_float_eq(i1->getNormal().m_components[1], 0.0, EQ_APPROX);
  cr_assert_float_eq(i1->getNormal().m_components[2], 1.0, EQ_APPROX);
}

Test(CylinderSuite, RayMiss) {
  Cylinder c("Z", Point<3>(0.0, 0.0, 0.0), 1.0, 2.0);

  Ray r1(Point<3>(5.0, 5.0, 0.0), Vector<3>(-1.0, 0.0, 0.0));
  auto i1 = c.intersect(r1);

  cr_assert(!i1.has_value(), "Ray should miss cylinder");

  Ray r2(Point<3>(0.0, 0.0, 5.0), Vector<3>(1.0, 0.0, 0.0));
  auto i2 = c.intersect(r2);

  cr_assert(!i2.has_value(), "Ray outside height range should miss cylinder");
}

Test(CylinderSuite, RayIntersectionFromInside) {
  Cylinder c("Z", Point<3>(0.0, 0.0, 0.0), 1.0, 2.0);

  Ray r1(Point<3>(0.0, 0.0, 0.0), Vector<3>(1.0, 0.0, 0.0));
  auto i1 = c.intersect(r1);

  cr_assert(i1.has_value(), "Ray from inside should intersect cylinder");
  cr_assert_float_eq(i1->getDistance(), 1.0, EQ_APPROX);
  cr_assert(i1->getIsInside(), "Ray should be marked as inside");
}

Test(CylinderSuite, RayDistanceLimits) {
  Cylinder c("Z", Point<3>(0.0, 0.0, 0.0), 1.0, 2.0);

  Ray r1(Point<3>(5.0, 0.0, 0.0), Vector<3>(-1.0, 0.0, 0.0), 0.0, 3.0);
  auto i1 = c.intersect(r1);

  cr_assert(!i1.has_value(),
            "Ray with max distance too short should not intersect");
}
