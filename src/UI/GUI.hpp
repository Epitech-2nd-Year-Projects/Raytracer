#pragma once

#include "Core/Renderer.hpp"
#include "Core/Scene.hpp"
#include "UI/Button.hpp"
#include <SFML/Graphics.hpp>
#include <filesystem>
#include <thread>

namespace Raytracer::UI {
/**
 * @class GUI
 * @brief A class representing the graphical user interface for the raytracer.
 */
class GUI {
public:
  /**
   * @brief Constructor for the GUI class.
   * @param title The title of the window.
   * @param size The size of the window.
   * @param sceneFile The path to the scene file.
   */
  GUI(const std::string &title, const sf::Vector2u &size,
      const std::string &sceneFile);
  /**
   * @brief Destructor for the GUI class.
   */
  ~GUI();

private:
  /**
   * @brief Load a scene from a file.
   * @param filename The path to the scene file.
   * @return true if the scene was loaded successfully, false otherwise.
   */
  [[nodiscard]] bool loadScene(const std::string &filename);

  /**
   * @brief Handle the preview button click event.
   */
  void onPreviewButton();

  /**
   * @brief Handle the render button click event.
   */
  void onRenderButton();

  /**
   * @brief Handle the save button click event.
   */
  void onSaveButton();

private:
  sf::RenderWindow m_window;
  Core::Renderer m_renderer;
  Core::Renderer m_previewRenderer;
  std::unique_ptr<Core::Scene> m_scene;

  std::unique_ptr<Button> m_btnRender;
  std::unique_ptr<Button> m_btnSave;
  std::unique_ptr<Button> m_btnPreview;

  std::atomic<bool> m_cancelRequested{false};
  std::atomic<size_t> m_rowsDone{0};
  bool m_rendering = false;
  std::thread m_renderThread;
  std::vector<uint8_t> m_pixelBuffer;
  sf::Texture m_previewTexture;

  std::atomic<bool> m_previewCancelRequested{false};
  std::atomic<size_t> m_previewRowsDone{0};
  bool m_previewRendering = false;
  std::thread m_previewThread;
  std::vector<uint8_t> m_previewBuffer;
  sf::Texture m_previewTextureLowRes;

  std::string m_sceneFile;
  std::filesystem::file_time_type m_lastConfigWriteTime;
  sf::Font m_font;
  bool m_showReloadNotification{false};
  sf::Clock m_reloadClock;
  sf::Text m_reloadNotification;
};
} // namespace Raytracer::UI
