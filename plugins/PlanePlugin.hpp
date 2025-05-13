#pragma once
#include "Plugin/PrimitivePlugin.hpp"

namespace Raytracer::Plugins {

/**
 * @class PlanePlugin
 * @brief Plugin for creating a plane primitive
 */
class PlanePlugin : public Plugin::PrimitivePlugin {
public:
  /**
   * @brief Default constructor
   */
  PlanePlugin() noexcept = default;

  /**
   * @brief Virtual destructor
   */
  ~PlanePlugin() noexcept override = default;

  /**
   * @brief Get the name of this plugin
   * @return The name of the plugin
   */
  [[nodiscard]]
  std::string getName() const override {
    return "Plane";
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
   * @brief Set the axis and position of the plane
   * @param axis The axis perpendicular to the plane ("X", "Y", or "Z")
   * @param position The position along the axis where the plane intersects
   */
  void setAxisAndPosition(const std::string &axis,
                          Math::Point<3> position) noexcept;

  /**
   * @brief Get the normal vector of the plane
   * @return The normalized normal vector
   */
  [[nodiscard]] const Math::Vector<3> &getNormal() const noexcept;

  /**
   * @brief Get the position of the plane along its axis
   * @return The position value
   */
  [[nodiscard]] const Math::Point<3> &getPosition() const noexcept override;

  /**
   * @brief Calculate intersection between a ray and this plane
   * @param ray The ray to test for intersection
   * @return An optional Intersection object, containing intersection data if
   * the ray hits the plane
   */
  [[nodiscard]] std::optional<Core::Intersection>
  intersect(const Core::Ray &ray) const noexcept override;

  /**
   * @brief Calculate the axis-aligned bounding box for this plane
   * @return The bounding box that represents this infinite plane
   */
  [[nodiscard]] Core::BoundingBox getBoundingBox() const noexcept override;

private:
  Math::Vector<3> m_normal{0.0, 0.0, 0.0};
  Math::Point<3> m_position{0.0, 0.0, 0.0};
};

} // namespace Raytracer::Plugins
