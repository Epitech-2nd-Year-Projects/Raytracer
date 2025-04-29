
/**
 * @file SceneBuilder.hpp
 * @brief SceneBuilder class for constructing scenes from configuration files
 */

#pragma once

#include "Core/Scene.hpp"
#include "Parser/SceneParser.hpp"
#include <libconfig.h++>
namespace Raytracer::Builder {

/**
 * @class SceneBuilder
 * @brief Builder class for constructing scenes from configuration
 */
class SceneBuilder {
public:
  SceneBuilder() : m_scene(std::make_unique<Core::Scene>()) {}

  /**
   * @brief Build camera from configuration
   */
  SceneBuilder &buildCamera(const libconfig::Setting &config);

  /**
   * @brief Build primitives from configuration
   */
  SceneBuilder &buildPrimitives(const libconfig::Setting &config);

  /**
   * @brief Build lights from configuration
   */
  SceneBuilder &buildLights(const libconfig::Setting &config);

  /**
   * @brief Apply transformations from configuration to a primitive
   * @param config The libconfig setting containing transformation parameters
   * @param primitive Pointer to the primitive to which transformations will be
   * applied
   */
  void applyTransformations(const libconfig::Setting &config,
                            Core::IPrimitive *primitive);

  /**
   * @brief Get the built scene
   */
  [[nodiscard]] std::unique_ptr<Core::Scene> getResult() {
    return std::move(m_scene);
  }

private:
  /**
   * @brief Build primitives from configuration
   * @param spheres Configuration for spheres
   */
  void buildSpheres(const libconfig::Setting &spheres);

  /**
   * @brief Build planes from configuration
   * @param planes Configuration for planes
   */
  void buildPlanes(const libconfig::Setting &planes);

  /**
   * @brief Build ambient light from configuration
   * @param ambient Configuration for ambient light
   */
  void buildAmbientLight(const libconfig::Setting &ambient);

  /**
   * @brief Build diffuse light from configuration
   * @param diffuse Configuration for diffuse light
   */
  void buildDiffuseLight(const libconfig::Setting &diffuse);

  /**
   * @brief Build point lights from configuration
   * @param points Configuration for point lights
   */
  void buildPointLights(const libconfig::Setting &points);

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

private:
  std::unique_ptr<Core::Scene> m_scene;
};
} // namespace Raytracer::Builder
