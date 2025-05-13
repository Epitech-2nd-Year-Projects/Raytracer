#include "Builder/SceneBuilder.hpp"
#include "Factory/LightFactory.hpp"
#include "Factory/MaterialFactory.hpp"
#include "Factory/PrimitiveFactory.hpp"
#include "Math/Point.hpp"
#include "Parser/SceneParser.hpp"

namespace Raytracer::Builder {

SceneBuilder &SceneBuilder::buildCamera(const libconfig::Setting &config) {
  try {
    const libconfig::Setting &resolution = config.lookup("resolution");
    int width = resolution.lookup("width");
    int height = resolution.lookup("height");

    const std::optional<Math::Point<3>> position =
        Parser::SceneParser::parsePoint3(config.lookup("position"));

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
    buildCylinder(config.lookup("cylinders"));
    buildCone(config.lookup("cones"));
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

SceneBuilder &
SceneBuilder::buildChildScenes(const libconfig::Setting &childScenes) {
  try {
    for (const auto &childScene : childScenes) {
      std::string id = childScene.getName();

      SceneBuilder childBuilder;
      childBuilder.buildCamera(childScene.lookup("camera"))
          .buildPrimitives(childScene.lookup("primitives"))
          .buildLights(childScene.lookup("lights"));

      if (childScene.exists("childScenes")) {
        childBuilder.buildChildScenes(childScene.lookup("childScenes"));
      }

      m_scene->addChildScene(id, childBuilder.getResult());
    }
  } catch (const libconfig::SettingNotFoundException &) {
  }
  return *this;
}

void SceneBuilder::buildSpheres(const libconfig::Setting &spheres) {
  for (const auto &sphereConfig : spheres) {
    try {
      std::string id = sphereConfig.lookup("id");
      auto spherePrimitive =
          Factory::PrimitiveFactory::createPrimitive("Sphere");

      if (spherePrimitive->configure(sphereConfig)) {
        const std::string &materialType = sphereConfig.lookup("material");
        auto material = Factory::MaterialFactory::createMaterial(materialType);

        if (material->configure(sphereConfig)) {
          spherePrimitive->setMaterial(material);
          m_scene->addPrimitive(id, std::move(spherePrimitive));
        }
      }
    } catch (const libconfig::SettingNotFoundException &) {
    }
  }
}

void SceneBuilder::buildPlanes(const libconfig::Setting &planes) {
  for (const auto &planeConfig : planes) {
    try {
      std::string id = planeConfig.lookup("id");
      auto planePrimitive = Factory::PrimitiveFactory::createPrimitive("Plane");

      if (planePrimitive->configure(planeConfig)) {
        const std::string &materialType = planeConfig.lookup("material");
        auto material = Factory::MaterialFactory::createMaterial(materialType);

        if (material->configure(planeConfig)) {
          planePrimitive->setMaterial(material);
          m_scene->addPrimitive(id, std::move(planePrimitive));
        }
      }
    } catch (const libconfig::SettingNotFoundException &) {
    }
  }
}

void SceneBuilder::buildCylinder(const libconfig::Setting &cylinders) {
  for (const auto &cylinderConfig : cylinders) {
    try {
      std::string id = cylinderConfig.lookup("id");
      auto cylinderPrimitive =
          Factory::PrimitiveFactory::createPrimitive("Cylinder");

      if (cylinderPrimitive->configure(cylinderConfig)) {
        const std::string &materialType = cylinderConfig.lookup("material");
        auto material = Factory::MaterialFactory::createMaterial(materialType);

        if (material->configure(cylinderConfig)) {
          cylinderPrimitive->setMaterial(material);
          m_scene->addPrimitive(id, std::move(cylinderPrimitive));
        }
      }
    } catch (const libconfig::SettingNotFoundException &) {
    }
  }
}

void SceneBuilder::buildCone(const libconfig::Setting &cones) {
  for (const auto &coneConfig : cones) {
    try {
      std::string id = coneConfig.lookup("id");
      auto conePrimitive = Factory::PrimitiveFactory::createPrimitive("Cone");

      if (conePrimitive->configure(coneConfig)) {
        const std::string &materialType = coneConfig.lookup("material");
        auto material = Factory::MaterialFactory::createMaterial(materialType);

        if (material->configure(coneConfig)) {
          conePrimitive->setMaterial(material);
          m_scene->addPrimitive(id, std::move(conePrimitive));
        }
      }
    } catch (const libconfig::SettingNotFoundException &) {
    }
  }
}

void SceneBuilder::buildAmbientLight(const libconfig::Setting &ambientConfig) {
  try {
    auto ambientLight = Factory::LightFactory::createLight("AmbientLight");

    if (ambientLight->configure(ambientConfig)) {
      m_scene->addLight("ambient", std::move(ambientLight));
    }
  } catch (const libconfig::SettingNotFoundException &) {
  }
}

void SceneBuilder::buildDiffuseLight(const libconfig::Setting &diffuse) {
  try {
    auto diffuseLight = Factory::LightFactory::createLight("DiffuseLight");

    if (diffuseLight->configure(diffuse)) {
      m_scene->addLight("diffuse", std::move(diffuseLight));
    }
  } catch (const libconfig::SettingNotFoundException &) {
  }
}

void SceneBuilder::buildPointLights(const libconfig::Setting &points) {
  for (const auto &pointConfig : points) {
    try {
      std::string id = pointConfig.lookup("id");
      auto pointLight = Factory::LightFactory::createLight("PointLight");

      if (pointLight->configure(pointConfig)) {
        m_scene->addLight(id, std::move(pointLight));
      }
    } catch (const libconfig::SettingNotFoundException &) {
    }
  }
}
} // namespace Raytracer::Builder
