#include "../src/Math/Matrix.hpp"
#include <criterion/criterion.h>

using Raytracer::Math::Matrix;
using Raytracer::Math::Matrix4;

static constexpr double EQ_APPROX = 1e-9;

Test(MatrixSuite, DefaultConstructor) {
  Matrix<3, 3> m;

  for (size_t i = 0; i < 3; i++) {
    for (size_t j = 0; j < 3; j++) {
      cr_assert_float_eq(m(i, j), 0.0, EQ_APPROX,
                         "Default constructor should initialize to zeros");
    }
  }
}

Test(MatrixSuite, InitializerList) {
  Matrix<2, 2> m = {{1.0, 2.0}, {3.0, 4.0}};

  cr_assert_float_eq(m(0, 0), 1.0, EQ_APPROX);
  cr_assert_float_eq(m(0, 1), 2.0, EQ_APPROX);
  cr_assert_float_eq(m(1, 0), 3.0, EQ_APPROX);
  cr_assert_float_eq(m(1, 1), 4.0, EQ_APPROX);
}

Test(MatrixSuite, Identity) {
  Matrix<3, 3> identity = Matrix<3, 3>::identity();

  for (size_t i = 0; i < 3; i++) {
    for (size_t j = 0; j < 3; j++) {
      if (i == j) {
        cr_assert_float_eq(identity(i, j), 1.0, EQ_APPROX,
                           "Diagonal elements should be 1.0");
      } else {
        cr_assert_float_eq(identity(i, j), 0.0, EQ_APPROX,
                           "Non-diagonal elements should be 0.0");
      }
    }
  }
}

Test(MatrixSuite, Multiply) {
  Matrix<2, 3> a = {{1.0, 2.0, 3.0}, {4.0, 5.0, 6.0}};

  Matrix<3, 2> b = {{7.0, 8.0}, {9.0, 10.0}, {11.0, 12.0}};

  Matrix<2, 2> result = a.multiply(b);

  cr_assert_float_eq(result(0, 0), 1 * 7 + 2 * 9 + 3 * 11, EQ_APPROX);
  cr_assert_float_eq(result(0, 1), 1 * 8 + 2 * 10 + 3 * 12, EQ_APPROX);
  cr_assert_float_eq(result(1, 0), 4 * 7 + 5 * 9 + 6 * 11, EQ_APPROX);
  cr_assert_float_eq(result(1, 1), 4 * 8 + 5 * 10 + 6 * 12, EQ_APPROX);
}

Test(MatrixSuite, Determinant) {
  Matrix<3, 3> m = {{1.0, 2.0, 3.0}, {4.0, 5.0, 6.0}, {7.0, 8.0, 9.0}};

  double det = m.determinant();
  cr_assert_float_eq(det, 0.0, EQ_APPROX);

  Matrix<3, 3> m2 = {{3.0, 0.0, 2.0}, {2.0, 0.0, -2.0}, {0.0, 1.0, 1.0}};

  double det2 = m2.determinant();
  cr_assert_float_eq(det2, 10.0, EQ_APPROX);
}