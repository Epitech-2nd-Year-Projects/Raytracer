#pragma once
#include "Plugin/MaterialPlugin.hpp"
#include "Core/Intersection.hpp"
#include <libconfig.h++>

namespace Raytracer::Plugins {

/**
 * @class ChessBoardMaterialPlugin
 * @brief Plugin for creating a chessboard pattern material
 */
class ChessBoardMaterialPlugin : public Plugin::MaterialPlugin {
public:
  ChessBoardMaterialPlugin() noexcept = default;
  ~ChessBoardMaterialPlugin() noexcept override = default;

  /**
   * @brief Get the name of this plugin
   * @return The name of the plugin
   */
  [[nodiscard]]
  std::string getName() const override {
    return "ChessBoardMaterial";
  }

  /**
   * @brief Create a new instance of the plugin
   * @return A shared pointer to the new plugin
   */
  std::shared_ptr<MaterialPlugin> create() override;

  /**
   * @brief Configure the plugin with the provided setting
   * @param config The libconfig setting to configure the plugin
   * @return True if configuration was successful, false otherwise
   */
  bool configure(const libconfig::Setting &config) override;

  /**
   * @brief Compute the color at a given intersection
   * @param intersection Intersection details
   * @param ray Incoming ray
   * @param lights All lights in the scene
   * @param scene The scene
   * @return Computed color
   */
  [[nodiscard]]
  Core::Color computeColor(const Core::Intersection &intersection, const Core::Ray &ray,
                           const std::vector<std::shared_ptr<Core::ILight>> &lights,
                           const Core::Scene &scene) const override;

private:
  std::optional<Core::Color> m_color1{Core::White};
  std::optional<Core::Color> m_color2{Core::Black};
  double m_squareSize{1.0};


};

} // namespace Raytracer::Plugins