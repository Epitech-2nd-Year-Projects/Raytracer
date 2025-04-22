#include "../src/Math/Point.hpp"
#include "../src/Math/Vector.hpp"
#include <array>
#include <criterion/criterion.h>

using Raytracer::Math::Point;
using Raytracer::Math::Vector;

static constexpr double EQ_APPROX = 1e-9;

template <std::size_t N>
void assert_point_eq(const Point<N> &p, const std::array<double, N> &ref,
                     double eps = EQ_APPROX) {
  for (std::size_t i = 0; i < N; ++i) {
    cr_assert_float_eq(p.m_components[i], ref[i], eps,
                       "component[%zu]: expected %g but got %g", i, ref[i],
                       p.m_components[i]);
  }
}

Test(PointSuite, DefaultConstructor) {
  Point<3> p;
  assert_point_eq(p, {0.0, 0.0, 0.0});
}

Test(PointSuite, ParameterizedConstructor) {
  Point<4> p(1.0, -2.5, 3.75, 0.0);
  assert_point_eq(p, {1.0, -2.5, 3.75, 0.0});
}

Test(PointSuite, AddVectorAndSubtractVector) {
  Point<3> p(0.5, 1.0, -1.5);
  Vector<3> v(2.0, -0.5, 4.0);

  Point<3> sum = p + v;
  assert_point_eq(sum, {2.5, 0.5, 2.5});

  assert_point_eq(p, {0.5, 1.0, -1.5});

  Point<3> q = p;
  q += v;
  assert_point_eq(q, {2.5, 0.5, 2.5});

  q -= v;
  assert_point_eq(q, {0.5, 1.0, -1.5});

  Point<3> diff = p - v;
  assert_point_eq(diff, {-1.5, 1.5, -5.5});
}

Test(PointSuite, MixedDimensions) {
  Point<2> p2(10.0, -5.0);
  Vector<2> v2(-2.0, 3.0);
  Point<2> r2 = p2 + v2;
  assert_point_eq(r2, {8.0, -2.0});

  r2 -= v2;
  assert_point_eq(r2, {10.0, -5.0});

  Point<1> p1(7.0);
  Vector<1> v1(2.5);
  Point<1> r1 = p1 - v1;
  assert_point_eq(r1, {4.5});

  r1 += v1;
  assert_point_eq(r1, {7.0});
}
