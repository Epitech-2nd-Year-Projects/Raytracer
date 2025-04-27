#include "Builder/SceneBuilder.hpp"
#include "Factory/LightFactory.hpp"
#include "Factory/MaterialFactory.hpp"
#include "Factory/PrimitiveFactory.hpp"
#include "Math/Point.hpp"

namespace Raytracer::Builder {

SceneBuilder &SceneBuilder::buildCamera(const libconfig::Setting &config) {
  try {
    const libconfig::Setting &resolution = config.lookup("resolution");
    int width = resolution.lookup("width");
    int height = resolution.lookup("height");

    const std::optional<Math::Point<3>> position =
        parsePoint3(config.lookup("position"));

    double fovDegrees = config.lookup("fov");

    if (position) {
      Core::Camera camera;

      camera.setOrigin(*position);
      camera.setFov(fovDegrees * M_PI / 180.0);
      camera.setPerspective(static_cast<double>(width) / height);
      m_scene->setCamera(camera);
    }
  } catch (const libconfig::SettingNotFoundException &) {
  }
  return *this;
}

SceneBuilder &SceneBuilder::buildPrimitives(const libconfig::Setting &config) {
  try {
    buildSpheres(config.lookup("spheres"));
    buildPlanes(config.lookup("planes"));
  } catch (const libconfig::SettingNotFoundException &) {
  }
  return *this;
}

SceneBuilder &SceneBuilder::buildLights(const libconfig::Setting &config) {
  try {
    buildAmbientLight(config.lookup("ambient"));
    buildDiffuseLight(config.lookup("diffuse"));
    buildPointLights(config.lookup("points"));
  } catch (const libconfig::SettingNotFoundException &) {
  }
  return *this;
}

void SceneBuilder::buildSpheres(const libconfig::Setting &spheres) {
  for (const auto &sphere : spheres) {
    try {
      std::string id = sphere.lookup("id");
      auto position = parsePoint3(sphere.lookup("position"));
      double radius = sphere.lookup("radius");

      if (position) {
        auto spherePrimitive =
            Factory::PrimitiveFactory::createSphere(*position, radius);

        const auto &color = sphere.lookup("color");
        auto material = Factory::MaterialFactory::createFlatMaterial(
            Core::Color(color.lookup("r"), color.lookup("g"),
                        color.lookup("b")),
            Core::Color(0, 0, 0));

        spherePrimitive->setMaterial(material);
        m_scene->addPrimitive(id, std::move(spherePrimitive));
      }
    } catch (const libconfig::SettingNotFoundException &) {
    }
  }
}

void SceneBuilder::buildPlanes(const libconfig::Setting &planes) {
  for (const auto &plane : planes) {
    try {
      std::string id = plane.lookup("id");
      std::string axis = plane.lookup("axis");
      double position = plane.lookup("position");

      auto planePrimitive = Factory::PrimitiveFactory::createPlane(
          axis, Math::Point<3>(0.0, 0.0, position));

      const auto &color = plane.lookup("color");
      auto material = Factory::MaterialFactory::createFlatMaterial(
          Core::Color(color.lookup("r"), color.lookup("g"), color.lookup("b")),
          Core::Color(0, 0, 0));

      planePrimitive->setMaterial(material);
      m_scene->addPrimitive(id, std::move(planePrimitive));
    } catch (const libconfig::SettingNotFoundException &) {
    }
  }
}

void SceneBuilder::buildAmbientLight(const libconfig::Setting &ambient) {
  try {
    double intensity = ambient.lookup("intensity");
    auto light = Factory::LightFactory::createAmbientLight(intensity);

    m_scene->addLight("ambient", std::move(light));
  } catch (const libconfig::SettingNotFoundException &) {
  }
}

void SceneBuilder::buildDiffuseLight(const libconfig::Setting &diffuse) {
  try {
    double intensity = diffuse.lookup("intensity");
    auto light = Factory::LightFactory::createDiffuseLight(intensity);

    m_scene->addLight("diffuse", std::move(light));
  } catch (const libconfig::SettingNotFoundException &) {
  }
}

void SceneBuilder::buildPointLights(const libconfig::Setting &points) {
  for (const auto &point : points) {
    try {
      std::string id = point.lookup("id");
      auto position = parsePoint3(point.lookup("position"));

      if (position) {
        auto light = Factory::LightFactory::createPointLight(*position);
        m_scene->addLight(id, std::move(light));
      }
    } catch (const libconfig::SettingNotFoundException &) {
    }
  }
}
} // namespace Raytracer::Builder
