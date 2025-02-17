/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** Bouton
*/

#pragma once

#include <SFML/Graphics.hpp>
#include <functional>
#include <iostream>

#include "Text.hpp"

namespace RealEngine {
class Button {
   public:
    Button(const sf::Vector2f& size, const sf::Vector2f& position, const std::string& labelText,
           const std::string& fontPath);

    void draw(sf::RenderTexture& window);
    void handleEvent(const sf::Event& event, std::function<void()> onClick);
    void setPosition(const sf::Vector2f& position);
    void setSize(const sf::Vector2f& size);
    void setFillColor(const sf::Color& color);
    void setTextColor(const sf::Color& color);
    void setLabel(const std::string& labelText);
    void setVisible(bool visible) { _visible = visible; }
    bool isVisible() const { return _visible; }
    void centerText();

   private:
    sf::RectangleShape _box;
    Text               _label;
    bool               _visible;
};
}  // namespace RealEngine
