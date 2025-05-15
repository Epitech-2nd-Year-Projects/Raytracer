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

void Renderer::renderToBuffer(const Scene &scene, std::vector<uint8_t> &out,
                              std::atomic<bool> *cancelFlag,
                              std::atomic<size_t> *rowsDone) const {
  if (out.size() < m_width * m_height * 4)
    throw std::runtime_error("renderToBuffer: output buffer too small");

  if (rowsDone)
    rowsDone->store(0, std::memory_order_relaxed);

  auto worker = [&](size_t y0, size_t y1) {
    for (size_t y = y0; y < y1; ++y) {
      if (cancelFlag && cancelFlag->load(std::memory_order_relaxed))
        return;
      for (size_t x = 0; x < m_width; ++x) {
        if (cancelFlag && cancelFlag->load(std::memory_order_relaxed))
          return;
        auto u = Utility::Clamped<double, 0.0, 1.0>(double(x) / (m_width - 1));
        auto v = Utility::Clamped<double, 0.0, 1.0>(1.0 -
                                                    double(y) / (m_height - 1));
        Color color = traceRay(scene, scene.getCamera().ray(u, v));

        size_t i = (y * m_width + x) * 4;
        out[i] = static_cast<uint8_t>(color.getR());
        out[i + 1] = static_cast<uint8_t>(color.getG());
        out[i + 2] = static_cast<uint8_t>(color.getB());
        out[i + 3] = 255;
      }
      if (rowsDone)
        rowsDone->fetch_add(1, std::memory_order_relaxed);
    }
  };

  if (m_useMultithreading) {
    unsigned threads = std::thread::hardware_concurrency();
    threads = threads ? threads : 1;
    size_t stripe = m_height / threads;
    std::vector<std::thread> threadPool;
    threadPool.reserve(threads);

    for (unsigned i = 0; i < threads; ++i) {
      size_t y0 = i * stripe;
      size_t y1 = (i + 1 == threads) ? m_height : (i + 1) * stripe;
      threadPool.emplace_back(worker, y0, y1);
    }
    for (auto &thread : threadPool)
      thread.join();
  } else {
    worker(0, m_height);
  }
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
