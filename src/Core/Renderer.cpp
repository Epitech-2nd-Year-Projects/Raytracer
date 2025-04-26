#include "Core/Renderer.hpp"
#include "Core/IMaterial.hpp"
#include "Exceptions/OutputException.hpp"
#include <fstream>
#include <memory>

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
  Utility::Clamped<double, 0.0, 1.0> u(static_cast<double>(x) / (m_width - 1));
  Utility::Clamped<double, 0.0, 1.0> v(static_cast<double>(y) / (m_height - 1));

  return traceRay(scene, scene.getCamera().ray(u, v));
}

[[nodiscard]] Color Renderer::traceRay(const Scene &scene,
                                       const Ray &ray) const {
  std::optional<Intersection> nearestHit;
  double nearestDistance = std::numeric_limits<double>::infinity();

  for (const auto &[id, primitive] : scene.getPrimitives()) {
    if (std::optional<Intersection> hit = primitive->intersect(ray)) {
      if (hit->getDistance() < nearestDistance) {
        nearestDistance = hit->getDistance();
        nearestHit = hit;
      }
    }
  }

  if (!nearestHit) {
    return Color(0, 0, 0);
  }

  std::shared_ptr<IMaterial> material = nearestHit->getMaterial();

  if (!material) {
    return Color(0, 0, 0);
  }

  std::vector<std::shared_ptr<ILight>> lights;

  for (const auto &[id, light] : scene.getLights()) {
    lights.push_back(std::shared_ptr<ILight>(light.get(), [](ILight *) {}));
  }

  return material->computeColor(*nearestHit, ray, lights);
}
} // namespace Raytracer::Core
