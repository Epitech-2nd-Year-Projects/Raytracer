#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <functional>

namespace Raytracer::UI {

/**
 * @class Button
 * @brief A class representing a button UI component.
 */
class Button {
public:
  /**
   * @brief Constructor for the Button class.
   * @param font The font to be used for the button label.
   * @param text The text to be displayed on the button.
   * @param position The position of the button in the window.
   * @param size The size of the button.
   * @param onClick The callback function to be called when the button is
   * clicked.
   */
  Button(const sf::Font &font, const std::string &text,
         const sf::Vector2f &position, const sf::Vector2f &size,
         std::function<void()> onClick)
      : m_callback(onClick) {
    m_background.setSize(size);
    m_background.setPosition(position);
    m_background.setFillColor({70, 70, 70});
    m_background.setOutlineColor(sf::Color::White);
    m_background.setOutlineThickness(1);

    m_label.setFont(font);
    m_label.setString(text);
    m_label.setCharacterSize(14);
    m_label.setFillColor(sf::Color::White);

    auto labelBounds = m_label.getLocalBounds();
    m_label.setPosition(
        position.x + (size.x - labelBounds.width) * 0.5f - labelBounds.left,
        position.y + (size.y - labelBounds.height) * 0.5f - labelBounds.top);
  }

  /**
   * @brief Draw the button on the given render target.
   * @param renderTarger The render target to draw the button on.
   */
  void draw(sf::RenderTarget &renderTarger) {
    renderTarger.draw(m_background);
    renderTarger.draw(m_label);
  }

  /**
   * @brief Set the position of the button.
   * @param position The new position of the button.
   */
  void setPosition(const sf::Vector2f &position) {
    m_background.setPosition(position);

    auto labelBounds = m_label.getLocalBounds();
    m_label.setPosition(
        position.x + (m_background.getSize().x - labelBounds.width) * 0.5f -
            labelBounds.left,
        position.y + (m_background.getSize().y - labelBounds.height) * 0.5f -
            labelBounds.top);
  }

  /**
   * @brief Set the text of the button.
   * @param text The new text to be displayed on the button.
   */
  void setText(const std::string &text) {
    m_label.setString(text);

    auto pos = m_background.getPosition();
    auto size = m_background.getSize();
    auto bounds = m_label.getLocalBounds();
    m_label.setPosition(pos.x + (size.x - bounds.width) * 0.5f - bounds.left,
                        pos.y + (size.y - bounds.height) * 0.5f - bounds.top);
  }

  /**
   * @brief Handle the mouse button release event.
   * @param event The event to handle.
   * @param window The render window.
   */
  void handleEvent(const sf::Event &event, const sf::RenderWindow &window) {
    if (event.type == sf::Event::MouseButtonReleased &&
        event.mouseButton.button == sf::Mouse::Left) {

      auto mousePosition =
          window.mapPixelToCoords({event.mouseButton.x, event.mouseButton.y});
      if (m_background.getGlobalBounds().contains(mousePosition)) {
        m_callback();
      }
    }
  }

private:
  sf::RectangleShape m_background;
  sf::Text m_label;
  std::function<void()> m_callback;
};
} // namespace Raytracer::UI
