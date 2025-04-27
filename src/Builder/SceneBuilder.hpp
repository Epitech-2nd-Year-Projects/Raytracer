#pragma once

#include "Core/Scene.hpp"
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
  parsePoint3(const libconfig::Setting &setting);

private:
  std::unique_ptr<Core::Scene> m_scene;
};
} // namespace Raytracer::Builder
