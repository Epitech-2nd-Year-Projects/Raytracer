#include "ObjectPlugin.hpp"
#include "Parser/SceneParser.hpp"
#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>

std::unique_ptr<Raytracer::Plugin::PrimitivePlugin> ObjectPlugin::create() {
  return std::make_unique<ObjectPlugin>();
}

bool ObjectPlugin::configure(const libconfig::Setting &config) {
  try {
    if (!PrimitivePlugin::configure(config)) {
      return false;
    }

    if (config.exists("file")) {
      std::string filename = config.lookup("file").c_str();
      if (!loadFromFile(filename)) {
        std::cerr << "Failed to load OBJ file: " << filename << std::endl;
        return false;
      }
      m_filename = filename;
    } else {
      std::cerr << "No 'file' setting found in Object configuration"
                << std::endl;
      return false;
    }

    Raytracer::Parser::SceneParser::applyTransformations(config, this);

    return true;
  } catch (const libconfig::SettingNotFoundException &e) {
    std::cerr << "Setting not found in Object configuration: " << e.what()
              << std::endl;
    return false;
  } catch (const std::exception &e) {
    std::cerr << "Error configuring Object: " << e.what() << std::endl;
    return false;
  }
}

bool ObjectPlugin::loadFromFile(const std::string &filename) {
  std::ifstream file(filename);
  if (!file.is_open()) {
    return false;
  }

  m_vertices.clear();
  m_normals.clear();
  m_texCoords.clear();
  m_faces.clear();

  m_vertices.emplace_back(0.0, 0.0, 0.0);
  m_normals.emplace_back(0.0, 0.0, 0.0);
  m_texCoords.emplace_back(0.0, 0.0);

  std::string line;
  while (std::getline(file, line)) {
    std::istringstream iss(line);
    std::string token;
    iss >> token;

    if (token == "v") {
      double x, y, z;
      iss >> x >> y >> z;
      m_vertices.emplace_back(x, y, z);
    } else if (token == "vn") {
      double x, y, z;
      iss >> x >> y >> z;
      m_normals.emplace_back(x, y, z);
    } else if (token == "vt") {
      double u, v;
      iss >> u >> v;
      m_texCoords.emplace_back(u, v);
    } else if (token == "f") {
      Face face;
      std::string vertex;

      while (iss >> vertex) {
        std::istringstream vertexStream(vertex);
        std::string indexStr;
        int indices[3] = {0, 0, 0};
        size_t i = 0;

        while (getline(vertexStream, indexStr, '/')) {
          if (!indexStr.empty()) {
            indices[i] = std::stoi(indexStr);
          }
          ++i;
        }
        face.vertexIndices.push_back(indices[0]);
        face.textureIndices.push_back(indices[1]);
        face.normalIndices.push_back(indices[2]);
      }

      m_faces.push_back(face);
    }
  }

  return !m_vertices.empty() && !m_faces.empty();
}

std::optional<Raytracer::Core::Intersection>
ObjectPlugin::intersect(const Raytracer::Core::Ray &ray) const noexcept {
  Raytracer::Core::Ray localRay = getTransform().inverseTransformRay(ray);

  double closestT = std::numeric_limits<double>::infinity();
  Raytracer::Math::Point<3> closestPoint;
  Raytracer::Math::Vector<3> closestNormal;
  Raytracer::Math::Point<2> closestUV;
  bool isInside = false;

  for (const auto &face : m_faces) {
    if (face.vertexIndices.size() >= 3) {
      for (size_t i = 0; i < face.vertexIndices.size() - 2; ++i) {
        const auto &v0 = m_vertices[face.vertexIndices[0]];
        const auto &v1 = m_vertices[face.vertexIndices[i + 1]];
        const auto &v2 = m_vertices[face.vertexIndices[i + 2]];

        Raytracer::Math::Vector<3> e1 = v1 - v0;
        Raytracer::Math::Vector<3> e2 = v2 - v0;
        Raytracer::Math::Vector<3> p = localRay.getDirection().cross(e2);
        double det = e1.dot(p);

        if (std::abs(det) < 1e-8) {
          continue;
        }

        double invDet = 1.0 / det;
        Raytracer::Math::Vector<3> t = localRay.getOrigin() - v0;
        double u = t.dot(p) * invDet;

        if (u < 0.0 || u > 1.0) {
          continue;
        }

        Raytracer::Math::Vector<3> q = t.cross(e1);
        double v = localRay.getDirection().dot(q) * invDet;

        if (v < 0.0 || u + v > 1.0) {
          continue;
        }

        double t_hit = e2.dot(q) * invDet;

        if (t_hit > localRay.getMinDistance() &&
            t_hit < localRay.getMaxDistance() && t_hit < closestT) {
          closestT = t_hit;
          closestPoint = localRay.at(t_hit);

          if (!face.normalIndices.empty() && face.normalIndices[0] > 0 &&
              face.normalIndices[i + 1] > 0 && face.normalIndices[i + 2] > 0) {
            Raytracer::Math::Vector<3> n0 = m_normals[face.normalIndices[0]];
            Raytracer::Math::Vector<3> n1 =
                m_normals[face.normalIndices[i + 1]];
            Raytracer::Math::Vector<3> n2 =
                m_normals[face.normalIndices[i + 2]];

            closestNormal = n0 * (1.0 - u - v) + n1 * u + n2 * v;
            closestNormal /= closestNormal.length();
          } else {
            closestNormal = e1.cross(e2);
            closestNormal /= closestNormal.length();
          }

          if (!face.textureIndices.empty() && face.textureIndices[0] > 0 &&
              face.textureIndices[i + 1] > 0 &&
              face.textureIndices[i + 2] > 0) {
            Raytracer::Math::Point<2> uv0 = m_texCoords[face.textureIndices[0]];
            Raytracer::Math::Point<2> uv1 =
                m_texCoords[face.textureIndices[i + 1]];
            Raytracer::Math::Point<2> uv2 =
                m_texCoords[face.textureIndices[i + 2]];

            closestUV = Raytracer::Math::Point<2>{
                uv0.m_components[0] * (1.0 - u - v) + uv1.m_components[0] * u +
                    uv2.m_components[0] * v,
                uv0.m_components[1] * (1.0 - u - v) + uv1.m_components[1] * u +
                    uv2.m_components[1] * v};
          } else {
            closestUV = Raytracer::Math::Point<2>{u, v};
          }

          isInside = det < 0;
        }
      }
    }
  }

  if (closestT < std::numeric_limits<double>::infinity()) {
    Raytracer::Math::Point<3> worldPoint =
        getTransform().transformPoint(closestPoint);
    Raytracer::Math::Vector<3> worldNormal =
        getTransform().transformNormal(closestNormal);
    worldNormal /= worldNormal.length();

    double worldDist = (worldPoint - ray.getOrigin()).length();

    return Raytracer::Core::Intersection(worldPoint, worldNormal, getMaterial(),
                                         worldDist, isInside, closestUV);
  }

  return std::nullopt;
}

