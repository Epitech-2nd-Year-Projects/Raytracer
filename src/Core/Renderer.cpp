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
                           size_t startY, size_t endY) const {
  for (size_t y = startY; y < endY; ++y) {
    for (size_t x = 0; x < m_width; ++x) {
      pixelBuffer[y][x] = computePixelColor(scene, x, y);
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
                                    startY, endY));
    }

    for (auto &thread : threads) {
      thread.join();
    }
  } else {
    renderChunk(scene, pixelBuffer, 0, m_height);
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
  const double invWidth = 1.0 / (m_width - 1);
  const double invHeight = 1.0 / (m_height - 1);

  using ClampedDouble = Utility::Clamped<double, 0.0, 1.0>;

  ClampedDouble uMin(x * invWidth);
  ClampedDouble uMax((x + 1) * invWidth);
  ClampedDouble vMax(1.0 - y * invHeight);
  ClampedDouble vMin(1.0 - (y + 1) * invHeight);

  if (m_enableAdaptiveSS) {
    return sampleRegion(scene, uMin.get(), vMin.get(), uMax.get(), vMax.get(),
                        0);
  }

  ClampedDouble u(x * invWidth);
  ClampedDouble v(1.0 - y * invHeight);
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
        Color color = computePixelColor(scene, x, y);
        size_t i = (y * m_width + x) * 4;
        out[i] = uint8_t(color.getR());
        out[i + 1] = uint8_t(color.getG());
        out[i + 2] = uint8_t(color.getB());
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

[[nodiscard]] Color Renderer::sampleRegion(const Scene &scene, double uMin,
                                           double vMin, double uMax,
                                           double vMax, int depth) const {
  double uMid = 0.5 * (uMin + uMax);
  double vMid = 0.5 * (vMin + vMax);

  std::array<std::pair<double, double>, 5> points = {
      std::make_pair(uMin, vMin), std::make_pair(uMin, vMax),
      std::make_pair(uMax, vMin), std::make_pair(uMax, vMax),
      std::make_pair(uMid, vMid)};

  std::array<Color, 5> cols;
  for (int i = 0; i < 5; ++i) {
    auto [u, v] = points[i];
    cols[i] = traceRay(
        scene, scene.getCamera().ray(Utility::Clamped<double, 0.0, 1.0>(u),
                                     Utility::Clamped<double, 0.0, 1.0>(v)));
  }

  double maxDiff = 0.0;
  for (int i = 0; i < 5; ++i) {
    for (int j = i + 1; j < 5; ++j) {
      double dr = std::abs(cols[i].getR() - cols[j].getR());
      double dg = std::abs(cols[i].getG() - cols[j].getG());
      double db = std::abs(cols[i].getB() - cols[j].getB());
      double d = std::max({dr, dg, db});
      maxDiff = std::max(maxDiff, d);
    }
  }

  if (depth < m_AAMaxDepth && maxDiff > m_AAThreshold) {
    Color color1 = sampleRegion(scene, uMin, vMid, uMid, vMax, depth + 1);
    Color color2 = sampleRegion(scene, uMid, vMid, uMax, vMax, depth + 1);
    Color color3 = sampleRegion(scene, uMin, vMin, uMid, vMid, depth + 1);
    Color color4 = sampleRegion(scene, uMid, vMin, uMax, vMid, depth + 1);
    double r =
        (color1.getR() + color2.getR() + color3.getR() + color4.getR()) * 0.25;
    double g =
        (color1.getG() + color2.getG() + color3.getG() + color4.getG()) * 0.25;
    double b =
        (color1.getB() + color2.getB() + color3.getB() + color4.getB()) * 0.25;
    return Color(r, g, b);
  } else {
    double r = 0, g = 0, b = 0;
    for (auto &col : cols) {
      r += col.getR();
      g += col.getG();
      b += col.getB();
    }
    return Color(r / 5.0, g / 5.0, b / 5.0);
  }
}

} // namespace Raytracer::Core
