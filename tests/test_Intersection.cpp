/*#include "../src/Core/IMaterial.hpp"
#include "../src/Core/Intersection.hpp"
#include "../src/Math/Point.hpp"
#include "../src/Math/Vector.hpp"
#include <criterion/criterion.h>
#include <memory>

using Raytracer::Core::IMaterial;
using Raytracer::Core::Intersection;
using Raytracer::Math::Point;
using Raytracer::Math::Vector;

static constexpr double EQ_APPROX = 1e-9;

class MockMaterial : public IMaterial {
public:
  MockMaterial() = default;
  ~MockMaterial() override = default;

  Raytracer::Core::Color
  computeColor(const Intersection &, const Raytracer::Core::Ray &,
               const std::vector<std::shared_ptr<Raytracer::Core::ILight>> &)
      const override {
    return Raytracer::Core::Color(0, 0, 0);
  }

  const Raytracer::Core::Color &getDiffuseColor() const noexcept override {
    static Raytracer::Core::Color color(0, 0, 0);
    return color;
  }

  const Raytracer::Core::Color &getAmbientColor() const noexcept override {
    static Raytracer::Core::Color color(0, 0, 0);
    return color;
  }

  double getAmbientCoefficient() const noexcept override { return 0.0; }
  double getDiffuseCoefficient() const noexcept override { return 0.0; }

  void setDiffuseColor(const Raytracer::Core::Color &) noexcept override {}
  void setAmbientColor(const Raytracer::Core::Color &) noexcept override {}
  void setAmbientCoefficient(double) noexcept override {}
  void setDiffuseCoefficient(double) noexcept override {}
};

void assert_point3d_eq(const Point<3> &p1, const Point<3> &p2,
                       double eps = EQ_APPROX) {
  for (std::size_t i = 0; i < 3; ++i) {
    cr_assert_float_eq(p1.m_components[i], p2.m_components[i], eps,
                       "component[%zu]: expected %g but got %g", i,
                       p2.m_components[i], p1.m_components[i]);
  }
}

void assert_vector3d_eq(const Vector<3> &v1, const Vector<3> &v2,
                        double eps = EQ_APPROX) {
  for (std::size_t i = 0; i < 3; ++i) {
    cr_assert_float_eq(v1.m_components[i], v2.m_components[i], eps,
                       "component[%zu]: expected %g but got %g", i,
                       v2.m_components[i], v1.m_components[i]);
  }
}

void assert_point2d_eq(const Point<2> &p1, const Point<2> &p2,
                       double eps = EQ_APPROX) {
  for (std::size_t i = 0; i < 2; ++i) {
    cr_assert_float_eq(p1.m_components[i], p2.m_components[i], eps,
                       "component[%zu]: expected %g but got %g", i,
                       p2.m_components[i], p1.m_components[i]);
  }
}

Test(IntersectionSuite, DefaultConstructor) {
  Intersection intersection;

  Point<3> expectedPoint(0.0, 0.0, 0.0);
  assert_point3d_eq(intersection.getPoint(), expectedPoint);

  Vector<3> expectedNormal(0.0, 0.0, 0.0);
  assert_vector3d_eq(intersection.getNormal(), expectedNormal);

  cr_assert_null(intersection.getMaterial().get());

  cr_assert_float_eq(intersection.getDistance(), 0.0, EQ_APPROX);

  cr_assert_eq(intersection.getIsInside(), false);

  Point<2> expectedUV(0.0, 0.0);
  assert_point2d_eq(intersection.getUv(), expectedUV);
}

Test(IntersectionSuite, ParameterizedConstructor) {
  Point<3> point(1.0, 2.0, 3.0);
  Vector<3> normal(0.0, 0.0, 1.0);
  auto material = std::make_shared<MockMaterial>();
  double distance = 5.0;
  bool isInside = true;
  Point<2> uv(0.5, 0.25);

  IMaterial *materialRawPtr = material.get();

  Intersection intersection(point, normal, material, distance, isInside, uv);

  assert_point3d_eq(intersection.getPoint(), point);
  assert_vector3d_eq(intersection.getNormal(), normal);

  cr_assert_not_null(intersection.getMaterial().get());
  cr_assert_eq(intersection.getMaterial().get(), materialRawPtr);

  cr_assert_float_eq(intersection.getDistance(), distance, EQ_APPROX);
  cr_assert_eq(intersection.getIsInside(), isInside);
  assert_point2d_eq(intersection.getUv(), uv);
}
*/
