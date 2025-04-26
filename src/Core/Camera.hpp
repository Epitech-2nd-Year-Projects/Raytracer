#pragma once

#include "Core/Ray.hpp"
#include "Math/Point.hpp"
#include "Math/Rectangle.hpp"
#include "Math/Vector.hpp"
#include "Utility/Clamped.hpp"
#include <cmath>

namespace Raytracer::Core {

class Camera {
public:
  Camera()
      : m_origin(Math::Point<3>(0.0, 0.0, 0.0)),
        m_screen(Math::Rectangle3D(Math::Point<3>(0.0, 0.0, 1.0),
                                   Math::Vector<3>(1.0, 0.0, 0.0),
                                   Math::Vector<3>(0.0, 1.0, 0.0))) {}

  Camera(const Math::Point<3> &origin, const Math::Rectangle3D &screen)
      : m_origin(origin), m_screen(screen) {}

  ~Camera() = default;

  Camera(const Camera &other)
      : m_origin(other.m_origin), m_screen(other.m_screen) {}

  Camera(Camera &&other) noexcept
      : m_origin(std::move(other.m_origin)),
        m_screen(std::move(other.m_screen)) {}

  Camera &operator=(const Camera &other) {
    if (this != &other) {
      m_origin = other.m_origin;
      m_screen = other.m_screen;
    }
    return *this;
  }

  Camera &operator=(Camera &&other) noexcept {
    if (this != &other) {
      m_origin = std::move(other.m_origin);
      m_screen = std::move(other.m_screen);
    }
    return *this;
  }

  [[nodiscard]] const Math::Point<3> &getOrigin() const { return m_origin; }

  [[nodiscard]] const Math::Rectangle3D &getScreen() const { return m_screen; }

  void setOrigin(const Math::Point<3> &origin) { m_origin = origin; }

  void setScreen(const Math::Rectangle3D &screen) { m_screen = screen; }

private:
  Math::Point<3> m_origin;
  Math::Rectangle3D m_screen;
};

} // namespace Raytracer::Core
