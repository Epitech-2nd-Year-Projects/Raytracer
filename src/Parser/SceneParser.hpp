/**
 * @file SceneParser.hpp
 * @brief Defines the SceneParser class for parsing scene configurations.
 */

#pragma once

#include "../Math/Point.hpp"
#include "../Math/Vector.hpp"
#include <libconfig.h++>
#include <optional>

namespace Raytracer::Parser {
/**
 * @class SceneParser
 * @brief A class for parsing scene configurations from a file.
 */
class SceneParser {
public:
  /**
   * @brief Template helper function to parse a setting of a specific type.
   * @tparam T The type of the setting.
   * @param setting The libconfig setting to parse.
   * @param name The name of the setting to look up.
   * @return An optional value of the specified type if found, otherwise
   */
  template <typename T>
  std::optional<T> getSetting(const libconfig::Setting &setting,
                              const std::string &name) const {
    try {
      T value;

      if (setting.lookupValue(name, value)) {
        return value;
      }
    } catch (const libconfig::SettingTypeException &ex) {
      return std::nullopt;
    }
    return std::nullopt;
  }

  /**
   * @brief Parse a 3D vector from a libconfig setting.
   * @param setting The libconfig setting to parse.
   * @return An optional Math::Vector<3> if all components are found, otherwise
   * std::nullopt.
   */
  std::optional<Math::Vector<3>>
  parseVector3(const libconfig::Setting &setting) {
    std::optional<double> x = getSetting<double>(setting, "x");
    std::optional<double> y = getSetting<double>(setting, "y");
    std::optional<double> z = getSetting<double>(setting, "z");

    if (!x || !y || !z) {
      return std::nullopt;
    }

    return Math::Vector<3>(x.value(), y.value(), z.value());
  }

  /**
   * @brief Parse a 3D point from a libconfig setting.
   * @param setting The libconfig setting to parse.
   * @return An optional Math::Point<3> if all components are found, otherwise
   * std::nullopt.
   */
  std::optional<Math::Point<3>> parsePoint3(const libconfig::Setting &setting) {
    std::optional<double> x = getSetting<double>(setting, "x");
    std::optional<double> y = getSetting<double>(setting, "y");
    std::optional<double> z = getSetting<double>(setting, "z");

    if (!x || !y || !z) {
      return std::nullopt;
    }

    return Math::Point<3>(x.value(), y.value(), z.value());
  }

private:
  libconfig::Config m_config;
};
} // namespace Raytracer::Parser
