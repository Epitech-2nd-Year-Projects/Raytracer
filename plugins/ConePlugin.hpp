#pragma once
#include "Plugin/PrimitivePlugin.hpp"

namespace Raytracer::Plugins {

/**
 * @class ConePlugin
 * @brief Plugin for creating a cone primitive
 */
class ConePlugin : public Plugin::PrimitivePlugin {
public:
  /**
   * @brief Default constructor
   */
  ConePlugin() noexcept = default;

  /**
   * @brief Virtual destructor
   */
  ~ConePlugin() noexcept override = default;

  /**
   * @brief Get the name of this plugin
   * @return The name of the plugin
   */
  [[nodiscard]]
  std::string getName() const override {
    return "Cone";
  }

  /**
   * @brief Create a new instance of the cone primitive plugin
   * @return A unique pointer to the new cone primitive
   */
  std::unique_ptr<PrimitivePlugin> create() override;

  /**
   * @brief Configure the cone plugin with a libconfig setting
   * @param config The libconfig setting to configure the cone
   * @return True if the configuration was successful, false otherwise
   */
  bool configure(const libconfig::Setting &config) override;

  /**
   * @brief Calculate the axis-aligned bounding box for this Cone
   * @return The bounding box that represents this Cone
   */
  [[nodiscard]] Core::BoundingBox getBoundingBox() const noexcept override;

  /**
   * @brief Calculate intersection between a ray and this Cone
   * @param ray The ray to test for intersection
   * @return An optional Intersection object, containing intersection data if
   * the ray hits the Cone
   */
  [[nodiscard]] std::optional<Core::Intersection>
  intersect(const Core::Ray &ray) const noexcept override;

  /**
   * @brief Get the axis vector of the Cone
   * @return The normalized axis vector
   */
  [[nodiscard]] const Math::Vector<3> &getAxis() const noexcept {
    return m_axis;
  }

private:
  /**
   * @brief Set the axis, apex position, radius and height of the Cone
   * @param axis The axis direction ("X", "Y", or "Z")
   * @param position The apex position
   * @param radius The radius of the base
   * @param height The height from apex to base
   */
  void setAxisPositionRadiusHeight(const std::string &axis,
                                   const Math::Point<3> &position,
                                   double radius, double height) noexcept;

  Math::Vector<3> m_axis{0.0, 1.0, 0.0};
  Math::Point<3> m_position{};
  double m_radius{1.0};
  double m_height{1.0};
};

} // namespace Raytracer::Plugins