Raytracer::Core::BoundingBox ObjectPlugin::getBoundingBox() const noexcept {
  if (m_vertices.empty()) {
    return Raytracer::Core::BoundingBox(
        Raytracer::Math::Point<3>{-1.0, -1.0, -1.0},
        Raytracer::Math::Point<3>{1.0, 1.0, 1.0});
  }

  Raytracer::Math::Point<3> min = m_vertices[1];
  Raytracer::Math::Point<3> max = m_vertices[1];

  for (size_t i = 2; i < m_vertices.size(); ++i) {
    const auto &v = m_vertices[i];
    min.m_components[0] = std::min(min.m_components[0], v.m_components[0]);
    min.m_components[1] = std::min(min.m_components[1], v.m_components[1]);
    min.m_components[2] = std::min(min.m_components[2], v.m_components[2]);

    max.m_components[0] = std::max(max.m_components[0], v.m_components[0]);
    max.m_components[1] = std::max(max.m_components[1], v.m_components[1]);
    max.m_components[2] = std::max(max.m_components[2], v.m_components[2]);
  }

  Raytracer::Math::Point<3> corners[8] = {
      Raytracer::Math::Point<3>{min.m_components[0], min.m_components[1],
                                min.m_components[2]},
      Raytracer::Math::Point<3>{min.m_components[0], min.m_components[1],
                                max.m_components[2]},
      Raytracer::Math::Point<3>{min.m_components[0], max.m_components[1],
                                min.m_components[2]},
      Raytracer::Math::Point<3>{min.m_components[0], max.m_components[1],
                                max.m_components[2]},
      Raytracer::Math::Point<3>{max.m_components[0], min.m_components[1],
                                min.m_components[2]},
      Raytracer::Math::Point<3>{max.m_components[0], min.m_components[1],
                                max.m_components[2]},
      Raytracer::Math::Point<3>{max.m_components[0], max.m_components[1],
                                min.m_components[2]},
      Raytracer::Math::Point<3>{max.m_components[0], max.m_components[1],
                                max.m_components[2]}};

  for (auto &corner : corners) {
    corner = getTransform().transformPoint(corner);
  }

  Raytracer::Math::Point<3> worldMin = corners[0];
  Raytracer::Math::Point<3> worldMax = corners[0];

  for (size_t i = 1; i < 8; ++i) {
    const auto &c = corners[i];
    worldMin.m_components[0] =
        std::min(worldMin.m_components[0], c.m_components[0]);
    worldMin.m_components[1] =
        std::min(worldMin.m_components[1], c.m_components[1]);
    worldMin.m_components[2] =
        std::min(worldMin.m_components[2], c.m_components[2]);

    worldMax.m_components[0] =
        std::max(worldMax.m_components[0], c.m_components[0]);
    worldMax.m_components[1] =
        std::max(worldMax.m_components[1], c.m_components[1]);
    worldMax.m_components[2] =
        std::max(worldMax.m_components[2], c.m_components[2]);
  }

  return {worldMin, worldMax};
}

extern "C" {
Raytracer::Plugin::IPlugin *createPlugin() { return new ObjectPlugin(); }

void destroyPlugin(Raytracer::Plugin::IPlugin *plugin) { delete plugin; }
}