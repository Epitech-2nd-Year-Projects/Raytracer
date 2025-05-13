
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
   * @param config The libconfig setting containing camera configuration
   */
  SceneBuilder &buildCamera(const libconfig::Setting &config);

  /**
   * @brief Build primitives from configuration
   * @param config The libconfig setting containing primitive configuration
   */
  SceneBuilder &buildPrimitives(const libconfig::Setting &config);

  /**
   * @brief Build lights from configuration
   * @param config The libconfig setting containing light configuration
   */
  SceneBuilder &buildLights(const libconfig::Setting &config);

  /**
   * @brief Build child scenes from configuration
   * @param childScenes Configuration for child scenes
   */
  SceneBuilder &buildChildScenes(const libconfig::Setting &childScenes);

  /**
   * @brief Get the built scene
   */
  [[nodiscard]] std::unique_ptr<Core::Scene> getResult() {
    return std::move(m_scene);
  }

private:
  std::unique_ptr<Core::Scene> m_scene;
};
} // namespace Raytracer::Builder
