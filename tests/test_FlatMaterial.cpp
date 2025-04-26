#include "../src/Core/ALight.hpp"
#include "../src/Core/Color.hpp"
#include "../src/Core/Intersection.hpp"
#include "../src/Core/Ray.hpp"
#include "../src/Materials/FlatMaterial.hpp"
#include "../src/Math/Point.hpp"
#include "../src/Math/Vector.hpp"
#include <criterion/criterion.h>
#include <memory>
#include <vector>

using Raytracer::Core::Color;
using Raytracer::Core::Intersection;
using Raytracer::Core::Ray;
using Raytracer::Materials::FlatMaterial;
using Raytracer::Math::Point;
using Raytracer::Math::Vector;

static constexpr double EQ_APPROX = 1e-9;

class MockDirectionalLight : public Raytracer::Core::ADirectionalLight {
public:
  [[nodiscard]] bool castsShadow() const noexcept override { return true; }
};

Test(FlatMaterialSuite, DefaultConstructor) {
  FlatMaterial material;

  cr_assert_float_eq(material.getAmbientCoefficient(), 0.0, EQ_APPROX);
  cr_assert_float_eq(material.getDiffuseCoefficient(), 0.0, EQ_APPROX);

  Color defaultColor = material.getAmbientColor();
  cr_assert_float_eq(defaultColor.getR(), 0.0, EQ_APPROX);
  cr_assert_float_eq(defaultColor.getG(), 0.0, EQ_APPROX);
  cr_assert_float_eq(defaultColor.getB(), 0.0, EQ_APPROX);
}

Test(FlatMaterialSuite, ParameterizedConstructor) {
  Color diffuse(100.0, 150.0, 200.0);
  Color ambient(50.0, 75.0, 100.0);
  double ambientCoef = 0.2;
  double diffuseCoef = 0.8;

  FlatMaterial material(diffuse, ambient, ambientCoef, diffuseCoef);

  cr_assert_float_eq(material.getAmbientCoefficient(), ambientCoef, EQ_APPROX);
  cr_assert_float_eq(material.getDiffuseCoefficient(), diffuseCoef, EQ_APPROX);

  Color materialAmbient = material.getAmbientColor();
  cr_assert_float_eq(materialAmbient.getR(), ambient.getR(), EQ_APPROX);
  cr_assert_float_eq(materialAmbient.getG(), ambient.getG(), EQ_APPROX);
  cr_assert_float_eq(materialAmbient.getB(), ambient.getB(), EQ_APPROX);

  Color materialDiffuse = material.getDiffuseColor();
  cr_assert_float_eq(materialDiffuse.getR(), diffuse.getR(), EQ_APPROX);
  cr_assert_float_eq(materialDiffuse.getG(), diffuse.getG(), EQ_APPROX);
  cr_assert_float_eq(materialDiffuse.getB(), diffuse.getB(), EQ_APPROX);
}
