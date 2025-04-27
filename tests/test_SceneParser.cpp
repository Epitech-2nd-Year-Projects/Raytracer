#include "../src/Math/Point.hpp"
#include "../src/Math/Vector.hpp"
#include "../src/Parser/SceneParser.hpp"
#include <array>
#include <criterion/criterion.h>
#include <libconfig.h++>
#include <string>

using Raytracer::Math::Point;
using Raytracer::Math::Vector;
using Raytracer::Parser::SceneParser;

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

Test(SceneParserSuite, GetSetting_ScalarSuccessAndMissing) {
  const char *cfgText = R"(
        config = {
            a = 42;
            b = 21.049348;
        };
    )";

  libconfig::Config cfg;
  cfg.readString(cfgText);

  SceneParser parser;
  auto &root = cfg.getRoot()["config"];

  auto intValue = parser.getSetting<int>(root, "a");
  cr_assert(intValue.has_value());
  cr_assert_eq(intValue.value(), 42);

  auto doubleValue = parser.getSetting<double>(root, "b");
  cr_assert(doubleValue.has_value());
  cr_assert_float_eq(doubleValue.value(), 21.049348, EQ_APPROX);

  auto missingValue = parser.getSetting<double>(root, "doesNotExist");
  cr_assert(!missingValue.has_value());
}

Test(SceneParserSuite, GetSetting_TypeMismatch) {
  const char *cfgText = R"(
        config = {
            NaN = "NaN";
        };
    )";

  libconfig::Config cfg;
  cfg.readString(cfgText);

  SceneParser parser;
  auto &root = cfg.getRoot()["config"];

  auto badValue = parser.getSetting<double>(root, "NaN");
  cr_assert(!badValue.has_value());
}

Test(SceneParserSuite, ParseVector3_Success) {
  const char *cfgText = R"(
        vector = {
            x = 8.0;
            y = -2.5;
            z = 6.75;
        };
    )";

  libconfig::Config cfg;
  cfg.readString(cfgText);

  SceneParser parser;
  auto &vectorSetting = cfg.getRoot()["vector"];

  auto vector = parser.parseVector3(vectorSetting);
  cr_assert(vector.has_value());
  assert_vector_eq(vector.value(), {8.0, -2.5, 6.75});
}

Test(SceneParserSuite, ParseVector3_MissingComponent) {
  const char *cfgText = R"(
        vector = {
            x = 3.0;
            y = 1.0;
        };
    )";

  libconfig::Config cfg;
  cfg.readString(cfgText);

  SceneParser parser;
  auto &vectorSetting = cfg.getRoot()["vector"];

  auto vector = parser.parseVector3(vectorSetting);
  cr_assert(!vector.has_value());
}

Test(SceneParserSuite, ParseVector3_TypeMismatch) {
  const char *cfgText = R"(
        vector = {
            x = 5.0;
            y = "NaN";
            z = 1.0;
        };
    )";

  libconfig::Config cfg;
  cfg.readString(cfgText);

  SceneParser parser;
  auto &vectorSetting = cfg.getRoot()["vector"];

  auto vector = parser.parseVector3(vectorSetting);
  cr_assert(!vector.has_value());
}

Test(SceneParserSuite, ParsePoint3_Success) {
  const char *cfgText = R"(
        point = {
            x = -2.0;
            y = 1.0;
            z = 3.718;
        };
    )";

  libconfig::Config cfg;
  cfg.readString(cfgText);

  SceneParser parser;
  auto &pointSetting = cfg.getRoot()["point"];

  auto point = parser.parsePoint3(pointSetting);
  cr_assert(point.has_value());
  assert_point_eq(point.value(), {-2.0, 1.0, 3.718});
}

Test(SceneParserSuite, ParsePoint3_MissingComponent) {
  const char *cfgText = R"(
        point = {
            x = 1.0;
            y = 2.0;
        };
    )";

  libconfig::Config cfg;
  cfg.readString(cfgText);

  SceneParser parser;
  auto &pointSetting = cfg.getRoot()["point"];

  auto point = parser.parsePoint3(pointSetting);
  cr_assert(!point.has_value());
}

Test(SceneParserSuite, ParsePoint3_TypeMismatch) {
  const char *cfgText = R"(
        point = {
            x = 1.0;
            y = "NaN";
            z = 3.0;
        };
    )";

  libconfig::Config cfg;
  cfg.readString(cfgText);

  SceneParser parser;
  auto &pointSetting = cfg.getRoot()["point"];

  auto point = parser.parsePoint3(pointSetting);
  cr_assert(!point.has_value());
}
