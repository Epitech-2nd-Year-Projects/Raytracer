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

Test(ScaleSuite, ScalePoint) {
  Transform t = Transform::scale(2.0, 3.0, 4.0);

  Point<3> p(1.0, 1.0, 1.0);
  Point<3> expected(2.0, 3.0, 4.0);

  Point<3> transformed = t.transformPoint(p);
  assert_point_equal(transformed, expected);

  Point<3> backTransformed = t.inverse().transformPoint(transformed);
  assert_point_equal(backTransformed, p);
}

Test(ScaleSuite, ScaleVector) {
  Transform t = Transform::scale(2.0, 3.0, 4.0);

  Vector<3> v(1.0, 1.0, 1.0);
  Vector<3> expected(2.0, 3.0, 4.0);

  Vector<3> transformed = t.transformVector(v);
  assert_vector_equal(transformed, expected);

  Vector<3> backTransformed = t.inverse().transformVector(transformed);
  assert_vector_equal(backTransformed, v);
}

Test(ScaleSuite, ScaleWithVector) {
  Vector<3> scaleVec(2.0, 3.0, 4.0);
  Transform t = Transform::scale(scaleVec);

  Point<3> p(1.0, 1.0, 1.0);
  Point<3> expected(2.0, 3.0, 4.0);

  Point<3> transformed = t.transformPoint(p);
  assert_point_equal(transformed, expected);
}

Test(ScaleSuite, CombineWithTranslation) {
  Transform scale = Transform::scale(2.0, 3.0, 4.0);
  Transform translation = Transform::translate(10.0, 20.0, 30.0);

  Transform combined = translation * scale;

  Point<3> p(1.0, 1.0, 1.0);
  Point<3> expected(12.0, 23.0, 34.0);

  Point<3> transformed = combined.transformPoint(p);
  assert_point_equal(transformed, expected);
}

Test(ScaleSuite, CombineWithRotation) {
  Transform scale = Transform::scale(2.0, 2.0, 2.0);
  Transform rotation = Transform::rotateX(M_PI / 2.0);

  Transform combined = rotation * scale;

  Point<3> p(1.0, 1.0, 1.0);
  Point<3> expected(2.0, -2.0, 2.0);

  Point<3> transformed = combined.transformPoint(p);
  assert_point_equal(transformed, expected, 1e-5);
}

Test(ScaleSuite, TransformRay) {
  Transform t = Transform::scale(2.0, 3.0, 4.0);

  Point<3> origin(1.0, 1.0, 1.0);
  Vector<3> direction(0.0, 0.0, 1.0);
  Ray ray(origin, direction);

  Ray transformed = t.transformRay(ray);
  assert_point_equal(transformed.getOrigin(), Point<3>(2.0, 3.0, 4.0));
  assert_vector_equal(transformed.getDirection(), Vector<3>(0.0, 0.0, 4.0));

  Ray inverseTransformed = t.inverseTransformRay(ray);
  assert_point_equal(inverseTransformed.getOrigin(), Point<3>(0.5, 1.0/3.0, 0.25));
  assert_vector_equal(inverseTransformed.getDirection(), Vector<3>(0.0, 0.0, 0.25));
}