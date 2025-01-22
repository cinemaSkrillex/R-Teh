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
    : _text(defaultText, fontPath),
      _contentType(_contentType),
      _characterLimit(_characterLimit),
      _isActive(false),
      _cursorVisible(false),
      _currentText(defaultText) {
    _box.setSize(size);
    _box.setPosition(position);
    _box.setFillColor(sf::Color::White);

    _text.setPosition(position.x + 5, position.y + (size.y - _text.getHeight()) / 2);

    _cursor.setSize(sf::Vector2f(2, _text.getHeight()));
    _cursor.setFillColor(sf::Color::White);
    updateCursorPosition();
}

void InputBox::draw(sf::RenderTexture& window) {
    window.draw(_box);
    _text.draw(window);
    if (_isActive && _cursorVisible) {
        window.draw(_cursor);
    }
}

void InputBox::handleEvent(const sf::Event& event) {
    if (event.type == sf::Event::MouseButtonPressed) {
        _isActive      = _box.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y);
        _cursorVisible = _isActive;
        if (_isActive) {
            _cursorClock.restart();
        }
    }

    if (_isActive && event.type == sf::Event::TextEntered) {
        if (isValidCharacter(event.text.unicode)) {
            if (event.text.unicode == '\b' && !_currentText.empty()) {
                _currentText.pop_back();
            } else if (_currentText.length() < _characterLimit && event.text.unicode != '\b') {
                _currentText += static_cast<char>(event.text.unicode);
            }
            _text.setString(_currentText);
            updateCursorPosition();
        }
    }
}

void InputBox::updateCursorPosition() {
    _cursor.setPosition(_text.getPosition().x + _text.getWidth(), _text.getPosition().y);
}

bool InputBox::isValidCharacter(uint32_t unicode) const {
    if (_currentText.length() >= _characterLimit && unicode != '\b') {
        return false;  // Bloque l'ajout si la limite est atteinte
    }

    switch (_contentType) {
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
    _box.setPosition(position);
    _text.setPosition(position.x + 5, position.y + 5);
    updateCursorPosition();
}

void InputBox::setSize(const sf::Vector2f& size) {
    _box.setSize(size);
    updateCursorPosition();
}

void InputBox::setFillColor(const sf::Color& color) { _box.setFillColor(color); }

void InputBox::setTextColor(const sf::Color& color) {
    _text.setColor(color.r, color.g, color.b, color.a);
}

void InputBox::setCharacterLimit(size_t limit) { _characterLimit = limit; }

std::string InputBox::getText() const { return _currentText; }

void InputBox::setText(const std::string& text) {
    _currentText = text;
    this->_text.setString(text);
    centerText();
    updateCursorPosition();
}

void InputBox::centerText() {
    // No center No on the left i want on the right
    _text.setPosition(_box.getPosition().x + _box.getSize().x - _text.getWidth() - 5,
                      _box.getPosition().y + (_box.getSize().y - _text.getHeight()) / 2);

    updateCursorPosition();
}

void InputBox::setFocus(bool focus) {
    _isActive      = focus;
    _cursorVisible = focus;

    if (focus) {
        _box.setOutlineThickness(2);
        _box.setOutlineColor(sf::Color::Blue);
        _cursorClock.restart();
    } else {
        _box.setOutlineThickness(0);
    }
}

sf::FloatRect InputBox::getGlobalBounds() const { return _box.getGlobalBounds(); }

void InputBox::setContentType(ContentType type) { _contentType = type; }

void InputBox::drawDebug(sf::RenderTexture& window) {
    sf::FloatRect bounds = _text.getLocalBounds();

    sf::RectangleShape debugRect(sf::Vector2f(bounds.width, bounds.height));
    debugRect.setPosition(_text.getPosition().x + bounds.left, _text.getPosition().y + bounds.top);
    debugRect.setOutlineColor(sf::Color::Red);
    debugRect.setOutlineThickness(1);
    debugRect.setFillColor(sf::Color::Transparent);
    window.draw(debugRect);
    sf::RectangleShape boxDebug(_box.getSize());
    boxDebug.setPosition(_box.getPosition());
    boxDebug.setOutlineColor(sf::Color::Yellow);
    boxDebug.setOutlineThickness(1);
    boxDebug.setFillColor(sf::Color::Transparent);
    window.draw(boxDebug);
}
}  // namespace RealEngine
