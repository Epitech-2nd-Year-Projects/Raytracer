#include "../src/Math/Vector.hpp"
#include <cmath>
#include <criterion/criterion.h>

using Raytracer::Math::Vector;

static constexpr double EQ_APPROX = 1e-9;

template <std::size_t N>
void assert_vector_eq(const Vector<N> &v, const std::array<double, N> &ref,
                      double eps = EQ_APPROX) {
  for (std::size_t i = 0; i < N; ++i) {
    cr_assert_float_eq(v.m_components[i], ref[i], eps,
                       "component[%zu]: expected %g but got %g", i, ref[i],
                       v.m_components[i]);
  }
}

Test(VectorSuite, DefaultConstructor) {
  Vector<3> v;

  assert_vector_eq(v, {0.0, 0.0, 0.0});
  cr_assert_float_eq(v.length(), 0.0, EQ_APPROX, "Expected length 0, got %g",
                     v.length());
}

Test(VectorSuite, ParameterizedConstructorAndLength) {
  double expected_len = std::sqrt(1.0 + 2.25 + 4.0);
  Vector<3> v(1.0, 1.5, 2.0);

  assert_vector_eq(v, {1.0, 1.5, 2.0});
  cr_assert_float_eq(v.length(), expected_len, EQ_APPROX,
                     "Expected length %g, got %g", expected_len, v.length());
}

Test(VectorSuite, AdditionAndSubtraction) {
  Vector<3> a(1.0, -2.0, 3.5);
  Vector<3> b(0.5, 2.0, -1.5);
  Vector<3> sum = a + b;
  Vector<3> diff = a - b;

  assert_vector_eq(sum, {1.5, 0.0, 2.0});
  assert_vector_eq(diff, {0.5, -4.0, 5.0});

  Vector<3> c = a;
  c += b;
  assert_vector_eq(c, {1.5, 0.0, 2.0});

  c -= b;
  assert_vector_eq(c, {1.0, -2.0, 3.5});
}

Test(VectorSuite, ComponentwiseMulDiv) {
  Vector<4> a(2.0, 4.0, -1.0, 0.5);
  Vector<4> b(1.0, 0.5, -2.0, 4.0);
  Vector<4> pmul = a * b;
  Vector<4> pdiv = a / b;

  assert_vector_eq(pmul, {2.0, 2.0, 2.0, 2.0});
  assert_vector_eq(pdiv, {2.0, 8.0, 0.5, 0.125});

  Vector<4> c = a;
  c *= b;
  assert_vector_eq(c, {2.0, 2.0, 2.0, 2.0});

  c = a;
  c /= b;
  assert_vector_eq(c, {2.0, 8.0, 0.5, 0.125});
}

Test(VectorSuite, ScalarMulDiv) {
  Vector<2> a(3.0, -4.0);
  double s = 2.5;
  Vector<2> mul = a * s;
  Vector<2> div = a / s;

  assert_vector_eq(mul, {7.5, -10.0});
  assert_vector_eq(div, {1.2, -1.6});

  Vector<2> b = a;
  b *= s;
  assert_vector_eq(b, {7.5, -10.0});

  b = a;
  b /= s;
  assert_vector_eq(b, {1.2, -1.6});
}

Test(VectorSuite, DotProduct) {
  Vector<5> a(1.0, 2.0, 3.0, 4.0, 5.0);
  Vector<5> b(5.0, 4.0, 3.0, 2.0, 1.0);
  double expected = 1 * 5 + 2 * 4 + 3 * 3 + 4 * 2 + 5 * 1;

  cr_assert_float_eq(a.dot(b), expected, EQ_APPROX, "Expected dot %g, got %g",
                     expected, a.dot(b));
}
