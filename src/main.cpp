/**
 * @file Main.cpp
 * @brief Main entry point for the raytracer application.
 */

#include <iostream>
#include <string_view>

/**
 * @brief Prints the usage instructions for the raytracer application.
 * @param programName The name of the program.
 */
void printUsage(const std::string_view programName) {
  std::cout << "USAGE: " << programName << " <SCENE_FILE>\n"
            << "\tSCENE_FILE: scene configuration\n";
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

  const std::string_view sceneFile = argv[1];

  // call the parser, build the scene, and render it
  return 0;
}
