/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** Button
*/

#include "../include/Core/Graphics/UI/Button.hpp"

namespace RealEngine {

Button::Button(const sf::Vector2f& size, const sf::Vector2f& position, const std::string& labelText,
               const std::string& fontPath)
    : label(labelText, fontPath) {
    box.setSize(size);
    box.setPosition(position);
    box.setFillColor(sf::Color::White);
    centerText();
}

void Button::draw(sf::RenderWindow& window) {
    if (!_visible) {
        window.draw(box);
        label.draw(window);
    }
}

void Button::handleEvent(const sf::Event& event, std::function<void()> onClick) {
    if (event.type == sf::Event::MouseButtonPressed) {
        if (box.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y) &&
            event.mouseButton.button == sf::Mouse::Left) {
            onClick();
        }
    }
}

void Button::setPosition(const sf::Vector2f& position) {
    box.setPosition(position);
    centerText();
}

void Button::setSize(const sf::Vector2f& size) {
    box.setSize(size);
    centerText();
}

void Button::setFillColor(const sf::Color& color) { box.setFillColor(color); }

void Button::setTextColor(const sf::Color& color) {
    label.setColor(color.r, color.g, color.b, color.a);
}

void Button::setLabel(const std::string& labelText) {
    label.setString(labelText);
    centerText();
}

void Button::centerText() {
    sf::FloatRect textBounds = label.getLocalBounds();
    float         textPositionX =
        box.getPosition().x + (box.getSize().x - textBounds.width) / 2 - textBounds.left;
    float textPositionY =
        box.getPosition().y + (box.getSize().y - textBounds.height) / 2 - textBounds.top;
    label.setPosition((textPositionX + 132), (textPositionY + 10));
}

}  // namespace RealEngine
