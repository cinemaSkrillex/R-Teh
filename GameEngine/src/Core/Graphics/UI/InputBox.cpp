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
      cursorVisible(false),
      currentText(defaultText) {
    box.setSize(size);
    box.setPosition(position);
    box.setFillColor(sf::Color::White);

    text.setPosition(position.x + 5, position.y + (size.y - text.getHeight()) / 2);

    cursor.setSize(sf::Vector2f(2, text.getHeight()));
    cursor.setFillColor(sf::Color::White);
    updateCursorPosition();
}

void InputBox::draw(sf::RenderTexture& window) {
    window.draw(box);
    text.draw(window);
    if (isActive && cursorVisible) {
        window.draw(cursor);
    }
}

void InputBox::handleEvent(const sf::Event& event) {
    if (event.type == sf::Event::MouseButtonPressed) {
        isActive      = box.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y);
        cursorVisible = isActive;
        if (isActive) {
            cursorClock.restart();
        }
    }

    if (isActive && event.type == sf::Event::TextEntered) {
        if (isValidCharacter(event.text.unicode)) {
            if (event.text.unicode == '\b' && !currentText.empty()) {
                currentText.pop_back();
            } else if (currentText.length() < characterLimit && event.text.unicode != '\b') {
                currentText += static_cast<char>(event.text.unicode);
            }
            text.setString(currentText);
            updateCursorPosition();
        }
    }
}

void InputBox::updateCursorPosition() {
    cursor.setPosition(text.getPosition().x + text.getWidth(), text.getPosition().y);
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
    updateCursorPosition();
}

void InputBox::setSize(const sf::Vector2f& size) {
    box.setSize(size);
    updateCursorPosition();
}

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
    updateCursorPosition();
}

void InputBox::centerText() {
    // No center No on the left i want on the right
    text.setPosition(box.getPosition().x + box.getSize().x - text.getWidth() - 5,
                     box.getPosition().y + (box.getSize().y - text.getHeight()) / 2);

    updateCursorPosition();
}

void InputBox::setFocus(bool focus) {
    isActive      = focus;
    cursorVisible = focus;

    if (focus) {
        box.setOutlineThickness(2);
        box.setOutlineColor(sf::Color::Blue);
        cursorClock.restart();
    } else {
        box.setOutlineThickness(0);
    }
}

sf::FloatRect InputBox::getGlobalBounds() const { return box.getGlobalBounds(); }

void InputBox::setContentType(ContentType type) { contentType = type; }

void InputBox::drawDebug(sf::RenderTexture& window) {
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
