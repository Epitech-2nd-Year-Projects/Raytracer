#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

namespace Raytracer::UI {
/**
 * @class Slider
 * @brief A class representing a slider UI component.
 */
class Slider {
public:
  /**
   * @brief Constructor for the Slider class.
   * @param font The font to be used for the slider label.
   * @param txt The text to be displayed on the slider.
   * @param pos The position of the slider in the window.
   * @param length The length of the slider track.
   * @param minValue The minimum value of the slider.
   * @param maxValue The maximum value of the slider.
   * @param initial The initial value of the slider.
   */
  Slider(const sf::Font &font, const std::string &text,
         const sf::Vector2f &position, float length, float minValue,
         float maxValue, float initial)
      : m_minValue(minValue), m_maxValue(maxValue), m_currentValue(initial) {
    m_label.setFont(font);
    m_label.setString(text);
    m_label.setCharacterSize(12);
    m_label.setFillColor(sf::Color::White);
    m_label.setPosition(position);

    m_track.setPosition(position.x, position.y + 20.f);
    m_track.setSize({length, 4.f});
    m_track.setFillColor({150, 150, 150});

    m_knob.setRadius(8.f);
    m_knob.setFillColor({200, 200, 200});
    m_knob.setOrigin({m_knob.getRadius(), m_knob.getRadius()});

    updateKnob();
  }

  /**
   * @brief Draw the slider on the given render target.
   * @param renderTarget The render target to draw the slider on.
   */
  void draw(sf::RenderTarget &renderTarget) const {
    renderTarget.draw(m_label);
    renderTarget.draw(m_track);
    renderTarget.draw(m_knob);
  }

  /**
   * @brief Set the position of the slider.
   * @param position The new position of the slider.
   */
  void setPosition(const sf::Vector2f &position) {
    m_label.setPosition(position);
    m_track.setPosition(position.x, position.y + 20.f);
    updateKnob();
  }

  /**
   * @brief Handle events for the slider.
   * @param ev The event to handle.
   * @param win The render window.
   */
  void handleEvent(const sf::Event &event, const sf::RenderWindow &window) {
    sf::Vector2f mousePosition;
    if (event.type == sf::Event::MouseButtonPressed ||
        event.type == sf::Event::MouseButtonReleased) {
      mousePosition =
          window.mapPixelToCoords({event.mouseButton.x, event.mouseButton.y});
    } else if (event.type == sf::Event::MouseMoved) {
      mousePosition =
          window.mapPixelToCoords({event.mouseMove.x, event.mouseMove.y});
    } else {
      return;
    }

    if (event.type == sf::Event::MouseButtonPressed &&
        event.mouseButton.button == sf::Mouse::Left &&
        m_knob.getGlobalBounds().contains(mousePosition)) {
      m_Dragging = true;
    }
    if (event.type == sf::Event::MouseButtonReleased &&
        event.mouseButton.button == sf::Mouse::Left) {
      m_Dragging = false;
    }
    if (event.type == sf::Event::MouseMoved && m_Dragging) {
      float x = mousePosition.x;
      auto trackerPosition = m_track.getPosition();
      float left = trackerPosition.x;
      float right = trackerPosition.x + m_track.getSize().x;
      x = std::max(left, std::min(x, right));

      m_knob.setPosition({x, trackerPosition.y + 2.f});

      float t = (x - left) / m_track.getSize().x;
      m_currentValue = m_minValue + t * (m_maxValue - m_minValue);
    }
  }

  /**
   * @brief Get the current value of the slider.
   * @return The current value of the slider.
   */
  [[nodiscard]]
  float getValue() const {
    return m_currentValue;
  }

private:
  /**
   * @brief Update the position of the knob based on the current value.
   */
  void updateKnob() {
    auto trackerPosition = m_track.getPosition();
    float tracker = (m_currentValue - m_minValue) / (m_maxValue - m_minValue);

    m_knob.setPosition({trackerPosition.x + tracker * m_track.getSize().x,
                        trackerPosition.y + 2.f});
  }

private:
  sf::RectangleShape m_track;
  sf::Text m_label;
  sf::CircleShape m_knob;
  float m_minValue;
  float m_maxValue;
  float m_currentValue;
  bool m_Dragging = false;
};
} // namespace Raytracer::UI
