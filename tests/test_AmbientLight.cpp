#include "../src/Core/Color.hpp"
#include "../src/Lights/AmbientLight.hpp"
#include "../src/Math/Point.hpp"
#include "../src/Math/Vector.hpp"
#include <criterion/criterion.h>

using Raytracer::Core::Color;
using Raytracer::Lights::AmbientLight;
using Raytracer::Math::Point;
using Raytracer::Math::Vector;

static constexpr double EQ_APPROX = 1e-9;

TestSuite(AmbientLightSuite);

Test(AmbientLightSuite, DefaultConstructor) {
  AmbientLight light;

  cr_assert_float_eq(light.getIntensity(), 1.0, EQ_APPROX,
                     "Default intensity should be 1.0, got %g",
                     light.getIntensity());

  Color color = light.getColor();
  cr_assert_float_eq(color.getR(), 255.0, EQ_APPROX,
                     "Red component should be 255.0");
  cr_assert_float_eq(color.getG(), 255.0, EQ_APPROX,
                     "Green component should be 255.0");
  cr_assert_float_eq(color.getB(), 255.0, EQ_APPROX,
                     "Blue component should be 255.0");

  cr_assert_not(light.castsShadow(), "Ambient light should not cast shadows");
}

Test(AmbientLightSuite, ParameterizedConstructor) {
  double intensity = 0.5;
  AmbientLight light(intensity);

  cr_assert_float_eq(light.getIntensity(), intensity, EQ_APPROX,
                     "Intensity should be %g, got %g", intensity,
                     light.getIntensity());
}

Test(AmbientLightSuite, SetAndGetColor) {
  AmbientLight light;
  Color color(100.0, 150.0, 200.0);

  light.setColor(color);

  Color lightColor = light.getColor();
  cr_assert_float_eq(lightColor.getR(), color.getR(), EQ_APPROX,
                     "Red component mismatch");
  cr_assert_float_eq(lightColor.getG(), color.getG(), EQ_APPROX,
                     "Green component mismatch");
  cr_assert_float_eq(lightColor.getB(), color.getB(), EQ_APPROX,
                     "Blue component mismatch");
}

Test(AmbientLightSuite, SetAndGetIntensity) {
  AmbientLight light;

  light.setIntensity(0.7);
  cr_assert_float_eq(light.getIntensity(), 0.7, EQ_APPROX,
                     "Intensity should be 0.7, got %g", light.getIntensity());

  light.setIntensity(1.5);
  cr_assert_float_eq(light.getIntensity(), 1.0, EQ_APPROX,
                     "Intensity should be clamped to 1.0, got %g",
                     light.getIntensity());

  light.setIntensity(-0.5);
  cr_assert_float_eq(light.getIntensity(), 0.0, EQ_APPROX,
                     "Intensity should be clamped to 0.0, got %g",
                     light.getIntensity());
}

Test(AmbientLightSuite, ComputeIllumination) {
  double intensity = 0.6;
  AmbientLight light(intensity);

  Point<3> point1(0.0, 0.0, 0.0);
  Vector<3> normal1(0.0, 1.0, 0.0);

  double illumination1 = light.computeIllumination(point1, normal1);
  cr_assert_float_eq(illumination1, intensity, EQ_APPROX,
                     "Illumination should equal intensity %g, got %g",
                     intensity, illumination1);

  Point<3> point2(10.0, 5.0, -3.0);
  Vector<3> normal2(1.0, 0.0, 0.0);

  double illumination2 = light.computeIllumination(point2, normal2);
  cr_assert_float_eq(illumination2, intensity, EQ_APPROX,
                     "Illumination should equal intensity %g, got %g",
                     intensity, illumination2);
}

Test(AmbientLightSuite, GetDirectionFrom) {
  AmbientLight light;

  Point<3> point(5.0, 3.0, -2.0);

  Vector<3> direction = light.getDirectionFrom(point);
  double length = direction.length();

  cr_assert_float_eq(
      length, 1.0, EQ_APPROX,
      "Direction vector should be normalized (length 1.0), got %g", length);
}