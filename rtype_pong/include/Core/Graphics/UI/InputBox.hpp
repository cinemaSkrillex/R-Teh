/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** InputBox
*/

#pragma once

#include <SFML/Graphics.hpp>
#include "Text.hpp"

namespace RealEngine {
class InputBox {
   public:
    enum class ContentType { Text, Numeric, Alphanumeric };

    InputBox(const sf::Vector2f& size, const sf::Vector2f& position, const std::string& defaultText,
             const std::string& fontPath, ContentType contentType = ContentType::Text);

    void        draw(sf::RenderWindow& window);
    void        handleEvent(const sf::Event& event);
    void        setPosition(const sf::Vector2f& position);
    void        setSize(const sf::Vector2f& size);
    void        setFillColor(const sf::Color& color);
    void        setTextColor(const sf::Color& color);
    void        setCharacterLimit(size_t limit);
    std::string getText() const;
    void        setText(const std::string& text);
    void        setContentType(ContentType type);
    void        centerText();
    void        drawDebug(sf::RenderWindow& window);
    void        setFocus(bool focus);
    sf::FloatRect getGlobalBounds() const;

   private:
    sf::RectangleShape box;
    Text               text;
    ContentType        contentType;
    size_t             characterLimit;
    bool               isActive;
    std::string        currentText;

    bool isValidCharacter(uint32_t unicode) const;
};
}  // namespace RealEngine
