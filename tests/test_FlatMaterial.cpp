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

class TestDirectionalLight : public Raytracer::Core::ADirectionalLight {
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

Test(FlatMaterialSuite, ComputeColorAmbientOnly) {
  Color diffuse(100.0, 150.0, 200.0);
  Color ambient(50.0, 75.0, 100.0);
  double ambientCoef = 0.2;
  double diffuseCoef = 0.8;

  FlatMaterial material(diffuse, ambient, ambientCoef, diffuseCoef);

  Point<3> point(0.0, 0.0, 0.0);
  Vector<3> normal(0.0, 1.0, 0.0);
  Ray ray(Point<3>(0.0, -5.0, 0.0), Vector<3>(0.0, 1.0, 0.0));
  auto materialPtr = std::make_shared<FlatMaterial>(diffuse, ambient,
                                                    ambientCoef, diffuseCoef);
  Intersection intersection(point, normal, materialPtr, 5.0, false,
                            Point<2>(0.0, 0.0));

  std::vector<std::shared_ptr<Raytracer::Core::ILight>> lights;

  Color computedColor = material.computeColor(intersection, ray, lights);
  Color expectedAmbient(ambient.getR() * ambientCoef,
                        ambient.getG() * ambientCoef,
                        ambient.getB() * ambientCoef);

  cr_assert_float_eq(computedColor.getR(), expectedAmbient.getR(), EQ_APPROX);
  cr_assert_float_eq(computedColor.getG(), expectedAmbient.getG(), EQ_APPROX);
  cr_assert_float_eq(computedColor.getB(), expectedAmbient.getB(), EQ_APPROX);
}

Test(FlatMaterialSuite, ComputeColorWithDirectionalLight) {
  Color diffuse(100.0, 150.0, 200.0);
  Color ambient(50.0, 75.0, 100.0);
  double ambientCoef = 0.2;
  double diffuseCoef = 0.8;

  FlatMaterial material(diffuse, ambient, ambientCoef, diffuseCoef);

  Point<3> point(0.0, 0.0, 0.0);
  Vector<3> normal(0.0, 1.0, 0.0);
  Ray ray(Point<3>(0.0, -5.0, 0.0), Vector<3>(0.0, 1.0, 0.0));
  auto materialPtr = std::make_shared<FlatMaterial>(diffuse, ambient,
                                                    ambientCoef, diffuseCoef);
  Intersection intersection(point, normal, materialPtr, 5.0, false,
                            Point<2>(0.0, 0.0));

  auto light = std::make_shared<TestDirectionalLight>();
  light->setDirection(Vector<3>(0.0, -1.0, 0.0));
  light->setIntensity(1.0);
  light->setColor(Color(255.0, 255.0, 255.0));

  std::vector<std::shared_ptr<Raytracer::Core::ILight>> lights;
  lights.push_back(light);

  Color computedColor = material.computeColor(intersection, ray, lights);

  Color expectedAmbient(ambient.getR() * ambientCoef,
                        ambient.getG() * ambientCoef,
                        ambient.getB() * ambientCoef);

  double cosTheta = 1.0;
  Color expectedDiffuse(diffuse.getR() * diffuseCoef * cosTheta,
                        diffuse.getG() * diffuseCoef * cosTheta,
                        diffuse.getB() * diffuseCoef * cosTheta);

  Color expectedTotal = expectedAmbient.add(expectedDiffuse);

  cr_assert_float_eq(computedColor.getR(), expectedTotal.getR(), EQ_APPROX);
  cr_assert_float_eq(computedColor.getG(), expectedTotal.getG(), EQ_APPROX);
  cr_assert_float_eq(computedColor.getB(), expectedTotal.getB(), EQ_APPROX);
}

Test(FlatMaterialSuite, ComputeColorWithAngledLight) {
  Color diffuse(100.0, 150.0, 200.0);
  Color ambient(50.0, 75.0, 100.0);
  double ambientCoef = 0.2;
  double diffuseCoef = 0.8;

  FlatMaterial material(diffuse, ambient, ambientCoef, diffuseCoef);

  Point<3> point(0.0, 0.0, 0.0);
  Vector<3> normal(0.0, 1.0, 0.0);
  Ray ray(Point<3>(0.0, -5.0, 0.0), Vector<3>(0.0, 1.0, 0.0));
  auto materialPtr = std::make_shared<FlatMaterial>(diffuse, ambient,
                                                    ambientCoef, diffuseCoef);
  Intersection intersection(point, normal, materialPtr, 5.0, false,
                            Point<2>(0.0, 0.0));

  auto light = std::make_shared<TestDirectionalLight>();
  Vector<3> lightDir(1.0, -1.0, 0.0);
  double length = lightDir.length();
  lightDir = lightDir / length;
  light->setDirection(lightDir);
  light->setIntensity(1.0);
  light->setColor(Color(255.0, 255.0, 255.0));

  std::vector<std::shared_ptr<Raytracer::Core::ILight>> lights;
  lights.push_back(light);

  Color computedColor = material.computeColor(intersection, ray, lights);

  Color expectedAmbient(ambient.getR() * ambientCoef,
                        ambient.getG() * ambientCoef,
                        ambient.getB() * ambientCoef);

  double cosTheta = normal.dot(lightDir * -1.0);

  Color expectedDiffuse(diffuse.getR() * diffuseCoef * cosTheta,
                        diffuse.getG() * diffuseCoef * cosTheta,
                        diffuse.getB() * diffuseCoef * cosTheta);

  Color expectedTotal = expectedAmbient.add(expectedDiffuse);

  cr_assert_float_eq(computedColor.getR(), expectedTotal.getR(), EQ_APPROX);
  cr_assert_float_eq(computedColor.getG(), expectedTotal.getG(), EQ_APPROX);
  cr_assert_float_eq(computedColor.getB(), expectedTotal.getB(), EQ_APPROX);
}

Test(FlatMaterialSuite, ComputeColorWithBackLight) {
  Color diffuse(100.0, 150.0, 200.0);
  Color ambient(50.0, 75.0, 100.0);
  double ambientCoef = 0.2;
  double diffuseCoef = 0.8;

  FlatMaterial material(diffuse, ambient, ambientCoef, diffuseCoef);

  Point<3> point(0.0, 0.0, 0.0);
  Vector<3> normal(0.0, 1.0, 0.0);
  Ray ray(Point<3>(0.0, -5.0, 0.0), Vector<3>(0.0, 1.0, 0.0));
  auto materialPtr = std::make_shared<FlatMaterial>(diffuse, ambient,
                                                    ambientCoef, diffuseCoef);
  Intersection intersection(point, normal, materialPtr, 5.0, false,
                            Point<2>(0.0, 0.0));

  auto light = std::make_shared<TestDirectionalLight>();
  light->setDirection(Vector<3>(0.0, 1.0, 0.0));
  light->setIntensity(1.0);
  light->setColor(Color(255.0, 255.0, 255.0));

  std::vector<std::shared_ptr<Raytracer::Core::ILight>> lights;
  lights.push_back(light);

  Color computedColor = material.computeColor(intersection, ray, lights);

  Color expectedAmbient(ambient.getR() * ambientCoef,
                        ambient.getG() * ambientCoef,
                        ambient.getB() * ambientCoef);

  cr_assert_float_eq(computedColor.getR(), expectedAmbient.getR(), EQ_APPROX);
  cr_assert_float_eq(computedColor.getG(), expectedAmbient.getG(), EQ_APPROX);
  cr_assert_float_eq(computedColor.getB(), expectedAmbient.getB(), EQ_APPROX);
}

Test(FlatMaterialSuite, ComputeColorWithColoredLight) {
  Color diffuse(100.0, 150.0, 200.0);
  Color ambient(50.0, 75.0, 100.0);
  double ambientCoef = 0.2;
  double diffuseCoef = 0.8;

  FlatMaterial material(diffuse, ambient, ambientCoef, diffuseCoef);

  Point<3> point(0.0, 0.0, 0.0);
  Vector<3> normal(0.0, 1.0, 0.0);
  Ray ray(Point<3>(0.0, -5.0, 0.0), Vector<3>(0.0, 1.0, 0.0));
  auto materialPtr = std::make_shared<FlatMaterial>(diffuse, ambient,
                                                    ambientCoef, diffuseCoef);
  Intersection intersection(point, normal, materialPtr, 5.0, false,
                            Point<2>(0.0, 0.0));

  auto light = std::make_shared<TestDirectionalLight>();
  light->setDirection(Vector<3>(0.0, -1.0, 0.0));
  light->setIntensity(1.0);
  light->setColor(Color(255.0, 0.0, 0.0));

  std::vector<std::shared_ptr<Raytracer::Core::ILight>> lights;
  lights.push_back(light);

  Color computedColor = material.computeColor(intersection, ray, lights);

  Color expectedAmbient(ambient.getR() * ambientCoef,
                        ambient.getG() * ambientCoef,
                        ambient.getB() * ambientCoef);

  double cosTheta = 1.0;
  Color lightColor = light->getColor();
  Color expectedDiffuse(
      diffuse.getR() * diffuseCoef * cosTheta * lightColor.getR() / 255.0,
      diffuse.getG() * diffuseCoef * cosTheta * lightColor.getG() / 255.0,
      diffuse.getB() * diffuseCoef * cosTheta * lightColor.getB() / 255.0);

  Color expectedTotal = expectedAmbient.add(expectedDiffuse);

  cr_assert_float_eq(computedColor.getR(), expectedTotal.getR(), EQ_APPROX);
  cr_assert_float_eq(computedColor.getG(), expectedTotal.getG(), EQ_APPROX);
  cr_assert_float_eq(computedColor.getB(), expectedTotal.getB(), EQ_APPROX);
}

Test(FlatMaterialSuite, ComputeColorWithMultipleLights) {
  Color diffuse(100.0, 150.0, 200.0);
  Color ambient(50.0, 75.0, 100.0);
  double ambientCoef = 0.2;
  double diffuseCoef = 0.8;

  FlatMaterial material(diffuse, ambient, ambientCoef, diffuseCoef);

  Point<3> point(0.0, 0.0, 0.0);
  Vector<3> normal(0.0, 1.0, 0.0);
  Ray ray(Point<3>(0.0, -5.0, 0.0), Vector<3>(0.0, 1.0, 0.0));
  auto materialPtr = std::make_shared<FlatMaterial>(diffuse, ambient,
                                                    ambientCoef, diffuseCoef);
  Intersection intersection(point, normal, materialPtr, 5.0, false,
                            Point<2>(0.0, 0.0));

  auto light1 = std::make_shared<TestDirectionalLight>();
  light1->setDirection(Vector<3>(0.0, -1.0, 0.0));
  light1->setIntensity(0.5);
  light1->setColor(Color(255.0, 255.0, 255.0));

  auto light2 = std::make_shared<TestDirectionalLight>();
  Vector<3> lightDir(1.0, -1.0, 0.0);
  double length = lightDir.length();
  lightDir = lightDir / length;
  light2->setDirection(lightDir);
  light2->setIntensity(0.5);
  light2->setColor(Color(255.0, 255.0, 255.0));

  std::vector<std::shared_ptr<Raytracer::Core::ILight>> lights;
  lights.push_back(light1);
  lights.push_back(light2);

  Color computedColor = material.computeColor(intersection, ray, lights);

  Color expectedAmbient(ambient.getR() * ambientCoef,
                        ambient.getG() * ambientCoef,
                        ambient.getB() * ambientCoef);

  double cosTheta1 = 1.0;
  Color expectedDiffuse1(
      diffuse.getR() * diffuseCoef * cosTheta1 * light1->getIntensity(),
      diffuse.getG() * diffuseCoef * cosTheta1 * light1->getIntensity(),
      diffuse.getB() * diffuseCoef * cosTheta1 * light1->getIntensity());

  double cosTheta2 = normal.dot(lightDir * -1.0);
  Color expectedDiffuse2(
      diffuse.getR() * diffuseCoef * cosTheta2 * light2->getIntensity(),
      diffuse.getG() * diffuseCoef * cosTheta2 * light2->getIntensity(),
      diffuse.getB() * diffuseCoef * cosTheta2 * light2->getIntensity());

  Color expectedTotal =
      expectedAmbient.add(expectedDiffuse1).add(expectedDiffuse2);

  cr_assert_float_eq(computedColor.getR(), expectedTotal.getR(), EQ_APPROX);
  cr_assert_float_eq(computedColor.getG(), expectedTotal.getG(), EQ_APPROX);
  cr_assert_float_eq(computedColor.getB(), expectedTotal.getB(), EQ_APPROX);
}