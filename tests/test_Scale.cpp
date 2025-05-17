#include "../src/Core/BoundingBox.hpp"
#include "../src/Core/Ray.hpp"
#include "../src/Math/Point.hpp"
#include "../src/Math/Transform.hpp"
#include "../src/Math/Vector.hpp"
#include <cmath>
#include <criterion/criterion.h>

using Raytracer::Core::BoundingBox;
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

TestSuite(ScaleSuite);

Test(ScaleSuite, BasicScale) {
  Transform t = Transform::scale(2.0, 3.0, 4.0);
  Point<3> p(1.0, 1.0, 1.0);

  Point<3> transformed = t.transformPoint(p);
  assert_point_equal(transformed, Point<3>(2.0, 3.0, 4.0));

  Point<3> p2(-1.5, 2.5, 0.5);
  Point<3> expected2(-3.0, 7.5, 2.0);
  Point<3> transformed2 = t.transformPoint(p2);
  assert_point_equal(transformed2, expected2);
}

Test(ScaleSuite, UniformScale) {
  Transform t = Transform::scale(2.0, 2.0, 2.0);
  Point<3> p(1.5, -2.5, 3.0);

  Point<3> transformed = t.transformPoint(p);
  assert_point_equal(transformed, Point<3>(3.0, -5.0, 6.0));
}

Test(ScaleSuite, VectorScale) {
  Vector<3> scaleVec(2.0, 3.0, 4.0);
  Transform t = Transform::scale(scaleVec);
  Point<3> p(1.0, 1.0, 1.0);

  Point<3> transformed = t.transformPoint(p);
  assert_point_equal(transformed, Point<3>(2.0, 3.0, 4.0));
}

Test(ScaleSuite, InverseScale) {
  Transform t = Transform::scale(2.0, 3.0, 4.0);
  Transform inv = t.inverse();

  Point<3> p(2.0, 3.0, 4.0);
  Point<3> original = inv.transformPoint(p);
  assert_point_equal(original, Point<3>(1.0, 1.0, 1.0));

  Point<3> p2(1.5, -2.5, 3.0);
  Point<3> roundTrip = inv.transformPoint(t.transformPoint(p2));
  assert_point_equal(roundTrip, p2);
}

Test(ScaleSuite, VectorTransformation) {
  Transform t = Transform::scale(2.0, 3.0, 4.0);
  Vector<3> v(1.0, 1.0, 1.0);

  Vector<3> transformed = t.transformVector(v);
  assert_vector_equal(transformed, Vector<3>(2.0, 3.0, 4.0));

  Vector<3> v2(-1.5, 2.5, 0.5);
  Vector<3> expected2(-3.0, 7.5, 2.0);
  Vector<3> transformed2 = t.transformVector(v2);
  assert_vector_equal(transformed2, expected2);
}

Test(ScaleSuite, NormalTransformation) {
  Transform t = Transform::scale(2.0, 3.0, 4.0);

  Vector<3> nx(1.0, 0.0, 0.0);
  Vector<3> transformedNx = t.transformNormal(nx);

  assert_vector_equal(transformedNx.normalize(), Vector<3>(1.0, 0.0, 0.0));

  Vector<3> ny(0.0, 1.0, 0.0);
  Vector<3> transformedNy = t.transformNormal(ny);
  assert_vector_equal(transformedNy.normalize(), Vector<3>(0.0, 1.0, 0.0));

  Vector<3> nz(0.0, 0.0, 1.0);
  Vector<3> transformedNz = t.transformNormal(nz);
  assert_vector_equal(transformedNz.normalize(), Vector<3>(0.0, 0.0, 1.0));
}

Test(ScaleSuite, RayTransformation) {
  Transform t = Transform::scale(2.0, 3.0, 4.0);

  Point<3> origin(1.0, 1.0, 1.0);
  Vector<3> direction(1.0, 0.0, 0.0);
  Ray ray(origin, direction);

  Ray transformedRay = t.transformRay(ray);
  assert_point_equal(transformedRay.getOrigin(), Point<3>(2.0, 3.0, 4.0));

  Vector<3> expectedDirection(2.0, 0.0, 0.0);

  assert_vector_equal(transformedRay.getDirection(), expectedDirection);

  Ray inverseRay = t.inverseTransformRay(ray);
  assert_point_equal(inverseRay.getOrigin(), Point<3>(0.5, 1.0 / 3.0, 0.25));

  Vector<3> expectedInverseDirection(0.5, 0.0, 0.0);
  assert_vector_equal(inverseRay.getDirection(), expectedInverseDirection);
}

Test(ScaleSuite, NonPositiveScale) {
  Transform t = Transform::scale(-2.0, 3.0, -4.0);
  Point<3> p(1.0, 1.0, 1.0);

  Point<3> transformed = t.transformPoint(p);
  assert_point_equal(transformed, Point<3>(-2.0, 3.0, -4.0));

  Transform tzero = Transform::scale(0.0, 3.0, 4.0);
  Point<3> pzero = tzero.transformPoint(p);
  assert_point_equal(pzero, Point<3>(0.0, 3.0, 4.0));
}

Test(ScaleSuite, CombinedTransformations) {
  Transform scale = Transform::scale(2.0, 3.0, 4.0);
  Transform translate = Transform::translate(5.0, -2.0, 3.0);
  Transform combined = translate * scale;

  Point<3> p(1.0, 1.0, 1.0);
  Point<3> expected(7.0, 1.0, 7.0);

  Point<3> transformed = combined.transformPoint(p);
  assert_point_equal(transformed, expected);

  Transform rotate = Transform::rotateZ(M_PI / 2);
  Transform combined2 = rotate * scale;

  Point<3> expected2(-3.0, 2.0, 4.0);
  Point<3> transformed2 = combined2.transformPoint(p);
  assert_point_equal(transformed2, expected2);
}

Test(ScaleSuite, BoundingBoxTransformation) {
  Point<3> min(1.0, 2.0, 3.0);
  Point<3> max(4.0, 6.0, 8.0);
  BoundingBox box(min, max);

  Transform t = Transform::scale(2.0, 0.5, -1.0);

  BoundingBox transformedBox = t.transformBoundingBox(box);

  Point<3> expectedMin(2.0, 1.0, -8.0);
  Point<3> expectedMax(8.0, 3.0, -3.0);

  assert_point_equal(transformedBox.getMin(), expectedMin);
  assert_point_equal(transformedBox.getMax(), expectedMax);
}