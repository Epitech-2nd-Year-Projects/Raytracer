#include "Parser/SceneParser.hpp"
#include "Builder/SceneBuilder.hpp"

namespace Raytracer::Parser {

std::optional<std::unique_ptr<Core::Scene>>
SceneParser::parseFile(const std::string &filename) {
  try {
    m_config.readFile(filename.c_str());

    Builder::SceneBuilder builder;
    builder.buildCamera(m_config.lookup("camera"))
        .buildPrimitives(m_config.lookup("primitives"))
        .buildLights(m_config.lookup("lights"))
        .buildChildScenes(m_config.lookup("childScenes"));

    return builder.getResult();
  } catch (const libconfig::FileIOException &) {
    return std::nullopt;
  } catch (const libconfig::ParseException &) {
    return std::nullopt;
  }
}

} // namespace Raytracer::Parser
