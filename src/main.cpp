/**
 * @file Main.cpp
 * @brief Main entry point for the raytracer application.
 */

#include "Core/Renderer.hpp"
#include "Parser/SceneParser.hpp"
#include "Plugin/PluginManager.hpp"
#include <iostream>
#include <memory>
#include <optional>
#include <string_view>

/**
 * @brief Prints the usage instructions for the raytracer application.
 * @param programName The name of the program.
 */
void printUsage(const std::string_view programName) {
  std::cout << "USAGE: " << programName << " <SCENE_FILE> [OPTIONS]\n"
            << "\tSCENE_FILE: scene configuration\n"
            << "OPTIONS:\n"
            << "\t-d: enable debug mode\n"
            << "\t-m: disable multithreading (enabled by default)\n"
            << "\t-o <FILENAME>: specify output file (default: output.ppm)\n"
            << "\t-h, --help: show this help message\n";
}

/**
 * @brief Main function for the raytracer application.
 * @param argc Number of command-line arguments.
 * @param argv Array of command-line arguments.
 * @return Exit status code.
 */
int main(int argc, char *argv[]) {
  const std::string_view programName = argv[0];

  if (argc < 2) {
    printUsage(programName);
    return 84;
  }

  if (argc == 2 && (std::string_view(argv[1]) == "--help" ||
                    std::string_view(argv[1]) == "-h")) {
    printUsage(programName);
    return 0;
  }

  const std::string_view sceneFile = argv[1];
  [[maybe_unused]] bool debug = false;
  bool useMultithreading = true;
  std::string outputFile = "output.ppm";

  for (int i = 2; i < argc; i++) {
    const std::string_view arg = argv[i];
    if (arg == "-d") {
      debug = true;
    } else if (arg == "-m") {
      useMultithreading = false;
    } else if (arg == "-o" && i + 1 < argc) {
      outputFile = argv[++i];
    } else {
      printUsage(programName);
      return 84;
    }
  }

  try {
    Raytracer::Plugin::PluginManager::getInstance().loadPluginsFromDirectory(
        "./plugins");
    std::optional<std::unique_ptr<Raytracer::Core::Scene>> scene =
        Raytracer::Parser::SceneParser().parseFile(sceneFile.data());

    if (!scene) {
      return 84;
    }

    Raytracer::Core::Renderer renderer(1920, 1080);
    renderer.setMultithreading(useMultithreading);
    renderer.render(*scene.value(), outputFile);
  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << "\n";
    return 84;
  }

  return 0;
}
