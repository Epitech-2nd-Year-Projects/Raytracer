#include "../src/Core/Camera.hpp"
#include "../src/Core/Ray.hpp"
#include "../src/Math/Point.hpp"
#include "../src/Math/Rectangle.hpp"
#include "../src/Math/Vector.hpp"
#include <cmath>
#include <criterion/criterion.h>

using Raytracer::Core::Camera;
using Raytracer::Core::Ray;
using Raytracer::Math::Point;
using Raytracer::Math::Rectangle3D;
using Raytracer::Math::Vector;

static constexpr double EQ_APPROX = 1e-9;

template <std::size_t N>
void assert_point_eq(const Point<N> &p, const std::array<double, N> &ref,
                     double eps = EQ_APPROX) {
  for (std::size_t i = 0; i < N; ++i) {
    cr_assert_float_eq(p.m_components[i], ref[i], eps,
                       "Point component[%zu]: expected %g but got %g", i,
                       ref[i], p.m_components[i]);
  }
}

template <std::size_t N>
void assert_vector_eq(const Vector<N> &v, const std::array<double, N> &ref,
                      double eps = EQ_APPROX) {
  for (std::size_t i = 0; i < N; ++i) {
    cr_assert_float_eq(v.m_components[i], ref[i], eps,
                       "Vector component[%zu]: expected %g but got %g", i,
                       ref[i], v.m_components[i]);
  }
}

void assert_rectangle_eq(const Rectangle3D &rectangle,
                         const Point<3> &expectedOrigin,
                         const Vector<3> &expectedBottomSide,
                         const Vector<3> &expectedLeftSide,
                         double eps = EQ_APPROX) {
  assert_point_eq(rectangle.getOrigin(),
                  {expectedOrigin.m_components[0],
                   expectedOrigin.m_components[1],
                   expectedOrigin.m_components[2]},
                  eps);
  assert_vector_eq(rectangle.getBottomSide(),
                   {expectedBottomSide.m_components[0],
                    expectedBottomSide.m_components[1],
                    expectedBottomSide.m_components[2]},
                   eps);
  assert_vector_eq(rectangle.getLeftSide(),
                   {expectedLeftSide.m_components[0],
                    expectedLeftSide.m_components[1],
                    expectedLeftSide.m_components[2]},
                   eps);
}

void assert_ray_eq(const Ray &ray, const Point<3> &expectedOrigin,
                   const Vector<3> &expectedDirection, double eps = EQ_APPROX) {
  assert_point_eq(ray.getOrigin(),
                  {expectedOrigin.m_components[0],
                   expectedOrigin.m_components[1],
                   expectedOrigin.m_components[2]},
                  eps);
  assert_vector_eq(ray.getDirection(),
                   {expectedDirection.m_components[0],
                    expectedDirection.m_components[1],
                    expectedDirection.m_components[2]},
                   eps);
  cr_assert_float_eq(ray.getDirection().length(), 1.0, eps,
                     "Ray direction should be normalized (length 1.0), got %g",
                     ray.getDirection().length());
}

Test(CameraSuite, DefaultConstructor) {
  Camera camera;

  assert_point_eq(camera.getOrigin(), {0.0, 0.0, 0.0});

  cr_assert_float_eq(camera.getFov(), M_PI / 2.0, EQ_APPROX,
                     "Expected default FOV %g, got %g", M_PI / 2.0,
                     camera.getFov());

  Point<3> expectedScreenOrigin(0.0, 0.0, 1.0);
  Vector<3> expectedScreenBottomSide(1.0, 0.0, 0.0);
  Vector<3> expectedScreenLeftSide(0.0, 1.0, 0.0);
  assert_rectangle_eq(camera.getScreen(), expectedScreenOrigin,
                      expectedScreenBottomSide, expectedScreenLeftSide);
}

Test(CameraSuite, ParameterizedConstructor) {
  Point<3> origin(1.0, 2.0, -1.0);
  Point<3> screenOrigin(0.0, 0.0, 1.0);
  Vector<3> screenBottomSide(1.6, 0.0, 0.0);
  Vector<3> screenLeftSide(0.0, 0.9, 0.0);
  Rectangle3D screen(screenOrigin, screenBottomSide, screenLeftSide);
  double fov = 60.0 * M_PI / 180.0;

  Camera camera(origin, screen, fov);

  assert_point_eq(camera.getOrigin(), {1.0, 2.0, -1.0});
  assert_rectangle_eq(camera.getScreen(), screenOrigin, screenBottomSide,
                      screenLeftSide);
  cr_assert_float_eq(camera.getFov(), fov, EQ_APPROX, "Expected FOV %g, got %g",
                     fov, camera.getFov());
}

