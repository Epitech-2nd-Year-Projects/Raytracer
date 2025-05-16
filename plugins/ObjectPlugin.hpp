#pragma once
#include "Plugin/PrimitivePlugin.hpp"
#include <string>
#include <vector>

/**
 * @class ObjectPlugin
 * @brief A plugin for creating and configuring object primitives in a
 * raytracer.
 */
class ObjectPlugin : public Raytracer::Plugin::PrimitivePlugin {
public:
  /**
   * @brief Default Constructor
   */
  ObjectPlugin() noexcept = default;

  /**
   * @brief Virtual destructor
   */
  ~ObjectPlugin() noexcept override = default;

  /**
   * @brief Get the name of this plugin
   * @return The name of the plugin
   */
  [[nodiscard]] std::string getName() const override { return "Object"; }

  /**
   * @brief Create a new instance of the object primitive plugin
   * @return A unique pointer to the new object primitive
   */
  std::unique_ptr<PrimitivePlugin> create() override;

  /**
   * @brief Configure the object plugin with a libconfig setting
   * @param config The libconfig setting to configure the object
   * @return True if the configuration was successful, false otherwise
   */
  bool configure(const libconfig::Setting &config) override;

  /**
   * @brief Calculate the intersection between a ray and this Object
   * @param ray The ray to test for intersection
   * @return An optional Intersection object, containing intersection data if
   * the ray hits the Object
   */
  [[nodiscard]] std::optional<Raytracer::Core::Intersection>
  intersect(const Raytracer::Core::Ray &ray) const noexcept override;

  /**
   * @brief Calculate the axis-aligned bounding box for this Object
   * @return The bounding box that represents this Object
   */
  [[nodiscard]] Raytracer::Core::BoundingBox
  getBoundingBox() const noexcept override;

  /**
   * @brief Loads and parses an object file to initialize the object's geometry.
   * @param filename The path to the file to be loaded.
   * @return True if the file was successfully loaded and parsed, false
   * otherwise.
   */
  bool loadFromFile(const std::string &filename);

private:
  struct Face {
    std::vector<int> vertexIndices;
    std::vector<int> normalIndices;
    std::vector<int> textureIndices;
  };

  std::vector<Raytracer::Math::Point<3>> m_vertices;
  std::vector<Raytracer::Math::Vector<3>> m_normals;
  std::vector<Raytracer::Math::Point<2>> m_texCoords;
  std::vector<Face> m_faces;
  std::string m_filename;
  std::string m_texture;
};