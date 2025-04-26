#include "../src/Math/Point.hpp"
#include "../src/Math/Rectangle.hpp"
#include "../src/Math/Vector.hpp"
#include "../src/Utility/Clamped.hpp"
#include <array>
#include <cmath>
#include <criterion/criterion.h>

using Raytracer::Math::Point;
using Raytracer::Math::Rectangle;
using Raytracer::Math::Rectangle3D;
using Raytracer::Math::Vector;
using Raytracer::Utility::Clamped;

static constexpr double EQ_APPROX = 1e-9;

template <std::size_t N>
void assert_vector_eq(const Vector<N> &v, const std::array<double, N> &ref,
                      double eps = EQ_APPROX) {
  for (std::size_t i = 0; i < N; ++i) {
    cr_assert_float_eq(v.m_components[i], ref[i], eps,
                       "Vector component[%zu]: expected %g but got %g", i,
                       ref[i], v.m_components[i]);
  }
}

template <std::size_t N>
void assert_point_eq(const Point<N> &p, const std::array<double, N> &ref,
                     double eps = EQ_APPROX) {
  for (std::size_t i = 0; i < N; ++i) {
    cr_assert_float_eq(p.m_components[i], ref[i], eps,
                       "Point component[%zu]: expected %g but got %g", i,
                       ref[i], p.m_components[i]);
  }
}

Test(RectangleSuite, DefaultConstructor) {
  Rectangle<3> rectangle;

  assert_point_eq(rectangle.getOrigin(), {0.0, 0.0, 0.0});
  assert_vector_eq(rectangle.getBottomSide(), {0.0, 0.0, 0.0});
  assert_vector_eq(rectangle.getLeftSide(), {0.0, 0.0, 0.0});
}

Test(RectangleSuite, ParameterizedConstructorAndGetters) {
  Point<3> origin(1.0, 2.0, 3.0);
  Vector<3> bottom(3.0, 0.0, 0.0);
  Vector<3> left(1.0, 0.0, 0.0);
  Rectangle<3> rectangle(origin, bottom, left);

  assert_point_eq(rectangle.getOrigin(), {1.0, 2.0, 3.0});
  assert_vector_eq(rectangle.getBottomSide(), {3.0, 0.0, 0.0});
  assert_vector_eq(rectangle.getLeftSide(), {1.0, 0.0, 0.0});
}

Test(RectangleSuite, Setters) {
  Rectangle<3> rectangle;

  Point<3> newOrigin(-1.0, -1.0, -1.0);
  Vector<3> newBottom(1.0, 0.0, 0.0);
  Vector<3> newLeft(0.0, 1.0, 0.0);

  rectangle.setOrigin(newOrigin);
  rectangle.setBottomSide(newBottom);
  rectangle.setLeftSide(newLeft);

  assert_point_eq(rectangle.getOrigin(), {-1.0, -1.0, -1.0});
  assert_vector_eq(rectangle.getBottomSide(), {1.0, 0.0, 0.0});
  assert_vector_eq(rectangle.getLeftSide(), {0.0, 1.0, 0.0});
}

Test(Rectangle3DSuite, DefaultConstructor) {
  Rectangle3D rectangle;

  assert_point_eq(rectangle.getOrigin(), {0.0, 0.0, 0.0});
  assert_vector_eq(rectangle.getBottomSide(), {0.0, 0.0, 0.0});
  assert_vector_eq(rectangle.getLeftSide(), {0.0, 0.0, 0.0});
}

Test(Rectangle3DSuite, ParameterizedConstructor) {
  Point<3> origin(1.0, 2.0, 3.0);
  Vector<3> bottom(4.0, 0.0, 0.0);
  Vector<3> left(0.0, 5.0, 0.0);
  Rectangle3D rectangle(origin, bottom, left);

  assert_point_eq(rectangle.getOrigin(), {1.0, 2.0, 3.0});
  assert_vector_eq(rectangle.getBottomSide(), {4.0, 0.0, 0.0});
  assert_vector_eq(rectangle.getLeftSide(), {0.0, 5.0, 0.0});
}

Test(Rectangle3DSuite, PointAt) {
  Point<3> origin(1.0, 1.0, 1.0);
  Vector<3> bottom(2.0, 0.0, 0.0);
  Vector<3> left(0.0, 3.0, 0.0);
  Rectangle3D rectangle(origin, bottom, left);

  assert_point_eq(rectangle.pointAt(Clamped<double, 0.0, 1.0>(0.0),
                                    Clamped<double, 0.0, 1.0>(0.0)),
                  {1.0, 1.0, 1.0});
  assert_point_eq(rectangle.pointAt(Clamped<double, 0.0, 1.0>(1.0),
                                    Clamped<double, 0.0, 1.0>(0.0)),
                  {3.0, 1.0, 1.0});
  assert_point_eq(rectangle.pointAt(Clamped<double, 0.0, 1.0>(0.0),
                                    Clamped<double, 0.0, 1.0>(1.0)),
                  {1.0, 4.0, 1.0});
  assert_point_eq(rectangle.pointAt(Clamped<double, 0.0, 1.0>(1.0),
                                    Clamped<double, 0.0, 1.0>(1.0)),
                  {3.0, 4.0, 1.0});
  assert_point_eq(rectangle.pointAt(Clamped<double, 0.0, 1.0>(0.5),
                                    Clamped<double, 0.0, 1.0>(0.5)),
                  {2.0, 2.5, 1.0});
}

