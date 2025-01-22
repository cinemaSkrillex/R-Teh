/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** InputBox
*/

#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>

#include "Text.hpp"

namespace RealEngine {
class InputBox {
   public:
    enum class ContentType { Text, Numeric, Alphanumeric, IpAddress };

    InputBox(const sf::Vector2f& size, const sf::Vector2f& position, const std::string& defaultText,
             const std::string& fontPath, ContentType contentType = ContentType::Text,
             size_t characterLimit = 256);

    void          draw(sf::RenderTexture& window);
    void          handleEvent(const sf::Event& event);
    void          setPosition(const sf::Vector2f& position);
    void          setSize(const sf::Vector2f& size);
    void          setFillColor(const sf::Color& color);
    void          setTextColor(const sf::Color& color);
    void          setCharacterLimit(size_t limit);
    std::string   getText() const;
    void          setText(const std::string& text);
    void          setContentType(ContentType type);
    void          centerText();
    void          drawDebug(sf::RenderTexture& window);
    void          setFocus(bool focus);
    void          updateCursorPosition();
    sf::FloatRect getGlobalBounds() const;

   private:
    sf::RectangleShape _box;
    Text               _text;
    ContentType        _contentType;
    size_t             _characterLimit;
    bool               _isActive;
    std::string        _currentText;
    sf::RectangleShape _cursor;
    sf::Clock          _cursorClock;
    bool               _cursorVisible;

    bool isValidCharacter(uint32_t unicode) const;
};
}  // namespace RealEngine