#pragma once

#include "Core/Ray.hpp"
#include "Math/Point.hpp"
#include "Math/Rectangle.hpp"
#include "Math/Vector.hpp"
#include "Utility/Clamped.hpp"
#include <cmath>

namespace Raytracer::Core {

/**
 * @class Camera
 * @brief A class representing a camera in 3D space.
 */
class Camera {
public:
  /**
   * @brief Default constructor initializes the camera at the origin with a
   * default screen.
   */
  Camera()
      : m_origin(Math::Point<3>(0.0, 0.0, 0.0)),
        m_screen(Math::Rectangle3D(Math::Point<3>(0.0, 0.0, 1.0),
                                   Math::Vector<3>(1.0, 0.0, 0.0),
                                   Math::Vector<3>(0.0, 1.0, 0.0))) {}

  /**
   * @brief Constructor that initializes the camera with a given origin and
   * screen.
   * @param origin The origin of the camera.
   * @param screen The screen of the camera.
   */
  Camera(const Math::Point<3> &origin, const Math::Rectangle3D &screen)
      : m_origin(origin), m_screen(screen) {}

  /**
   * @brief Default destructor for the Camera class.
   */
  ~Camera() = default;

  /**
   * @brief Copy constructor.
   * @param other The camera to copy from.
   */
  Camera(const Camera &other)
      : m_origin(other.m_origin), m_screen(other.m_screen) {}

  /**
   * @brief Move constructor.
   * @param other The camera to move from.
   */
  Camera(Camera &&other) noexcept
      : m_origin(std::move(other.m_origin)),
        m_screen(std::move(other.m_screen)) {}

  /**
   * @brief Copy assignment operator.
   * @param other The camera to copy from.
   * @return A reference to the current camera.
   */
  Camera &operator=(const Camera &other) {
    if (this != &other) {
      m_origin = other.m_origin;
      m_screen = other.m_screen;
    }
    return *this;
  }

  /**
   * @brief Move assignment operator.
   * @param other The camera to move from.
   * @return A reference to the current camera.
   */
  Camera &operator=(Camera &&other) noexcept {
    if (this != &other) {
      m_origin = std::move(other.m_origin);
      m_screen = std::move(other.m_screen);
    }
    return *this;
  }

  /**
   * @brief Get the origin of the camera.
   * @return The origin of the camera.
   */
  [[nodiscard]] const Math::Point<3> &getOrigin() const { return m_origin; }

  /**
   * @brief Get the screen of the camera.
   * @return The screen of the camera.
   */
  [[nodiscard]] const Math::Rectangle3D &getScreen() const { return m_screen; }

  /**
   * @brief Set the origin of the camera.
   * @param origin The new origin of the camera.
   */
  void setOrigin(const Math::Point<3> &origin) { m_origin = origin; }

  /**
   * @brief Set the screen of the camera.
   * @param screen The new screen of the camera.
   */
  void setScreen(const Math::Rectangle3D &screen) { m_screen = screen; }

  /**
   * @brief Generate a ray from the camera to a point on the screen.
   * @param u The u coordinate on the screen (0.0 to 1.0).
   * @param v The v coordinate on the screen (0.0 to 1.0).
   * @return A ray from the camera to the specified point on the screen.
   */
  [[nodiscard]] Core::Ray ray(Utility::Clamped<double, 0.0, 1.0> u,
                              Utility::Clamped<double, 0.0, 1.0> v) const {
    Math::Point<3> screenPoint = m_screen.pointAt(u, v);
    Math::Vector<3> direction = screenPoint - m_origin;

    direction.normalize();

    return Core::Ray(m_origin, direction);
  }

private:
  Math::Point<3> m_origin;
  Math::Rectangle3D m_screen;
};

} // namespace Raytracer::Core
