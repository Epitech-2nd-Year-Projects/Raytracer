
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
   * @brief Build Cylinder from configuration
   * @param cylinders Configuration for Cylinder
   */
  void buildCylinder(const libconfig::Setting &cylinders);

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

private:
  std::unique_ptr<Core::Scene> m_scene;
};
} // namespace Raytracer::Builder
