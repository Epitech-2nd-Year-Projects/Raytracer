#include "../src/Core/Ray.hpp"
#include "../src/Math/Transform.hpp"
#include <criterion/criterion.h>

using Raytracer::Core::Ray;
using Raytracer::Math::Point;
using Raytracer::Math::Transform;
using Raytracer::Math::Vector;

static constexpr double EQ_APPROX = 1e-9;

void assert_point_equal(const Point<3> &a, const Point<3> &b) {
  for (int i = 0; i < 3; i++) {
    cr_assert_float_eq(a.m_components[i], b.m_components[i], EQ_APPROX,
                       "Component %d mismatch: expected %f, got %f", i,
                       b.m_components[i], a.m_components[i]);
  }
}

void assert_vector_equal(const Vector<3> &a, const Vector<3> &b) {
  for (int i = 0; i < 3; i++) {
    cr_assert_float_eq(a.m_components[i], b.m_components[i], EQ_APPROX,
                       "Component %d mismatch: expected %f, got %f", i,
                       b.m_components[i], a.m_components[i]);
  }
}

Test(TransformSuite, DefaultConstructor) {
  Transform t;
  Point<3> p(1.0, 2.0, 3.0);

  Point<3> transformed = t.transformPoint(p);
  assert_point_equal(transformed, p);
}

Test(TransformSuite, Translation) {
  Transform t = Transform::translate(5.0, -3.0, 2.0);
  Point<3> p(1.0, 2.0, 3.0);

  Point<3> transformed = t.transformPoint(p);
  assert_point_equal(transformed, Point<3>(6.0, -1.0, 5.0));

  Vector<3> v(1.0, 2.0, 3.0);
  Vector<3> transformedV = t.transformVector(v);
  assert_vector_equal(transformedV, v);
}

Test(TransformSuite, Inverse) {
  Transform t = Transform::translate(5.0, -3.0, 2.0);
  Transform inv = t.inverse();

  Point<3> p(1.0, 2.0, 3.0);

  Point<3> transformed = inv.transformPoint(t.transformPoint(p));
  assert_point_equal(transformed, p);
}

Test(TransformSuite, Composition) {
  Transform t1 = Transform::translate(5.0, 0.0, 0.0);
  Transform t2 = Transform::translate(0.0, 3.0, 0.0);

  Transform combined = t1 * t2;

  Point<3> p(1.0, 2.0, 3.0);
  Point<3> expected = Point<3>(6.0, 5.0, 3.0);

  Point<3> transformed = combined.transformPoint(p);
  assert_point_equal(transformed, expected);
}

Test(TransformSuite, RayTransformation) {
  Transform t = Transform::translate(0.0, 0.0, 5.0);

  Point<3> origin(0.0, 0.0, 0.0);
  Vector<3> direction(0.0, 0.0, 1.0);
  Ray ray(origin, direction);

  Ray transformedRay = t.transformRay(ray);
  assert_point_equal(transformedRay.getOrigin(), Point<3>(0.0, 0.0, 5.0));
  assert_vector_equal(transformedRay.getDirection(), direction);

  Ray inverseRay = t.inverseTransformRay(ray);
  assert_point_equal(inverseRay.getOrigin(), Point<3>(0.0, 0.0, -5.0));
  assert_vector_equal(inverseRay.getDirection(), direction);
}