/**
 * @file Renderer.hpp
 * @brief Defines the Renderer class for generating images from scenes.
 */

#pragma once

#include "Core/Color.hpp"
#include "Core/Scene.hpp"
#include <cstddef>
#include <vector>

namespace Raytracer::Core {
/**
 * @class Renderer
 * @brief Handles rendering of scenes to PPM image files.
 */
class Renderer {
public:
  /**
   * @brief Constructor with image dimensions.
   * @param width Image width in pixels.
   * @param height Image height in pixels.
   */
  Renderer(std::size_t width, std::size_t height)
      : m_width(width), m_height(height) {}

  /**
   * @brief Set the image dimensions.
   * @param width Image width in pixels.
   * @param height Image height in pixels.
   */
  void setDimensions(std::size_t width, std::size_t height) {
    m_width = width;
    m_height = height;
  }

  /**
   * @brief Get image width.
   * @return Width in pixels.
   */
  [[nodiscard]] std::size_t getWidth() const noexcept { return m_width; }

  /**
   * @brief Get image height.
   * @return Height in pixels.
   */
  [[nodiscard]] std::size_t getHeight() const noexcept { return m_height; }

  /**
   * @brief Render a scene to a PPM file.
   * @param scene Scene to render.
   * @param filename Output filename.
   * @return true if successful, false otherwise.
   */
  void render(const Scene &scene, const std::string &filename) const;

private:
  /**
   * @brief Compute the color for a specific pixel.
   * @param scene Scene to render.
   * @param x Pixel x coordinate.
   * @param y Pixel y coordinate.
   * @return Computed pixel color.
   */
  [[nodiscard]] Color computePixelColor(const Scene &scene, std::size_t x,
                                        std::size_t y) const;

  /**
   * @brief Trace a ray through the scene.
   * @param scene Scene to render.
   * @param ray Ray to trace.
   * @return Resulting color.
   */
  [[nodiscard]] Color traceRay(const Scene &scene, const Ray &ray) const;

private:
  /**
   * @brief Collect all lights in the scene.
   * @param scene Scene to render.
   * @param lights Vector to store collected lights.
   */
  void collectLights(const Scene &scene,
                     std::vector<std::shared_ptr<ILight>> &lights) const;

private:
  std::size_t m_width;
  std::size_t m_height;
};
} // namespace Raytracer::Core
