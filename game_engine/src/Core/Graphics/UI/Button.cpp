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
    : label(labelText, fontPath),
      _useSprite(false),
      _visible(true),
      _originalColor(sf::Color::White),
      _hoverColor(sf::Color::White) {
    box.setSize(size);
    box.setPosition(position);
    box.setFillColor(sf::Color::White);
    centerText();
}

Button::Button(Sprite spriteButton)
    : _visible(true),
      _useSprite(true),
      _originalColor(sf::Color::White),
      _hoverColor(sf::Color::White) {
    sprite = spriteButton;
    box.setSize({sprite.getBounds().width, sprite.getBounds().height});
    box.setPosition(sprite.getPosition().x - sprite.getBounds().width / 2,
                    sprite.getPosition().y - sprite.getBounds().height / 2);
}

Button::~Button() {}

void Button::draw(sf::RenderTexture& window) {
    if (_visible) {
        if (_useSprite) {
            sprite.draw(window);
        } else {
            window.draw(box);
            label.draw(window);
        }
    }
}

void Button::handleEvent(const sf::Event& event, std::function<void()> onClick,
                         const sf::RenderWindow& window) {
    if (event.type == sf::Event::MouseMoved || event.type == sf::Event::MouseButtonPressed) {
        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        if (box.getGlobalBounds().contains(mousePos)) {
            if (_useSprite) {
                sprite.setColor(_hoverColor);
            } else {
                box.setFillColor(_hoverColor);
            }
            if (event.type == sf::Event::MouseButtonPressed &&
                event.mouseButton.button == sf::Mouse::Left) {
                onClick();
            }
        } else {
            if (_useSprite) {
                sprite.setColor(_originalColor);
            } else {
                box.setFillColor(_originalColor);
            }
        }
    }
}

void Button::setPosition(const sf::Vector2f& position) {
    if (_useSprite) {
        sprite.setPosition(position.x, position.y);
        box.setPosition(sprite.getPosition().x - sprite.getBounds().width / 2,
                        sprite.getPosition().y - sprite.getBounds().height / 2);
    } else {
        box.setPosition(position);
    }
    centerText();
}

void Button::setSize(const sf::Vector2f& size) {
    if (_useSprite) {
        sprite.setScale(size.x, size.y);
        box.setSize({sprite.getBounds().width, sprite.getBounds().height});
    } else {
        box.setSize(size);
    }
    centerText();
}

void Button::setFillColor(const sf::Color& color) {
    _originalColor = color;
    box.setFillColor(color);
}

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
