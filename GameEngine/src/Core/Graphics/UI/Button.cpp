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
    : _label(labelText, fontPath) {
    _box.setSize(size);
    _box.setPosition(position);
    _box.setFillColor(sf::Color::White);
    centerText();
}

void Button::draw(sf::RenderTexture& window) {
    if (!_visible) {
        window.draw(_box);
        _label.draw(window);
    }
}

void Button::handleEvent(const sf::Event& event, std::function<void()> onClick) {
    if (event.type == sf::Event::MouseButtonPressed) {
        if (_box.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y) &&
            event.mouseButton.button == sf::Mouse::Left) {
            onClick();
        }
    }
}

void Button::setPosition(const sf::Vector2f& position) {
    if (position.x < 0 || position.y < 0) {
        std::cerr << "Button position cannot be negative" << std::endl;
        return;
    }
    _box.setPosition(position);
    centerText();
}

void Button::setSize(const sf::Vector2f& size) {
    if (size.x < 0 || size.y < 0) {
        std::cerr << "Button size cannot be negative" << std::endl;
        return;
    }
    _box.setSize(size);
    centerText();
}

void Button::setFillColor(const sf::Color& color) { _box.setFillColor(color); }

void Button::setTextColor(const sf::Color& color) {
    _label.setColor(color.r, color.g, color.b, color.a);
}

void Button::setLabel(const std::string& labelText) {
    if (labelText.empty()) {
        std::cerr << "Button label cannot be empty" << std::endl;
        return;
    }
    _label.setString(labelText);
    centerText();
}

void Button::centerText() {
    sf::FloatRect textBounds = _label.getLocalBounds();
    float         textPositionX =
        _box.getPosition().x + (_box.getSize().x - textBounds.width) / 2 - textBounds.left;
    float textPositionY =
        _box.getPosition().y + (_box.getSize().y - textBounds.height) / 2 - textBounds.top;
    if (textPositionX < 0 || textPositionY < 0) {
        std::cerr << "Button text position cannot be negative" << std::endl;
        return;
    }
    _label.setPosition((textPositionX + 132), (textPositionY + 10));
}

}  // namespace RealEngine
