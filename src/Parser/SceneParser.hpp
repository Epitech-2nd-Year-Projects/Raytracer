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

private:
  libconfig::Config m_config;
};
} // namespace Raytracer::Parser
