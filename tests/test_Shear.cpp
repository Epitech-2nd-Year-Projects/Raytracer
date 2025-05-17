#include "../src/Core/Ray.hpp"
#include "../src/Math/Point.hpp"
#include "../src/Math/Transform.hpp"
#include "../src/Math/Vector.hpp"
#include <cmath>
#include <criterion/criterion.h>

using Raytracer::Core::Ray;
using Raytracer::Math::Point;
using Raytracer::Math::Transform;
using Raytracer::Math::Vector;

static constexpr double EQ_APPROX = 1e-6;

static void assert_point_equal(const Point<3> &a, const Point<3> &b,
                               double eps = EQ_APPROX) {
  for (int i = 0; i < 3; i++) {
    cr_assert_float_eq(a.m_components[i], b.m_components[i], eps,
                       "Component %d mismatch: expected %f, got %f", i,
                       b.m_components[i], a.m_components[i]);
  }
}

static void assert_vector_equal(const Vector<3> &a, const Vector<3> &b,
                                double eps = EQ_APPROX) {
  for (int i = 0; i < 3; i++) {
    cr_assert_float_eq(a.m_components[i], b.m_components[i], eps,
                       "Component %d mismatch: expected %f, got %f", i,
                       b.m_components[i], a.m_components[i]);
  }
}

TestSuite(ShearSuite);

Test(ShearSuite, BasicShear) {
  Transform txy = Transform::shear(1.0, 0.0, 0.0, 0.0, 0.0, 0.0);
  Point<3> p1(1.0, 2.0, 3.0);
  Point<3> expected1(3.0, 2.0, 3.0);
  assert_point_equal(txy.transformPoint(p1), expected1);

  Transform txz = Transform::shear(0.0, 1.0, 0.0, 0.0, 0.0, 0.0);
  Point<3> expected2(4.0, 2.0, 3.0);
  assert_point_equal(txz.transformPoint(p1), expected2);

  Transform tyx = Transform::shear(0.0, 0.0, 2.0, 0.0, 0.0, 0.0);
  Point<3> expected3(1.0, 4.0, 3.0);
  assert_point_equal(tyx.transformPoint(p1), expected3);

  Transform tyz = Transform::shear(0.0, 0.0, 0.0, 1.0, 0.0, 0.0);
  Point<3> expected4(1.0, 5.0, 3.0);
  assert_point_equal(tyz.transformPoint(p1), expected4);

  Transform tzx = Transform::shear(0.0, 0.0, 0.0, 0.0, 0.5, 0.0);
  Point<3> expected5(1.0, 2.0, 3.5);
  assert_point_equal(tzx.transformPoint(p1), expected5);

  Transform tzy = Transform::shear(0.0, 0.0, 0.0, 0.0, 0.0, 0.5);
  Point<3> expected6(1.0, 2.0, 4.0);
  assert_point_equal(tzy.transformPoint(p1), expected6);
}

Test(ShearSuite, CombinedShear) {
  Transform t = Transform::shear(1.0, 2.0, 0.0, 0.0, 0.0, 0.0);
  Point<3> p(1.0, 2.0, 3.0);
  Point<3> expected(9.0, 2.0, 3.0);
  assert_point_equal(t.transformPoint(p), expected);

  Transform t2 = Transform::shear(0.0, 0.0, 0.5, 0.5, 0.5, 0.5);
  Point<3> expected2(1.0, 4.0, 4.5);
  assert_point_equal(t2.transformPoint(p), expected2);
}

Test(ShearSuite, InverseShear) {
  Transform t = Transform::shear(1.0, 0.0, 0.0, 0.0, 0.0, 0.0);
  Transform inv = t.inverse();

  Point<3> p(1.0, 2.0, 3.0);
  Point<3> transformed = t.transformPoint(p);
  Point<3> original = inv.transformPoint(transformed);

  assert_point_equal(original, p);

  Transform t2 = Transform::shear(0.5, 0.25, 0.75, 0.3, 0.1, 0.2);
  Transform inv2 = t2.inverse();

  Point<3> p2(3.0, -2.0, 1.0);
  Point<3> transformed2 = t2.transformPoint(p2);
  Point<3> original2 = inv2.transformPoint(transformed2);

  assert_point_equal(original2, p2);
}

Test(ShearSuite, VectorShear) {
  Transform t = Transform::shear(1.0, 0.0, 0.0, 0.0, 0.0, 0.0);
  Vector<3> v(1.0, 2.0, 3.0);
  Vector<3> expected(3.0, 2.0, 3.0);

  assert_vector_equal(t.transformVector(v), expected);

  Transform t2 = Transform::shear(0.0, 0.5, 0.0, 0.0, 0.7, 0.0);
  Vector<3> expected2(2.5, 2.0, 3.7);
  assert_vector_equal(t2.transformVector(v), expected2);
}

Test(ShearSuite, RayShear) {
  Transform t = Transform::shear(1.0, 0.0, 0.0, 0.0, 0.0, 0.0);

  Point<3> origin(1.0, 2.0, 3.0);
  Vector<3> direction(0.0, 0.0, 1.0);
  Ray ray(origin, direction);

  Ray transformed = t.transformRay(ray);
  assert_point_equal(transformed.getOrigin(), Point<3>(3.0, 2.0, 3.0));
  assert_vector_equal(transformed.getDirection(), Vector<3>(0.0, 0.0, 1.0));

  Vector<3> direction2(1.0, 2.0, 3.0);
  Ray ray2(origin, direction2);

  Ray transformed2 = t.transformRay(ray2);
  assert_point_equal(transformed2.getOrigin(), Point<3>(3.0, 2.0, 3.0));
  assert_vector_equal(transformed2.getDirection(), Vector<3>(3.0, 2.0, 3.0));
}

Test(ShearSuite, ComposeTransformations) {
  Transform shear = Transform::shear(1.0, 0.0, 0.0, 0.0, 0.0, 0.0);
  Transform translate = Transform::translate(1.0, 2.0, 3.0);
  Transform scale = Transform::scale(2.0, 2.0, 2.0);

  Transform combined1 = translate * shear;
  Transform combined2 = shear * translate;

  Point<3> p(0.0, 1.0, 0.0);

  Point<3> expected1(2.0, 3.0, 3.0);
  assert_point_equal(combined1.transformPoint(p), expected1);

  Point<3> expected2(4.0, 3.0, 3.0);
  assert_point_equal(combined2.transformPoint(p), expected2);

  Transform combined3 = translate * shear * scale;

  Point<3> expected3(3.0, 4.0, 3.0);
  assert_point_equal(combined3.transformPoint(p), expected3);
}
