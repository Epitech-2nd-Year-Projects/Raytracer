#include "Core/Renderer.hpp"
#include "Exceptions/OutputException.hpp"
#include <fstream>

namespace Raytracer::Core {
void Renderer::render(const Scene &scene, const std::string &filename) const {
  std::ofstream file(filename);

  if (!file) {
    throw Exceptions::OutputFileException(
        filename, "Failed to open output file for writing.");
  }

  file << "P3\n" << m_width << " " << m_height << "\n255\n";

  double aspectRatio = static_cast<double>(m_width) / m_height;

  Camera &camera = const_cast<Camera &>(scene.getCamera());
  camera.setPerspective(aspectRatio);

  for (size_t y = 0; y < m_height; ++y) {
    for (size_t x = 0; x < m_width; ++x) {
      Color color = computePixelColor(scene, x, y);
      file << static_cast<int>(color.getR()) << " "
           << static_cast<int>(color.getG()) << " "
           << static_cast<int>(color.getB()) << " ";
    }
    file << "\n";
  }
}

[[nodiscard]] Color Renderer::computePixelColor(const Scene &scene,
                                                std::size_t x,
                                                std::size_t y) const {
  (void)scene;
  (void)x;
  (void)y;

  return Color(0, 0, 0);
}

[[nodiscard]] Color Renderer::traceRay(const Scene &scene,
                                       const Ray &ray) const {
  (void)scene;
  (void)ray;

  return Color(0, 0, 0);
}
} // namespace Raytracer::Core
