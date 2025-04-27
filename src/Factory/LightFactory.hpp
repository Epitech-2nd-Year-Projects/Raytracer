#pragma once

#include "Core/IMaterial.hpp"
#include "Lights/AmbientLight.hpp"
#include "Lights/DiffuseLight.hpp"
#include "Lights/PointLight.hpp"
namespace Raytracer::Factory {

/**
 * @class LightFactory
 * @brief Factory for creating different types of lights
 */
class LightFactory {
public:
  /**
   * @brief Create an ambient light
   */
  [[nodiscard]] static std::unique_ptr<Core::ILight>
  createAmbientLight(double intensity) {
    return std::make_unique<Lights::AmbientLight>(intensity);
  }

  /**
   * @brief Create a diffuse light
   */
  [[nodiscard]] static std::unique_ptr<Core::ILight>
  createDiffuseLight(double intensity) {
    return std::make_unique<Lights::DiffuseLight>(intensity);
  }

  /**
   * @brief Create a point light
   */
  [[nodiscard]] static std::unique_ptr<Core::ILight>
  createPointLight(const Math::Point<3> &position, double intensity = 1.0) {
    return std::make_unique<Lights::PointLight>(position, intensity);
  }
};
;
} // namespace Raytracer::Factory