Test(CameraSuite, CopyConstructor) {
  Point<3> origin(1.0, 2.0, -1.0);
  Point<3> screenOrigin(0.0, 0.0, 1.0);
  Vector<3> screenBottomSide(1.0, 0.0, 0.0);
  Vector<3> screenLeftSide(0.0, 1.0, 0.0);
  Rectangle3D screen(screenOrigin, screenBottomSide, screenLeftSide);
  double fov = M_PI / 3.0;
  Camera original(origin, screen, fov);
  Camera copy(original);

  assert_point_eq(copy.getOrigin(), {1.0, 2.0, -1.0});
  assert_rectangle_eq(copy.getScreen(), screenOrigin, screenBottomSide,
                      screenLeftSide);
  cr_assert_float_eq(copy.getFov(), fov, EQ_APPROX);

  original.setOrigin(Point<3>(5.0, 5.0, 5.0));
  original.setFov(M_PI / 4.0);
  original.setScreen(Rectangle3D(Point<3>(1.0, 1.0, 2.0),
                                 Vector<3>(2.0, 0.0, 0.0),
                                 Vector<3>(0.0, 2.0, 0.0)));

  assert_point_eq(copy.getOrigin(), {1.0, 2.0, -1.0});
  assert_rectangle_eq(copy.getScreen(), screenOrigin, screenBottomSide,
                      screenLeftSide);
  cr_assert_float_eq(copy.getFov(), fov, EQ_APPROX);
}

Test(CameraSuite, CopyAssignment) {
  Point<3> origin1(1.0, 1.0, 1.0);
  Point<3> screenOrigin1(0.0, 0.0, 1.0);
  Vector<3> screenBottomSide1(1.0, 0.0, 0.0);
  Vector<3> screenLeftSide1(0.0, 1.0, 0.0);
  Rectangle3D screen1(screenOrigin1, screenBottomSide1, screenLeftSide1);
  double fov1 = M_PI / 2.0;
  Camera camera1(origin1, screen1, fov1);

  Point<3> origin2(2.0, 2.0, 2.0);
  Point<3> screenOrigin2(0.0, 0.0, 2.0);
  Vector<3> screenBottomSide2(2.0, 0.0, 0.0);
  Vector<3> screenLeftSide2(0.0, 2.0, 0.0);
  Rectangle3D screen2(screenOrigin2, screenBottomSide2, screenLeftSide2);
  double fov2 = M_PI / 3.0;
  Camera camera2(origin2, screen2, fov2);

  camera2 = camera1;

  assert_point_eq(camera2.getOrigin(), {1.0, 1.0, 1.0});
  assert_rectangle_eq(camera2.getScreen(), screenOrigin1, screenBottomSide1,
                      screenLeftSide1);
  cr_assert_float_eq(camera2.getFov(), fov1, EQ_APPROX);

  camera1.setOrigin(Point<3>(5.0, 5.0, 5.0));
  camera1.setScreen(Rectangle3D(Point<3>(5.0, 5.0, 5.0),
                                Vector<3>(5.0, 0.0, 0.0),
                                Vector<3>(0.0, 5.0, 0.0)));
  assert_point_eq(camera2.getOrigin(), {1.0, 1.0, 1.0});
  assert_rectangle_eq(camera2.getScreen(), screenOrigin1, screenBottomSide1,
                      screenLeftSide1);
}

Test(CameraSuite, GettersAndSetters) {
  Camera camera;

  Point<3> newOrigin(10.0, -5.0, 2.0);
  camera.setOrigin(newOrigin);
  assert_point_eq(camera.getOrigin(), {10.0, -5.0, 2.0});

  Point<3> screenOrigin(1.0, 1.0, 5.0);
  Vector<3> screenBottomSide(2.0, 0.0, 0.0);
  Vector<3> screenLeftSide(0.0, 1.0, 0.0);
  Rectangle3D newScreen(screenOrigin, screenBottomSide, screenLeftSide);
  camera.setScreen(newScreen);
  assert_rectangle_eq(camera.getScreen(), screenOrigin, screenBottomSide,
                      screenLeftSide);

  double newFov = 45.0 * M_PI / 180.0;
  camera.setFov(newFov);

  cr_assert_float_eq(camera.getFov(), newFov, EQ_APPROX);
}

Test(CameraSuite, SetPerspective) {
  Camera camera;
  camera.setFov(60.0 * M_PI / 180.0);

  double aspectRatio = 16.0 / 9.0;

  camera.setPerspective(aspectRatio);

  double expectedHeight = 2.0 / std::sqrt(3.0);
  double expectedWidth = aspectRatio * expectedHeight;

  Point<3> expectedScreenOrigin(-expectedWidth / 2.0, -expectedHeight / 2.0,
                                1.0);
  Vector<3> expectedScreenBottomSide(expectedWidth, 0.0, 0.0);
  Vector<3> expectedScreenLeftSide(0.0, expectedHeight, 0.0);

  assert_rectangle_eq(camera.getScreen(), expectedScreenOrigin,
                      expectedScreenBottomSide, expectedScreenLeftSide,
                      EQ_APPROX);
}
