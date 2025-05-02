/**
 * @file Scene.hpp
 * @brief Defines the Scene class for managing 3D scenes.
 */

#pragma once

#include "Core/Camera.hpp"
#include "Core/ILight.hpp"
#include "Core/IPrimitive.hpp"
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
   * @brief Deleted copy constructor.
   */
  Scene(const Scene &) = delete;

  /**
   * @brief Deleted copy assignment operator.
   */
  Scene &operator=(const Scene &) = delete;

  /**
   * @brief Default move constructor.
   */
  Scene(Scene &&) noexcept = default;

  /**
   * @brief Default move assignment operator.
   */
  Scene &operator=(Scene &&) noexcept = default;

  /**
   * @brief Add a primitive to the scene with a unique identifier.
   * @tparam T Type of primitive deriving from IPrimitive.
   * @param id Unique identifier for the primitive.
   * @param primitive Unique pointer to the primitive.
   * @return true if added successfully, false if ID already exists.
   */
  template <typename T,
            typename = std::enable_if_t<std::is_base_of_v<IPrimitive, T>>>
  bool addPrimitive(const std::string &id, std::unique_ptr<T> primitive) {
    return m_primitives
        .try_emplace(id, std::unique_ptr<IPrimitive>(primitive.release()))
        .second;
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
   * @brief Get a reference to a primitive by its ID.
   * @param id The identifier of the primitive.
   * @return Pointer to the primitive if found, nullptr otherwise.
   */
  [[nodiscard]] IPrimitive *getPrimitive(const std::string &id) const {
    auto it = m_primitives.find(id);
    return (it != m_primitives.end()) ? it->second.get() : nullptr;
  }

  /**
   * @brief Add a light to the scene with a unique identifier.
   * @tparam T Type of light deriving from ILight.
   * @param id Unique identifier for the light.
   * @param light Unique pointer to the light.
   * @return true if added successfully, false if ID already exists.
   */
  template <typename T,
            typename = std::enable_if_t<std::is_base_of_v<ILight, T>>>
  bool addLight(const std::string &id, std::unique_ptr<T> light) {
    return m_lights.try_emplace(id, std::unique_ptr<ILight>(light.release()))
        .second;
  }

  /**
   * @brief Remove a light from the scene by its ID.
   * @param id The identifier of the light to remove.
   * @return true if removed successfully, false if not found.
   */
  bool removeLight(const std::string &id) { return m_lights.erase(id) > 0; }

  /**
   * @brief Get a reference to a light by its ID.
   * @param id The identifier of the light.
   * @return Pointer to the light if found, nullptr otherwise.
   */
  [[nodiscard]] ILight *getLight(const std::string &id) const {
    auto it = m_lights.find(id);
    return (it != m_lights.end()) ? it->second.get() : nullptr;
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
   * @brief Add a child scene to this scene.
   * @param id Unique identifier for the child scene.
   * @param childScene Unique pointer to the child scene.
   * @return true if added successfully, false if ID already exists.
   */
  bool addChildScene(const std::string &id, std::unique_ptr<Scene> childScene) {
    return m_childScenes.try_emplace(id, std::move(childScene)).second;
  }

  /**
   * @brief Remove a child scene by its ID.
   * @param id The identifier of the child scene to remove.
   * @return true if removed successfully, false if not found.
   */
  bool removeChildScene(const std::string &id) {
    return m_childScenes.erase(id) > 0;
  }

  /**
   * @brief Get a child scene by its ID.
   * @param id The identifier of the child scene.
   * @return Pointer to the child scene if found, nullptr otherwise.
   */
  [[nodiscard]] Scene *getChildScene(const std::string &id) const {
    auto it = m_childScenes.find(id);
    return (it != m_childScenes.end()) ? it->second.get() : nullptr;
  }

  /**
   * @brief Get all primitives in the scene.
   * @return Const reference to the primitives container.
   */
  [[nodiscard]] const std::unordered_map<std::string,
                                         std::unique_ptr<IPrimitive>> &
  getPrimitives() const {
    return m_primitives;
  }

  /**
   * @brief Get all lights in the scene.
   * @return Const reference to the lights container.
   */
  [[nodiscard]] const std::unordered_map<std::string, std::unique_ptr<ILight>> &
  getLights() const {
    return m_lights;
  }

  /**
   * @brief Get all child scenes
   * @return Const reference to the child scenes container.
   */
  [[nodiscard]] const std::unordered_map<std::string, std::unique_ptr<Scene>> &
  getChildScenes() const {
    return m_childScenes;
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
   * @brief Clear all child scenes.
   */
  void clearChildScenes() { m_childScenes.clear(); }

  /**
   * @brief Clear the entire scene (primitives, lights, and child scenes).
   */
  void clear() {
    clearPrimitives();
    clearLights();
    clearChildScenes();
  }

  /**
   * @brief Check if a ray intersects with any primitive in the scene
   * @param ray The ray to test for intersection
   * @return true if any intersection is found, false otherwise
   */
  [[nodiscard]] bool hasIntersection(const Ray &ray) const;

  /**
   * @brief Find the nearest intersection point for a ray in the scene
   * @param ray The ray to test for intersection
   * @return Optional containing the nearest intersection if found
   */
  [[nodiscard]] std::optional<Intersection>
  findNearestIntersection(const Ray &ray) const;

private:
  Camera m_camera;
  std::unordered_map<std::string, std::unique_ptr<IPrimitive>> m_primitives;
  std::unordered_map<std::string, std::unique_ptr<ILight>> m_lights;
  std::unordered_map<std::string, std::unique_ptr<Scene>> m_childScenes;
};
} // namespace Raytracer::Core
