#include "Core/Renderer.hpp"
#include "Core/IMaterial.hpp"
#include "Exceptions/OutputException.hpp"
#include <fstream>
#include <memory>
#include <thread>
#include <vector>

namespace Raytracer::Core {
void Renderer::renderChunk(const Scene &scene,
                           std::vector<std::vector<Color>> &pixelBuffer,
                           size_t startY, size_t endY, size_t width,
                           size_t height) const {
  for (size_t y = startY; y < endY; ++y) {
    for (size_t x = 0; x < width; ++x) {
      Utility::Clamped<double, 0.0, 1.0> u(static_cast<double>(x) /
                                           (width - 1));
      Utility::Clamped<double, 0.0, 1.0> v(1.0 - static_cast<double>(y) /
                                                     (height - 1));
      pixelBuffer[y][x] = traceRay(scene, scene.getCamera().ray(u, v));
    }
  }
}

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

  std::vector pixelBuffer(m_height, std::vector<Color>(m_width));

  if (m_useMultithreading) {
    unsigned int threadCount = std::thread::hardware_concurrency();
    threadCount = (threadCount == 0) ? 1 : threadCount;

    std::vector<std::thread> threads;
    size_t linesPerThread = m_height / threadCount;

    for (unsigned int i = 0; i < threadCount; ++i) {
      size_t startY = i * linesPerThread;
      size_t endY = i == threadCount - 1 ? m_height : (i + 1) * linesPerThread;
      threads.push_back(std::thread(&Renderer::renderChunk, this,
                                    std::ref(scene), std::ref(pixelBuffer),
                                    startY, endY, m_width, m_height));
    }

    for (auto &thread : threads) {
      thread.join();
    }
  } else {
    renderChunk(scene, pixelBuffer, 0, m_height, m_width, m_height);
  }

  for (size_t y = 0; y < m_height; ++y) {
    for (size_t x = 0; x < m_width; ++x) {
      Color color = pixelBuffer[y][x];
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
  Utility::Clamped<double, 0.0, 1.0> v(1.0 -
                                       static_cast<double>(y) / (m_height - 1));

  return traceRay(scene, scene.getCamera().ray(u, v));
}

[[nodiscard]] Color Renderer::traceRay(const Scene &scene,
                                       const Ray &ray) const {
  std::optional<Intersection> nearestHit = scene.findNearestIntersection(ray);
  if (!nearestHit) {
    return Color(0, 0, 0);
  }

  std::shared_ptr<IMaterial> material = nearestHit->getMaterial();
  if (!material) {
    return Color(0, 0, 0);
  }

  std::vector<std::shared_ptr<ILight>> lights;
  collectLights(scene, lights);

  return material->computeColor(*nearestHit, ray, lights, scene);
}

std::vector<uint8_t> Renderer::renderToBuffer(const Scene &scene) const {
  std::vector<std::vector<Color>> pixelBuffer(m_height,
                                              std::vector<Color>(m_width));
  double aspectRatio = double(m_width) / double(m_height);
  Camera &cam = const_cast<Camera &>(scene.getCamera());
  cam.setPerspective(aspectRatio);

  if (m_useMultithreading) {
    unsigned int threadCount = std::thread::hardware_concurrency();
    if (threadCount == 0)
      threadCount = 1;
    std::vector<std::thread> threads;
    size_t linesPerThread = m_height / threadCount;

    for (unsigned int i = 0; i < threadCount; ++i) {
      size_t startY = i * linesPerThread;
      size_t endY =
          (i + 1 == threadCount) ? m_height : (i + 1) * linesPerThread;
      threads.emplace_back(&Renderer::renderChunk, this, std::cref(scene),
                           std::ref(pixelBuffer), startY, endY, m_width,
                           m_height);
    }
    for (auto &t : threads)
      t.join();
  } else {
    renderChunk(scene, pixelBuffer, 0, m_height, m_width, m_height);
  }

  std::vector<uint8_t> out;
  out.reserve(m_width * m_height * 4);
  for (size_t y = 0; y < m_height; ++y) {
    for (size_t x = 0; x < m_width; ++x) {
      const auto &c = pixelBuffer[y][x];
      out.push_back(static_cast<uint8_t>(c.getR()));
      out.push_back(static_cast<uint8_t>(c.getG()));
      out.push_back(static_cast<uint8_t>(c.getB()));
      out.push_back(255);
    }
  }
  return out;
}

void Renderer::collectLights(
    const Scene &scene, std::vector<std::shared_ptr<ILight>> &lights) const {
  for (const auto &[id, light] : scene.getLights()) {
    lights.push_back(std::shared_ptr<ILight>(light.get(), [](ILight *) {}));
  }

  for (const auto &[id, childScene] : scene.getChildScenes()) {
    collectLights(*childScene, lights);
  }
}

} // namespace Raytracer::Core
