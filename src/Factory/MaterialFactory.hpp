#pragma once

#include "Core/IMaterial.hpp"
#include "Materials/FlatMaterial.hpp"
#include "Materials/MirrorMaterial.hpp"
#include "Materials/SteelMaterial.hpp"
namespace Raytracer::Factory {

/**
 * @class MaterialFactory
 * @brief Factory for creating different types of materials
 */
class MaterialFactory {
public:
  /**
   * @brief Create a flat material with specified colors
   */
  [[nodiscard]] static std::shared_ptr<Core::IMaterial>
  createFlatMaterial(const Core::Color &diffuse, const Core::Color &ambient,
                     double ambientCoef = 1.0, double diffuseCoef = 1.0) {
    return std::make_shared<Materials::FlatMaterial>(diffuse, ambient,
                                                     ambientCoef, diffuseCoef);
  }

  /**
   * @brief Create a mirror material with specified colors
   */
  [[nodiscard]] static std::shared_ptr<Core::IMaterial>
  createMirrorMaterial(const Core::Color &diffuse, const Core::Color &ambient,
                       double ambientCoef = 1.0, double diffuseCoef = 1.0) {
    return std::make_shared<Materials::MirrorMaterial>(
        diffuse, ambient, ambientCoef, diffuseCoef);
  }

  /**
   * @brief Create a steel material with specified colors and fuzz factor
   */
  [[nodiscard]] static std::shared_ptr<Core::IMaterial>
  createSteelMaterial(const Core::Color &diffuse, const Core::Color &ambient,
                      double ambientCoef = 1.0, double diffuseCoef = 1.0,
                      double fuzz = 0.3) {
    return std::make_shared<Materials::SteelMaterial>(
        diffuse, ambient, ambientCoef, diffuseCoef, fuzz);
  }
};
} // namespace Raytracer::Factory