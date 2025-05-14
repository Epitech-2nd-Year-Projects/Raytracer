#pragma once
#include "Plugin/PrimitivePlugin.hpp"
#include <fstream>
#include <vector>
#include <string>

class ObjectPlugin : public Raytracer::Plugin::PrimitivePlugin {
public:
  ObjectPlugin() noexcept = default;
  ~ObjectPlugin() noexcept override = default;
  [[nodiscard]] std::string getName() const override {
    return "Object";
  }
  std::unique_ptr<PrimitivePlugin> create() override;
  bool configure(const libconfig::Setting &config) override;

  [[nodiscard]] std::optional<Raytracer::Core::Intersection>
  intersect(const Raytracer::Core::Ray &ray) const noexcept override;

  [[nodiscard]] Raytracer::Core::BoundingBox getBoundingBox() const noexcept override;

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