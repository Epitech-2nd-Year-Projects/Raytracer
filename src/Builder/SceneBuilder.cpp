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

    /*const std::optional<Math::Point<3>> rotation =
        Parser::SceneParser::parsePoint3(config.lookup("rotation"));*/

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
    for (int i = 0; i < config.getLength(); i++) {
      const libconfig::Setting &primitiveConfig = config[i];
      std::string type, id;

      if (!(primitiveConfig.lookupValue("type", type) &&
            primitiveConfig.lookupValue("id", id))) {
        continue;
      }

      auto primitive = Factory::PrimitiveFactory::createPrimitive(type);
      if (!primitive || !primitive->configure(primitiveConfig)) {
        continue;
      }

      if (primitiveConfig.exists("material")) {
        const libconfig::Setting &materialConfig = primitiveConfig["material"];
        std::string materialType;

        if (materialConfig.lookupValue("type", materialType)) {
          auto material =
              Factory::MaterialFactory::createMaterial(materialType);
          if (material && material->configure(materialConfig)) {
            primitive->setMaterial(material);
          }
        }
      }
      m_scene->addPrimitive(id, std::move(primitive));
    }
  } catch (const libconfig::SettingNotFoundException &) {
  }
  return *this;
}

SceneBuilder &SceneBuilder::buildLights(const libconfig::Setting &config) {
  try {
    buildAmbientLight(config.lookup("ambient"));
    buildDiffuseLight(config.lookup("diffuse"));
    buildPointLights(config.lookup("points"));
    buildDirectionalLights(config.lookup("directional"));
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

void SceneBuilder::buildDirectionalLights(
    const libconfig::Setting &directionals) {
  for (int i = 0; i < directionals.getLength(); i++) {
    try {
      const libconfig::Setting &directionalConfig = directionals[i];
      std::string id = directionalConfig.lookup("id");
      auto directionalLight =
          Factory::LightFactory::createLight("DirectionalLight");

      if (directionalLight->configure(directionalConfig)) {
        m_scene->addLight(id, std::move(directionalLight));
      }
    } catch (const libconfig::SettingNotFoundException &) {
    }
  }
}
} // namespace Raytracer::Builder
