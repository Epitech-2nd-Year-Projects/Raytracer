#include "UI/GUI.hpp"
#include "Parser/SceneParser.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>

using namespace Raytracer::UI;

GUI::GUI(const std::string &title, const sf::Vector2u &size,
         const std::string &sceneFile)
    : m_window({size.x, size.y}, title), m_renderer(size.x, size.y),
      m_previewRenderer(size.x / 2, size.y / 2), m_sceneFile(sceneFile) {
  m_window.setFramerateLimit(60);

  m_previewTexture.create(size.x, size.y);
  m_previewTexture.create(size.x, size.y);
  m_previewTextureLowRes.create(size.x / 2, size.y / 2);

  if (!loadScene(m_sceneFile)) {
    std::cerr << "Failed to load scene: " << m_sceneFile << "\n";
  }

  try {
    m_lastConfigWriteTime = std::filesystem::last_write_time(m_sceneFile);
  } catch (const std::filesystem::filesystem_error &e) {
    std::cerr << "Warning: cannot watch file: " << e.what() << "\n";
  }

  if (!m_font.loadFromFile("assets/DejaVuSans.ttf")) {
    throw std::runtime_error("Failed to load font");
  }

  m_reloadNotification.setFont(m_font);
  m_reloadNotification.setString("Scene changed and reloaded");
  m_reloadNotification.setCharacterSize(24);
  m_reloadNotification.setFillColor(sf::Color::White);
  m_reloadNotification.setOutlineColor(sf::Color::Black);
  m_reloadNotification.setOutlineThickness(2.f);

  auto notificationBounds = m_reloadNotification.getLocalBounds();
  m_reloadNotification.setOrigin(
      notificationBounds.left + notificationBounds.width / 2.f,
      notificationBounds.top + notificationBounds.height / 2.f);
  m_reloadNotification.setPosition(size.x / 2.f, size.y / 2.f);

  m_btnPreview = std::make_unique<Button>(
      m_font, "Preview", sf::Vector2f(10, 10), sf::Vector2f(140, 30),
      [this]() { onPreviewButton(); });
  m_btnRender = std::make_unique<Button>(m_font, "Render", sf::Vector2f(10, 50),
                                         sf::Vector2f(140, 30),
                                         [this]() { onRenderButton(); });
  m_btnSave = std::make_unique<Button>(m_font, "Save", sf::Vector2f(10, 90),
                                       sf::Vector2f(140, 30),
                                       [this]() { onSaveButton(); });
  m_btnToggleAASS = std::make_unique<Button>(
      m_font, "Toggle AA-SS", sf::Vector2f(10, 130), sf::Vector2f(140, 30),
      [this]() { onToggleASS(); });

  while (m_window.isOpen()) {
    try {
      auto currentWriteTime = std::filesystem::last_write_time(m_sceneFile);
      if (currentWriteTime != m_lastConfigWriteTime) {
        m_lastConfigWriteTime = currentWriteTime;
        m_cancelRequested.store(true, std::memory_order_relaxed);

        if (m_renderThread.joinable())
          m_renderThread.join();

        m_rendering = false;
        m_btnRender->setText("Render");
        m_pixelBuffer.assign(m_renderer.getWidth() * m_renderer.getHeight() * 4,
                             0);
        m_previewBuffer.assign(m_previewRenderer.getWidth() *
                                   m_previewRenderer.getHeight() * 4,
                               0);

        if (!loadScene(m_sceneFile)) {
          std::cerr << "Failed to reload scene: " << m_sceneFile << "\n";
        }

        m_showReloadNotification = true;
        m_reloadClock.restart();
      }
    } catch (const std::filesystem::filesystem_error &) {
    }

    if (!m_rendering && m_renderThread.joinable()) {
      m_renderThread.join();
      m_btnRender->setText("Render");
    }

    if (!m_previewRendering && m_previewThread.joinable()) {
      m_previewThread.join();
      m_btnPreview->setText("Preview");
    }

    if (m_renderer.isAdaptiveSupersampling()) {
      m_btnToggleAASS->setText("Disable AA-SS");
    } else {
      m_btnToggleAASS->setText("Enable AA-SS");
    }

    sf::Event ev;
    while (m_window.pollEvent(ev)) {
      if (ev.type == sf::Event::Closed)
        m_window.close();
      m_btnPreview->handleEvent(ev, m_window);
      m_btnRender->handleEvent(ev, m_window);
      m_btnSave->handleEvent(ev, m_window);
      m_btnToggleAASS->handleEvent(ev, m_window);
    }

    m_window.clear();

    if (!m_previewBuffer.empty()) {
      m_previewTextureLowRes.update(m_previewBuffer.data());
      sf::Sprite previewSprite(m_previewTextureLowRes);
      previewSprite.setScale(
          float(m_renderer.getWidth()) / m_previewRenderer.getWidth(),
          float(m_renderer.getHeight()) / m_previewRenderer.getHeight());
      m_window.draw(previewSprite);
    }

    if (!m_pixelBuffer.empty()) {
      m_previewTexture.update(m_pixelBuffer.data());
      m_window.draw(sf::Sprite(m_previewTexture));
    }

    if (m_rendering) {
      size_t done = m_rowsDone.load(std::memory_order_relaxed);
      size_t total = m_renderer.getHeight();
      int progress = int(double(done) * 100.0 / double(total));
      m_btnRender->setText("Stop (" + std::to_string(progress) + "%)");
    }

    if (m_previewRendering) {
      size_t done = m_previewRowsDone.load(std::memory_order_relaxed);
      size_t total = m_previewRenderer.getHeight();
      int progress = int(double(done) * 100.0 / double(total));
      m_btnPreview->setText("Stop (" + std::to_string(progress) + "%)");
    }

    m_btnPreview->draw(m_window);
    m_btnRender->draw(m_window);
    m_btnSave->draw(m_window);
    m_btnToggleAASS->draw(m_window);

    if (m_showReloadNotification) {
      float notificationTime = m_reloadClock.getElapsedTime().asSeconds();
      if (notificationTime <= 2.f) {
        sf::RectangleShape overlay(sf::Vector2f(m_window.getSize()));
        overlay.setFillColor(sf::Color(0, 0, 0, 150));
        m_window.draw(overlay);
        m_window.draw(m_reloadNotification);
      } else {
        m_showReloadNotification = false;
      }
    }

    m_window.display();
  }
}

