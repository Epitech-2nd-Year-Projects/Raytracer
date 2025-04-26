/**
 * @file Scene.hpp
 * @brief Defines the Scene class for managing 3D scenes.
 */

#pragma once

#include "Camera.hpp"
#include "ILight.hpp"
#include "IPrimitive.hpp"
#include <memory>
#include <unordered_map>

namespace Raytracer::Core {
/**
 * @class Scene
 * @brief Manages a 3D scene containing primitives, lights, and a camera.
 */
class Scene {
public:
  /**
   * @brief Default constructor.
   */
  Scene() = default;

  /**
   * @brief Default destructor.
   */
  ~Scene() = default;

  /**
   * @brief Add a primitive to the scene with a unique identifier.
   * @param id Unique identifier for the primitive.
   * @param primitive Shared pointer to the primitive.
   * @return true if added successfully, false if ID already exists.
   */
  bool addPrimitive(const std::string &id,
                    std::shared_ptr<IPrimitive> primitive) {
    return m_primitives.try_emplace(id, std::move(primitive)).second;
  }

  /**
   * @brief Remove a primitive from the scene by its ID.
   * @param id The identifier of the primitive to remove.
   * @return true if removed successfully, false if not found.
   */
  bool removePrimitive(const std::string &id) {
    return m_primitives.erase(id) > 0;
  }

  /**
   * @brief Get a primitive by its ID.
   * @param id The identifier of the primitive.
   * @return Shared pointer to the primitive if found, nullptr otherwise.
   */
  [[nodiscard]] std::shared_ptr<IPrimitive>
  getPrimitive(const std::string &id) const {
    auto it = m_primitives.find(id);
    return (it != m_primitives.end()) ? it->second : nullptr;
  }

  /**
   * @brief Add a light to the scene with a unique identifier.
   * @param id Unique identifier for the light.
   * @param light Shared pointer to the light.
   * @return true if added successfully, false if ID already exists.
   */
  bool addLight(const std::string &id, std::shared_ptr<ILight> light) {
    return m_lights.try_emplace(id, std::move(light)).second;
  }

  /**
   * @brief Remove a light from the scene by its ID.
   * @param id The identifier of the light to remove.
   * @return true if removed successfully, false if not found.
   */
  bool removeLight(const std::string &id) { return m_lights.erase(id) > 0; }

  /**
   * @brief Get a light by its ID.
   * @param id The identifier of the light.
   * @return Shared pointer to the light if found, nullptr otherwise.
   */
  [[nodiscard]] std::shared_ptr<ILight> getLight(const std::string &id) const {
    auto it = m_lights.find(id);
    return (it != m_lights.end()) ? it->second : nullptr;
  }

  /**
   * @brief Set the scene's camera.
   * @param camera The new camera.
   */
  void setCamera(Camera camera) { m_camera = std::move(camera); }

  /**
   * @brief Get the scene's camera.
   * @return Reference to the camera.
   */
  [[nodiscard]] const Camera &getCamera() const { return m_camera; }

  /**
   * @brief Get a modifiable reference to the camera.
   * @return Reference to the camera.
   */
  Camera &getCamera() { return m_camera; }

  /**
   * @brief Get all primitives in the scene.
   * @return Const reference to the primitives container.
   */
  [[nodiscard]] const std::unordered_map<std::string,
                                         std::shared_ptr<IPrimitive>> &
  getPrimitives() const {
    return m_primitives;
  }

  /**
   * @brief Get all lights in the scene.
   * @return Const reference to the lights container.
   */
  [[nodiscard]] const std::unordered_map<std::string, std::shared_ptr<ILight>> &
  getLights() const {
    return m_lights;
  }

  /**
   * @brief Clear all primitives from the scene.
   */
  void clearPrimitives() { m_primitives.clear(); }

  /**
   * @brief Clear all lights from the scene.
   */
  void clearLights() { m_lights.clear(); }

  /**
   * @brief Clear the entire scene (primitives and lights).
   */
  void clear() {
    clearPrimitives();
    clearLights();
  }

private:
  Camera m_camera;
  std::unordered_map<std::string, std::shared_ptr<IPrimitive>> m_primitives;
  std::unordered_map<std::string, std::shared_ptr<ILight>> m_lights;
};
} // namespace Raytracer::Core
