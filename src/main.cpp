/**
 * @file Main.cpp
 * @brief Main entry point for the raytracer application.
 */

#include "Core/Renderer.hpp"
#include "Parser/SceneParser.hpp"
#include <iostream>
#include <memory>
#include <optional>
#include <string_view>

/**
 * @brief Prints the usage instructions for the raytracer application.
 * @param programName The name of the program.
 */
void printUsage(const std::string_view programName) {
  std::cout << "USAGE: " << programName << " <SCENE_FILE> [-d]\n"
            << "\tSCENE_FILE: scene configuration\n"
            << "\t-d: enable debug mode\n";
}

/**
 * @brief Main function for the raytracer application.
 * @param argc Number of command-line arguments.
 * @param argv Array of command-line arguments.
 * @return Exit status code.
 */
int main(int argc, char *argv[]) {
  const std::string_view programName = argv[0];

  if (argc == 2 && (std::string_view(argv[1]) == "-help")) {
    printUsage(programName);
    return 0;
  }

  if (argc != 2) {
    printUsage(programName);
    return 84;
  }

  if (argc == 3 && std::string_view(argv[2]) == "-d") {
    // Impelement a Logger class
  }

  const std::string_view sceneFile = argv[1];

  try {
    std::optional<std::unique_ptr<Raytracer::Core::Scene>> scene =
        Raytracer::Parser::SceneParser().parseFile(sceneFile.data());

    if (!scene) {
      return 84;
    }

    Raytracer::Core::Renderer renderer(1920, 1080);
    renderer.render(*scene.value(), "output.ppm");
  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << "\n";
    return 84;
  }

  return 0;
}
