/**
 * @file SceneParser.hpp
 * @brief Defines the SceneParser class for parsing scene configurations.
 */

#pragma once

#include "Builder/SceneBuilder.hpp"
#include "Core/Scene.hpp"
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
   * @brief Parse a scene configuration file.
   * @param filename The name of the configuration file.
   * @return An optional unique pointer to a Core::Scene object.
   */
  [[nodiscard]] std::optional<std::unique_ptr<Core::Scene>>
  parseFile(const std::string &filename) {
    try {
      m_config.readFile(filename.c_str());

      Builder::SceneBuilder builder;
      builder.buildCamera(m_config.lookup("camera"))
          .buildPrimitives(m_config.lookup("primitives"))
          .buildLights(m_config.lookup("lights"));

      return builder.getResult();
    } catch (const libconfig::FileIOException &) {
      return std::nullopt;
    } catch (const libconfig::ParseException &) {
      return std::nullopt;
    }
  }

private:
  libconfig::Config m_config;
};
} // namespace Raytracer::Parser
