/**
 * @file SceneParser.hpp
 * @brief Defines the SceneParser class for parsing scene configurations.
 */

#pragma once

#include "Core/Scene.hpp"
#include <libconfig.h++>
#include <optional>

namespace Raytracer::Builder {
class SceneBuilder;
}

namespace Raytracer::Parser {

/**
 * @class SceneParser
 * @brief A class for parsing scene configurations from a file.
 */
class SceneParser {
public:
  /**
   * @brief Parse a scene configuration file.
   * @param filename The name of the configuration file.
   * @return An optional unique pointer to a Core::Scene object.
   */
  [[nodiscard]] std::optional<std::unique_ptr<Core::Scene>>
  parseFile(const std::string &filename);

  /**
   * @brief Template helper function to parse a setting of a specific type.
   * @tparam T The type of the setting.
   * @param setting The libconfig setting to parse.
   * @param name The name of the setting to look up.
   * @return An optional value of the specified type if found, otherwise
   */
  template <typename T>
  static std::optional<T> getSetting(const libconfig::Setting &setting,
                                     const std::string &name) {
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
   * @brief Parse a 3D point from a libconfig setting.
   * @param setting The libconfig setting to parse.
   */
  [[nodiscard]] static std::optional<Math::Point<3>>
  parsePoint3(const libconfig::Setting &setting) {
    try {
      std::optional<double> x =
          Parser::SceneParser::getSetting<double>(setting, "x");
      std::optional<double> y =
          Parser::SceneParser::getSetting<double>(setting, "y");
      std::optional<double> z =
          Parser::SceneParser::getSetting<double>(setting, "z");

      if (!x || !y || !z) {
        return std::nullopt;
      }

      return Math::Point<3>(*x, *y, *z);
    } catch (const libconfig::SettingNotFoundException &e) {
      return std::nullopt;
    } catch (const libconfig::SettingTypeException &e) {
      return std::nullopt;
    }
  }

  /**
   * @brief Apply transformations to a primitive based on the configuration.
   * @param setting The libconfig setting to parse.
   * @param primitive The primitive to apply transformations to.
   */
  static void applyTransformations(const libconfig::Setting &config,
                                   Core::IPrimitive *primitive) {
    try {
      auto position = parsePoint3(config.lookup("position"));
      if (position) {
        primitive->setPosition(*position);
      }
    } catch (const libconfig::SettingNotFoundException &) {
    }

    try {
      auto translation = parsePoint3(config.lookup("translate"));
      if (translation) {
        Math::Point<3> currentPos = primitive->getPosition();
        Math::Point<3> newPos =
            currentPos + Math::Vector<3>(translation->m_components[0],
                                         translation->m_components[1],
                                         translation->m_components[2]);
        primitive->setPosition(newPos);
      }
    } catch (const libconfig::SettingNotFoundException &) {
    }

    try {
      auto rotation = parsePoint3(config.lookup("rotation"));
      if (rotation) {
        Math::Vector<3> rotationRad(rotation->m_components[0] * M_PI / 180.0,
                                    rotation->m_components[1] * M_PI / 180.0,
                                    rotation->m_components[2] * M_PI / 180.0);
        primitive->setRotation(rotationRad);
      }
    } catch (const libconfig::SettingNotFoundException &) {
    }
  }

private:
  libconfig::Config m_config;
};
} // namespace Raytracer::Parser
