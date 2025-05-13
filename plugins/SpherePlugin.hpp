#pragma once
#include "Plugin/PrimitivePlugin.hpp"

namespace Raytracer::Plugins {

/**
 * @class SpherePlugin
 * @brief Plugin for creating a sphere primitive
 */
class SpherePlugin : public Plugin::PrimitivePlugin {
public:
  /**
   * @brief Default constructor
   */
  SpherePlugin() noexcept = default;

  /**
   * @brief Virtual destructor
   */
  ~SpherePlugin() noexcept override = default;

  /**
   * @brief Get the name of this plugin
   * @return The name of the plugin
   */
  [[nodiscard]]
  std::string getName() const override {
    return "Sphere";
  }

  /**
   * @brief Create a new instance of the sphere primitive plugin
   * @return A unique pointer to the new sphere primitive
   */
  std::unique_ptr<PrimitivePlugin> create() override;

  /**
   * @brief Configure the sphere plugin with a libconfig setting
   * @param config The libconfig setting to configure the sphere
   * @return True if the configuration was successful, false otherwise
   */
  bool configure(const libconfig::Setting &config) override;

  /**
   * @brief Set the radius of the sphere
   * @param r The new radius value
   */
  void setRadius(double r) noexcept;

  /**
   * @brief Set the center point of the sphere
   * @param center The new center point
   */
  void setCenter(const Math::Point<3> &center) noexcept;

  /**
   * @brief Get the current radius of the sphere
   * @return The radius value
   */
  [[nodiscard]] double getRadius() const noexcept;

  /**
   * @brief Get the current center point of the sphere
   * @return The center point
   */
  [[nodiscard]] const Math::Point<3> &getCenter() const noexcept;

  /**
   * @brief Calculate intersection between a ray and this sphere
   * @param ray The ray to test for intersection
   * @return An optional Intersection object, containing intersection data if
   * the ray hits the sphere
   */
  [[nodiscard]] std::optional<Core::Intersection>
  intersect(const Core::Ray &ray) const noexcept override;

  /**
   * @brief Calculate the axis-aligned bounding box for this sphere
   * @return The bounding box that completely contains this sphere
   */
  [[nodiscard]] Core::BoundingBox getBoundingBox() const noexcept override;

private:
  double m_radius{1.0};
  Math::Point<3> m_center{};
};

} // namespace Raytracer::Plugins
