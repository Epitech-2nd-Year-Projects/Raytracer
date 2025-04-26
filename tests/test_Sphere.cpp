#include "../src/Core/Ray.hpp"
#include "../src/Math/Point.hpp"
#include "../src/Math/Vector.hpp"
#include "../src/Primitives/Sphere.hpp"
#include <criterion/criterion.h>

using Raytracer::Core::Ray;
using Raytracer::Math::Point;
using Raytracer::Math::Vector;
using Raytracer::Primitives::Sphere;

static constexpr double EQ_APPROX = 1e-9;

Test(SphereSuite, DefaultConstructor) {
  Sphere s;
  cr_assert_float_eq(s.getRadius(), 1.0, EQ_APPROX);
  cr_assert_float_eq(s.getCenter().m_components[0], 0.0, EQ_APPROX);
  cr_assert_float_eq(s.getCenter().m_components[1], 0.0, EQ_APPROX);
  cr_assert_float_eq(s.getCenter().m_components[2], 0.0, EQ_APPROX);
}

Test(SphereSuite, ParameterizedConstructor) {
  Sphere s(Point<3>(1.0, 2.0, 3.0), 5.0);
  cr_assert_float_eq(s.getRadius(), 5.0, EQ_APPROX);
  cr_assert_float_eq(s.getCenter().m_components[0], 1.0, EQ_APPROX);
  cr_assert_float_eq(s.getCenter().m_components[1], 2.0, EQ_APPROX);
  cr_assert_float_eq(s.getCenter().m_components[2], 3.0, EQ_APPROX);
}

Test(SphereSuite, Setters) {
  Sphere s;
  s.setRadius(2.5);
  s.setCenter(Point<3>(3.0, 4.0, 5.0));

  cr_assert_float_eq(s.getRadius(), 2.5, EQ_APPROX);
  cr_assert_float_eq(s.getCenter().m_components[0], 3.0, EQ_APPROX);
  cr_assert_float_eq(s.getCenter().m_components[1], 4.0, EQ_APPROX);
  cr_assert_float_eq(s.getCenter().m_components[2], 5.0, EQ_APPROX);
}

Test(SphereSuite, BoundingBox) {
  Sphere s(Point<3>(1.0, 2.0, 3.0), 2.0);
  auto bbox = s.getBoundingBox();

  cr_assert_float_eq(bbox.getMin().m_components[0], -1.0, EQ_APPROX);
  cr_assert_float_eq(bbox.getMin().m_components[1], 0.0, EQ_APPROX);
  cr_assert_float_eq(bbox.getMin().m_components[2], 1.0, EQ_APPROX);

  cr_assert_float_eq(bbox.getMax().m_components[0], 3.0, EQ_APPROX);
  cr_assert_float_eq(bbox.getMax().m_components[1], 4.0, EQ_APPROX);
  cr_assert_float_eq(bbox.getMax().m_components[2], 5.0, EQ_APPROX);
}

Test(SphereSuite, RayIntersection) {
  Sphere s(Point<3>(0.0, 0.0, 0.0), 1.0);

  Ray r1(Point<3>(0.0, 0.0, -5.0), Vector<3>(0.0, 0.0, 1.0));
  auto i1 = s.intersect(r1);
  cr_assert(i1.has_value(), "Ray should intersect sphere");
  cr_assert_float_eq(i1->getDistance(), 4.0, EQ_APPROX);
  cr_assert_float_eq(i1->getPoint().m_components[0], 0.0, EQ_APPROX);
  cr_assert_float_eq(i1->getPoint().m_components[1], 0.0, EQ_APPROX);
  cr_assert_float_eq(i1->getPoint().m_components[2], -1.0, EQ_APPROX);
  cr_assert_float_eq(i1->getNormal().m_components[0], 0.0, EQ_APPROX);
  cr_assert_float_eq(i1->getNormal().m_components[1], 0.0, EQ_APPROX);
  cr_assert_float_eq(i1->getNormal().m_components[2], -1.0, EQ_APPROX);
  cr_assert(!i1->getIsInside(), "Ray should hit from outside");

  Ray r2(Point<3>(0.0, 0.0, -5.0), Vector<3>(0.0, 2.0, 0.0));
  auto i2 = s.intersect(r2);
  cr_assert(!i2.has_value(), "Ray should miss sphere");

  Ray r3(Point<3>(0.0, 0.0, 0.0), Vector<3>(0.0, 0.0, 1.0));
  auto i3 = s.intersect(r3);
  cr_assert(i3.has_value(), "Ray should intersect sphere from inside");
  cr_assert_float_eq(i3->getDistance(), 1.0, EQ_APPROX);
  cr_assert(i3->getIsInside(), "Ray should be marked as inside");
  cr_assert_float_eq(i3->getNormal().m_components[0], 0.0, EQ_APPROX);
  cr_assert_float_eq(i3->getNormal().m_components[1], 0.0, EQ_APPROX);
  cr_assert_float_eq(i3->getNormal().m_components[2], -1.0, EQ_APPROX);

  Ray r4(Point<3>(0.0, 0.0, -5.0), Vector<3>(0.0, 0.0, 1.0), 0.0, 3.0);
  auto i4 = s.intersect(r4);
  cr_assert(!i4.has_value(),
            "Ray with max distance too short should not intersect");
}

Test(SphereSuite, TransformedSphere) {
  Sphere s(Point<3>(1.0, 2.0, 3.0), 2.0);

  Ray r(Point<3>(1.0, 2.0, -5.0), Vector<3>(0.0, 0.0, 1.0));
  auto i = s.intersect(r);

  cr_assert(i.has_value(), "Ray should intersect transformed sphere");
  cr_assert_float_eq(i->getDistance(), 6.0, EQ_APPROX);
}