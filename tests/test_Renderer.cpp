/**
 * @file test_Renderer.cpp
 * @brief Unit tests for the Renderer class.
 */

#include "../src/Core/Renderer.hpp"
#include <criterion/criterion.h>
#include <cstdio>
#include <fstream>
#include <string>

using namespace Raytracer::Core;

bool fileExists(const std::string &filename) {
  std::ifstream f(filename.c_str());
  return f.good();
}

TestSuite(RendererSuite);

Test(RendererSuite, ConstructorAndGetters) {
  const std::size_t width = 800;
  const std::size_t height = 600;
  Renderer renderer(width, height);

  cr_assert_eq(renderer.getWidth(), width,
               "Width mismatch after construction.");
  cr_assert_eq(renderer.getHeight(), height,
               "Height mismatch after construction.");
}

Test(RendererSuite, SetDimensions) {
  Renderer renderer(100, 100);

  const std::size_t newWidth = 1920;
  const std::size_t newHeight = 1080;
  renderer.setDimensions(newWidth, newHeight);

  cr_assert_eq(renderer.getWidth(), newWidth,
               "Width mismatch after setDimensions.");
  cr_assert_eq(renderer.getHeight(), newHeight,
               "Height mismatch after setDimensions.");
}