Test(Rectangle3DSuite, GetNormal) {
  Point<3> origin(0.0, 0.0, 0.0);
  Vector<3> bottom(2.0, 0.0, 0.0);
  Vector<3> left(0.0, 3.0, 0.0);
  Rectangle3D rectangleXY(origin, bottom, left);

  assert_vector_eq(rectangleXY.getNormal(), {0.0, 0.0, 1.0});

  Vector<3> bottomZ(0.0, 0.0, 4.0);
  Vector<3> leftY(0.0, -5.0, 0.0);
  Rectangle3D rectangleZY(origin, bottomZ, leftY);

  assert_vector_eq(rectangleZY.getNormal(), {1.0, 0.0, 0.0});

  Vector<3> bottomDiag(1.0, 1.0, 0.0);
  Vector<3> leftDiag(0.0, 1.0, 1.0);
  Rectangle3D rectangleDiag(origin, bottomDiag, leftDiag);

  double len = std::sqrt(1.0 + 1.0 + 1.0);
  assert_vector_eq(rectangleDiag.getNormal(),
                   {1.0 / len, -1.0 / len, 1.0 / len});
}

Test(Rectangle3DSuite, GetArea) {
  Point<3> origin(0.0, 0.0, 0.0);
  Vector<3> bottom(2.0, 0.0, 0.0);
  Vector<3> left(0.0, 3.0, 0.0);
  Rectangle3D rectangleOrtho(origin, bottom, left);

  cr_assert_float_eq(rectangleOrtho.getArea(), 6.0, EQ_APPROX);

  Vector<3> bottomDiag(3.0, 0.0, 0.0);
  Vector<3> leftDiag(1.0, 1.0, 0.0);
  Rectangle3D rectangleNonOrtho(origin, bottomDiag, leftDiag);

  cr_assert_float_eq(rectangleNonOrtho.getArea(), 3.0, EQ_APPROX);
}

Test(Rectangle3DSuite, Contains) {
  Point<3> origin(1.0, 2.0, 3.0);
  Vector<3> bottom(4.0, 0.0, 0.0);
  Vector<3> left(0.0, 6.0, 0.0);
  Rectangle3D rectangle(origin, bottom, left);

  cr_assert(rectangle.contains(Point<3>(1.0, 2.0, 3.0)),
            "Origin should be inside");
  cr_assert(rectangle.contains(Point<3>(5.0, 2.0, 3.0)),
            "End of bottom side should be inside");
  cr_assert(rectangle.contains(Point<3>(1.0, 8.0, 3.0)),
            "End of left side should be inside");
  cr_assert(rectangle.contains(Point<3>(5.0, 8.0, 3.0)),
            "Opposite corner should be inside");
  cr_assert(rectangle.contains(Point<3>(3.0, 5.0, 3.0)),
            "Center point should be inside");

  cr_assert_not(rectangle.contains(Point<3>(0.9, 2.0, 3.0)),
                "Point before origin (u<0)");
  cr_assert_not(rectangle.contains(Point<3>(5.1, 2.0, 3.0)),
                "Point past bottom side (u>1)");
  cr_assert_not(rectangle.contains(Point<3>(1.0, 1.9, 3.0)),
                "Point before origin (v<0)");
  cr_assert_not(rectangle.contains(Point<3>(1.0, 8.1, 3.0)),
                "Point past left side (v>1)");
  cr_assert_not(rectangle.contains(Point<3>(6.0, 9.0, 3.0)),
                "Point outside both u and v range");

  cr_assert(rectangle.contains(Point<3>(3.0, 5.0, 10.0)),
            "Center point but off-plane Z");
  cr_assert_not(rectangle.contains(Point<3>(6.0, 9.0, 10.0)),
                "Outside point but off-plane Z");

  Point<3> originNonOrtho(0.0, 0.0, 0.0);
  Vector<3> bottomNonOrtho(1.0, 1.0, 0.0);
  Vector<3> leftNonOrtho(0.0, 1.0, 1.0);
  Rectangle3D rectangleNonOrtho(originNonOrtho, bottomNonOrtho, leftNonOrtho);

  cr_assert(rectangleNonOrtho.contains(Point<3>(0.5, 1.0, 0.5)),
            "Center point (non-orthogonal)");

  cr_assert(rectangleNonOrtho.contains(Point<3>(1.0, 0.0, 0.0)),
            "Point on edge (non-orthogonal)");

  cr_assert_not(rectangleNonOrtho.contains(Point<3>(-0.1, 0.0, 0.0)),
                "Point outside u range (non-orthogonal)");
}
