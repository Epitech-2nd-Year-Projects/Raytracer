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

  /**
   * @brief Render into an RGBA8 buffer in‐place, optionally Cancelling
   *        and reporting progress via atomic flags.
   * @param scene       The scene.
   * @param out         Pre‐allocated RGBA8 buffer of size width*height*4.
   * @param cancelFlag  If non‐null, checked each row; if true, abort early.
   * @param rowsDone    If non‐null, incremented each time a row completes.
   */
  void renderToBuffer(const Scene &scene, std::vector<uint8_t> &out,
                      std::atomic<bool> *cancelFlag = nullptr,
                      std::atomic<size_t> *rowsDone = nullptr) const;
  /**
   * @brief Enable or disable multithreaded rendering.
   * @param enable True to enable multithreading, false to disable.
   */
  void setMultithreading(bool enable) { m_useMultithreading = enable; }

  /**
   * @brief Check if multithreaded rendering is enabled.
   * @return True if multithreading is enabled, false otherwise.
   */
  [[nodiscard]] bool isMultithreadingEnabled() const noexcept {
    return m_useMultithreading;
  }

  /**
   * @brief Enable/disable adaptive supersampling for anti-aliasing.
   * @param enable Turn on/off
   * @param maxDepth Maximum recursion depth
   * @param threshold Color‐difference threshold (0–255)
   */
  void setAdaptiveSupersampling(bool enable, int maxDepth, double threshold) {
    m_enableAdaptiveSS = enable;
    m_AAMaxDepth = maxDepth;
    m_AAThreshold = threshold;
  }

  /**
   * @brief Check if adaptive supersampling is enabled.
   * @return True if enabled, false otherwise.
   */
  [[nodiscard]] bool isAdaptiveSupersampling() const noexcept {
    return m_enableAdaptiveSS;
  }

private:
  /**
   * @brief Render a chunk of the scene.
   * @param scene Scene to render.
   * @param pixelBuffer Buffer to store pixel colors.
   * @param startY Starting Y coordinate of the chunk.
   * @param endY Ending Y coordinate of the chunk.
   */
  void renderChunk(const Scene &scene,
                   std::vector<std::vector<Color>> &pixelBuffer, size_t startY,
                   size_t endY) const;

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

  /** @brief Recursive adaptive‐supersample
   * @param scene Scene to render.
   * @param uMin Minimum U coordinate.
   * @param vMin Minimum V coordinate.
   * @param uMax Maximum U coordinate.
   * @param vMax Maximum V coordinate.
   * @param depth Current recursion depth.
   * @return Computed color.
   */
  [[nodiscard]] Color sampleRegion(const Scene &scene, double uMin, double vMin,
                                   double uMax, double vMax, int depth) const;

private:
  std::size_t m_width;
  std::size_t m_height;

  bool m_useMultithreading = true;

  bool m_enableAdaptiveSS = false;
  int m_AAMaxDepth = 2;
  double m_AAThreshold = 20.0;
};

} // namespace Raytracer::Core
