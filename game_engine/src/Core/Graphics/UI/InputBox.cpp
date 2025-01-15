/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** InputBox
*/

#include "../include/Core/Graphics/UI/InputBox.hpp"

#include <iostream>

namespace RealEngine {

InputBox::InputBox(const sf::Vector2f& size, const sf::Vector2f& position,
                   const std::string& defaultText, const std::string& fontPath,
                   ContentType contentType, size_t characterLimit)
    : text(defaultText, fontPath),
      contentType(contentType),
      characterLimit(characterLimit),
      isActive(false),
      currentText(defaultText) {
    box.setSize(size);
    box.setPosition(position);
    box.setFillColor(sf::Color::White);
    text.setPosition(position.x + 10000, position.y + (size.y / 2));
}

void InputBox::draw(sf::RenderWindow& window) {
    window.draw(box);
    text.draw(window);
}

void InputBox::handleEvent(const sf::Event& event) {
    if (event.type == sf::Event::MouseButtonPressed) {
        isActive = box.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y);
    }

    if (isActive && event.type == sf::Event::TextEntered) {
        if (isValidCharacter(event.text.unicode)) {
            if (event.text.unicode == '\b' && !currentText.empty()) {
                currentText.pop_back();
            } else if (currentText.length() < characterLimit && event.text.unicode != '\b') {
                currentText += static_cast<char>(event.text.unicode);
            }
            text.setString(currentText);
        }
    }
}

bool InputBox::isValidCharacter(uint32_t unicode) const {
    if (currentText.length() >= characterLimit && unicode != '\b') {
        return false;  // Bloque l'ajout si la limite est atteinte
    }

    switch (contentType) {
        case ContentType::Numeric:
            return (unicode >= '0' && unicode <= '9') || unicode == '\b';
        case ContentType::Text:
            return (unicode >= 32 && unicode <= 126) || unicode == '\b';
        case ContentType::Alphanumeric:
            return ((unicode >= '0' && unicode <= '9') || (unicode >= 'A' && unicode <= 'Z') ||
                    (unicode >= 'a' && unicode <= 'z') || unicode == '\b');
        case ContentType::IpAddress:
            return (unicode >= '0' && unicode <= '9') || unicode == '.' || unicode == '\b';
    }
    return false;
}

void InputBox::setPosition(const sf::Vector2f& position) {
    box.setPosition(position);
    text.setPosition(position.x + 5, position.y + 5);
}

void InputBox::setSize(const sf::Vector2f& size) { box.setSize(size); }

void InputBox::setFillColor(const sf::Color& color) { box.setFillColor(color); }

void InputBox::setTextColor(const sf::Color& color) {
    text.setColor(color.r, color.g, color.b, color.a);
}

void InputBox::setCharacterLimit(size_t limit) { characterLimit = limit; }

std::string InputBox::getText() const { return currentText; }

void InputBox::setText(const std::string& text) {
    currentText = text;
    this->text.setString(text);
    centerText();
}

void InputBox::centerText() {
    sf::FloatRect textBounds = text.getLocalBounds();
    float         textPositionX =
        box.getPosition().x + (box.getSize().x - textBounds.width) / 2 - textBounds.left;
    float textPositionY =
        box.getPosition().y + (box.getSize().y - textBounds.height) / 2 - textBounds.top;
    text.setPosition((textPositionX + 50), (textPositionY + 10));
}

void InputBox::setFocus(bool focus) {
    isActive = focus;

    if (focus) {
        box.setOutlineThickness(2);
        box.setOutlineColor(sf::Color::Blue);
    } else {
        box.setOutlineThickness(0);
    }
}

sf::FloatRect InputBox::getGlobalBounds() const { return box.getGlobalBounds(); }

void InputBox::setContentType(ContentType type) { contentType = type; }

void InputBox::drawDebug(sf::RenderWindow& window) {
    sf::FloatRect bounds = text.getLocalBounds();

    sf::RectangleShape debugRect(sf::Vector2f(bounds.width, bounds.height));
    debugRect.setPosition(text.getPosition().x + bounds.left, text.getPosition().y + bounds.top);
    debugRect.setOutlineColor(sf::Color::Red);
    debugRect.setOutlineThickness(1);
    debugRect.setFillColor(sf::Color::Transparent);
    window.draw(debugRect);
    sf::RectangleShape boxDebug(box.getSize());
    boxDebug.setPosition(box.getPosition());
    boxDebug.setOutlineColor(sf::Color::Yellow);
    boxDebug.setOutlineThickness(1);
    boxDebug.setFillColor(sf::Color::Transparent);
    window.draw(boxDebug);
}
}  // namespace RealEngine