GUI::~GUI() {
  m_cancelRequested.store(true);
  if (m_renderThread.joinable())
    m_renderThread.join();
  m_window.close();
}

bool GUI::loadScene(const std::string &fn) {
  auto parsedScene = Parser::SceneParser().parseFile(fn.c_str());
  if (!parsedScene) {
    std::cerr << "Failed to parse scene: " << fn << "\n";
    return false;
  }
  m_scene = std::move(*parsedScene);
  return true;
}

void GUI::onPreviewButton() {
  if (!m_previewRendering) {
    if (!m_pixelBuffer.empty()) {
      m_pixelBuffer.clear();
      m_btnRender->setText("Render");
    }
    if (m_previewThread.joinable())
      m_previewThread.join();

    m_previewCancelRequested.store(false, std::memory_order_relaxed);
    m_previewRowsDone.store(0, std::memory_order_relaxed);
    m_previewRendering = true;
    m_btnPreview->setText("Stop (0%)");

    size_t w = m_previewRenderer.getWidth();
    size_t h = m_previewRenderer.getHeight();
    m_previewBuffer.assign(w * h * 4, 0);
    m_previewThread = std::thread([this]() {
      m_previewRenderer.renderToBuffer(*m_scene, m_previewBuffer,
                                       &m_previewCancelRequested,
                                       &m_previewRowsDone);
      m_previewRendering = false;
    });
  } else {
    m_previewCancelRequested.store(true, std::memory_order_relaxed);
    if (m_previewThread.joinable())
      m_previewThread.join();

    m_previewRendering = false;
    m_btnPreview->setText("Preview");
  }
}

void GUI::onRenderButton() {
  if (!m_rendering) {
    if (!m_pixelBuffer.empty()) {
      m_pixelBuffer.clear();
      m_btnPreview->setText("Preview");
    }
    if (m_renderThread.joinable())
      m_renderThread.join();

    m_cancelRequested.store(false, std::memory_order_relaxed);
    m_rowsDone.store(0, std::memory_order_relaxed);
    m_rendering = true;
    m_btnRender->setText("Stop (0%)");

    m_pixelBuffer.assign(m_renderer.getWidth() * m_renderer.getHeight() * 4, 0);
    m_renderThread = std::thread([this]() {
      m_renderer.renderToBuffer(*m_scene, m_pixelBuffer, &m_cancelRequested,
                                &m_rowsDone);
      m_rendering = false;
    });
  } else {
    m_cancelRequested.store(true, std::memory_order_relaxed);
    if (m_renderThread.joinable())
      m_renderThread.join();

    m_rendering = false;
    m_btnRender->setText("Render");
  }
}

void GUI::onSaveButton() {
  if (m_pixelBuffer.empty())
    return;

  std::string outputFilePath =
      std::filesystem::path(m_sceneFile).stem().string() + ".ppm";

  std::ofstream outputFileStream(outputFilePath);
  if (!outputFileStream) {
    std::cerr << "Failed to open output file: " << outputFilePath << "\n";
    return;
  }

  std::size_t width = m_renderer.getWidth();
  std::size_t height = m_renderer.getHeight();

  outputFileStream << "P3\n" << width << " " << height << "\n255\n";

  for (std::size_t y = 0; y < height; ++y) {
    for (std::size_t x = 0; x < width; ++x) {
      std::size_t i = (y * width + x) * 4;
      outputFileStream << int(m_pixelBuffer[i]) << " "
                       << int(m_pixelBuffer[i + 1]) << " "
                       << int(m_pixelBuffer[i + 2]) << "\n";
    }
  }
}

void GUI::onToggleASS() {
  bool isEnabled = m_renderer.isAdaptiveSupersampling();
  m_renderer.setAdaptiveSupersampling(!isEnabled, 3, 15.0);
  if (!isEnabled) {
    m_btnToggleAASS->setText("Disable AA-SS");
  } else {
    m_btnToggleAASS->setText("Enable AA-SS");
  }
}
