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

Test(RotationSuite, RotateX90Degrees) {
  Transform t = Transform::rotateX(M_PI / 2);

  Point<3> p(0.0, 1.0, 0.0);
  Point<3> expected(0.0, 0.0, 1.0);

  Point<3> transformed = t.transformPoint(p);
  assert_point_equal(transformed, expected);

  Point<3> backTransformed = t.inverse().transformPoint(transformed);
  assert_point_equal(backTransformed, p);
}

Test(RotationSuite, RotateY90Degrees) {
  Transform t = Transform::rotateY(M_PI / 2);

  Point<3> p(0.0, 0.0, 1.0);
  Point<3> expected(1.0, 0.0, 0.0);

  Point<3> transformed = t.transformPoint(p);
  assert_point_equal(transformed, expected);

  Point<3> backTransformed = t.inverse().transformPoint(transformed);
  assert_point_equal(backTransformed, p);
}

Test(RotationSuite, RotateZ90Degrees) {
  Transform t = Transform::rotateZ(M_PI / 2);

  Point<3> p(1.0, 0.0, 0.0);
  Point<3> expected(0.0, 1.0, 0.0);

  Point<3> transformed = t.transformPoint(p);
  assert_point_equal(transformed, expected);

  Point<3> backTransformed = t.inverse().transformPoint(transformed);
  assert_point_equal(backTransformed, p);
}

Test(RotationSuite, RotateArbitraryPoint) {
  Point<3> p(1.0, 2.0, 3.0);

  Transform tx = Transform::rotateX(M_PI / 2);
  Transform ty = Transform::rotateY(M_PI / 2);
  Transform tz = Transform::rotateZ(M_PI / 2);

  Point<3> p1 = tz.transformPoint(p);
  Point<3> p2 = ty.transformPoint(p1);
  Point<3> p3 = tx.transformPoint(p2);

  Transform combined = tx * ty * tz;
  Point<3> combinedResult = combined.transformPoint(p);

  assert_point_equal(p3, combinedResult);

  Point<3> backTransformed = combined.inverse().transformPoint(combinedResult);
  assert_point_equal(backTransformed, p);
}

Test(RotationSuite, VectorRotation) {
  Vector<3> v(1.0, 0.0, 0.0);

  Transform t = Transform::rotateZ(M_PI / 2);
  Vector<3> transformed = t.transformVector(v);
  Vector<3> expected(0.0, 1.0, 0.0);

  assert_vector_equal(transformed, expected);
}

Test(RotationSuite, RayRotation) {
  Point<3> origin(1.0, 0.0, 0.0);
  Vector<3> direction(0.0, 1.0, 0.0);
  Ray ray(origin, direction);

  Transform t = Transform::rotateZ(M_PI / 2);
  Ray transformed = t.transformRay(ray);

  assert_point_equal(transformed.getOrigin(), Point<3>(0.0, 1.0, 0.0));

  assert_vector_equal(transformed.getDirection(), Vector<3>(-1.0, 0.0, 0.0));
}