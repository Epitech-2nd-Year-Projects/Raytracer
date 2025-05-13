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
    for (int i = 0; i < config.getLength(); i++) {
      const libconfig::Setting &lightConfig = config[i];
      std::string type, id;

      if (!(lightConfig.lookupValue("type", type) &&
            lightConfig.lookupValue("id", id))) {
        continue;
      }

      auto light = Factory::LightFactory::createLight(type);
      if (!light || !light->configure(lightConfig)) {
        continue;
      }
      m_scene->addLight(id, std::move(light));
    }
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
} // namespace Raytracer::Builder
