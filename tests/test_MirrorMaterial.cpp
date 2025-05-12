#include "../src/Core/Color.hpp"
#include "../src/Core/Ray.hpp"
#include "../src/Materials/MirrorMaterial.hpp"
#include "../src/Math/Point.hpp"
#include "../src/Math/Vector.hpp"
#include <criterion/criterion.h>

using Raytracer::Core::Color;
using Raytracer::Core::Ray;
using Raytracer::Materials::MirrorMaterial;
using Raytracer::Math::Point;
using Raytracer::Math::Vector;

static constexpr double EQ_APPROX = 1e-9;

Test(MirrorMaterialSuite, DefaultConstructor) {
  MirrorMaterial material;

  cr_assert_float_eq(material.getAmbientCoefficient(), 0.0, EQ_APPROX);
  cr_assert_float_eq(material.getDiffuseCoefficient(), 0.0, EQ_APPROX);

  Color ambientColor = material.getAmbientColor();
  cr_assert_float_eq(ambientColor.getR(), 0.0, EQ_APPROX);
  cr_assert_float_eq(ambientColor.getG(), 0.0, EQ_APPROX);
  cr_assert_float_eq(ambientColor.getB(), 0.0, EQ_APPROX);

  Color diffuseColor = material.getDiffuseColor();
  cr_assert_float_eq(diffuseColor.getR(), 0.0, EQ_APPROX);
  cr_assert_float_eq(diffuseColor.getG(), 0.0, EQ_APPROX);
  cr_assert_float_eq(diffuseColor.getB(), 0.0, EQ_APPROX);
}

Test(MirrorMaterialSuite, ParameterizedConstructor) {
  Color diffuseColor(200.0, 150.0, 100.0);
  Color ambientColor(50.0, 40.0, 30.0);
  double ambientCoef = 0.2;
  double diffuseCoef = 0.8;

  MirrorMaterial material(diffuseColor, ambientColor, ambientCoef, diffuseCoef);

  cr_assert_float_eq(material.getAmbientCoefficient(), ambientCoef, EQ_APPROX);
  cr_assert_float_eq(material.getDiffuseCoefficient(), diffuseCoef, EQ_APPROX);

  Color resultAmbient = material.getAmbientColor();
  cr_assert_float_eq(resultAmbient.getR(), ambientColor.getR(), EQ_APPROX);
  cr_assert_float_eq(resultAmbient.getG(), ambientColor.getG(), EQ_APPROX);
  cr_assert_float_eq(resultAmbient.getB(), ambientColor.getB(), EQ_APPROX);

  Color resultDiffuse = material.getDiffuseColor();
  cr_assert_float_eq(resultDiffuse.getR(), diffuseColor.getR(), EQ_APPROX);
  cr_assert_float_eq(resultDiffuse.getG(), diffuseColor.getG(), EQ_APPROX);
  cr_assert_float_eq(resultDiffuse.getB(), diffuseColor.getB(), EQ_APPROX);
}

Test(MirrorMaterialSuite, ReflectionCalculation) {
  Vector<3> normal(0.0, 0.0, 1.0);
  Vector<3> incoming(0.0, -1.0, -1.0);
  incoming = incoming.normalize();

  double dot = normal.dot(incoming);
  Vector<3> expected = incoming - normal * 2.0 * dot;
  expected = expected.normalize();

  cr_assert_float_eq(expected.m_components[0], 0.0, EQ_APPROX);
  cr_assert_float_eq(expected.m_components[1], -0.7071067811865475, EQ_APPROX);
  cr_assert_float_eq(expected.m_components[2], 0.7071067811865475, EQ_APPROX);
}

Test(MirrorMaterialSuite, DepthLimit) {
  Color diffuseColor(200.0, 150.0, 100.0);
  Color ambientColor(50.0, 40.0, 30.0);
  MirrorMaterial material(diffuseColor, ambientColor, 0.2, 0.8);

  cr_assert_float_eq(material.getAmbientCoefficient(), 0.2, EQ_APPROX);
  cr_assert_float_eq(material.getDiffuseCoefficient(), 0.8, EQ_APPROX);

  Color ambient = material.getAmbientColor();
  cr_assert_float_eq(ambient.getR(), 50.0, EQ_APPROX);
  cr_assert_float_eq(ambient.getG(), 40.0, EQ_APPROX);
  cr_assert_float_eq(ambient.getB(), 30.0, EQ_APPROX);
}