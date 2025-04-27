#include "../src/Core/Ray.hpp"
#include "../src/Math/Point.hpp"
#include "../src/Math/Vector.hpp"
#include <criterion/criterion.h>

using Raytracer::Core::Ray;
using Raytracer::Math::Point;
using Raytracer::Math::Vector;

static constexpr double EQ_APPROX = 1e-9;

void assert_point_eq(const Point<3> &p1, const Point<3> &p2,
                     double eps = EQ_APPROX) {
  for (std::size_t i = 0; i < 3; ++i) {
    cr_assert_float_eq(p1.m_components[i], p2.m_components[i], eps,
                       "component[%zu]: expected %g but got %g", i,
                       p2.m_components[i], p1.m_components[i]);
  }
}

void assert_vector_eq(const Vector<3> &v1, const Vector<3> &v2,
                      double eps = EQ_APPROX) {
  for (std::size_t i = 0; i < 3; ++i) {
    cr_assert_float_eq(v1.m_components[i], v2.m_components[i], eps,
                       "component[%zu]: expected %g but got %g", i,
                       v2.m_components[i], v1.m_components[i]);
  }
}

Test(RaySuite, DefaultConstructor) {
  Ray ray;

  Point<3> expectedOrigin(0.0, 0.0, 0.0);
  assert_point_eq(ray.getOrigin(), expectedOrigin);

  Vector<3> expectedDirection(0.0, 0.0, 0.0);
  assert_vector_eq(ray.getDirection(), expectedDirection);
}

Test(RaySuite, ParameterizedConstructor) {
  Point<3> origin(1.0, 2.0, 3.0);
  Vector<3> direction(0.0, 1.0, 0.0);

  Ray ray(origin, direction);

  assert_point_eq(ray.getOrigin(), origin);
  assert_vector_eq(ray.getDirection(), direction);
}

Test(RaySuite, FullParameterizedConstructor) {
  Point<3> origin(1.0, 2.0, 3.0);
  Vector<3> direction(0.0, 1.0, 0.0);
  double minDist = 0.1;
  double maxDist = 50.0;

  Ray ray(origin, direction, minDist, maxDist);

  assert_point_eq(ray.getOrigin(), origin);
  assert_vector_eq(ray.getDirection(), direction);
}

Test(RaySuite, PointAtParameter) {
  Point<3> origin(1.0, 2.0, 3.0);
  Vector<3> direction(2.0, 0.0, -1.0);

  Ray ray(origin, direction);

  Point<3> pointAtZero = ray.at(0.0);
  assert_point_eq(pointAtZero, origin);

  Point<3> pointAtTwo = ray.at(2.0);
  Point<3> expectedPoint(5.0, 2.0, 1.0);
  assert_point_eq(pointAtTwo, expectedPoint);

  Point<3> pointAtNegative = ray.at(-1.0);
  Point<3> expectedNegative(-1.0, 2.0, 4.0);
  assert_point_eq(pointAtNegative, expectedNegative);
}

Test(RaySuite, NormalizeDirection) {
  Point<3> origin(1.0, 0.0, 0.0);
  Vector<3> direction(3.0, 4.0, 0.0);

  Ray ray(origin, direction);
  ray.normalizeDirection();

  Vector<3> expectedNormalized(0.6, 0.8, 0.0);
  assert_vector_eq(ray.getDirection(), expectedNormalized);

  cr_assert_float_eq(ray.getDirection().length(), 1.0, EQ_APPROX);
}
