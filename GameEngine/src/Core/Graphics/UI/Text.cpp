/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** Text
*/

#include "../include/Core/Graphics/UI/Text.hpp"

#include <iostream>
namespace RealEngine {

Text::Text(const std::string text, const std::string fontPath) {
    if (!fontPath.empty()) loadFont(fontPath);
    setString(text);
    center();
}

Text::Text(const std::string text, sf::Font font) {
    _text.setFont(font);
    setString(text);
    center();
}

Text::~Text() {}

void Text::draw(sf::RenderTexture& window) { window.draw(_text); }

void Text::loadFont(const std::string filePath) {
    if (filePath.empty()) return;
    _font.loadFromFile(filePath);
    _text.setFont(_font);
}

void Text::disappear() { _text.setFillColor(sf::Color::Transparent); }

void Text::setPosition(float x, float y) { _text.setPosition(x, y); }

void Text::move(float x, float y) { _text.move(x, y); }

void Text::setScale(float x, float y) {
    _text.setScale(x, y);
    center();
}

void Text::setRotation(float angle) { _text.setRotation(angle); }

void Text::rotate(float angle) { _text.rotate(angle); }

void Text::center() {
    _text.setOrigin(_text.getGlobalBounds().width / 2, _text.getGlobalBounds().height / 2);
}

void Text::setFont(sf::Font font) {
    _font = font;
    _text.setFont(_font);
    center();
}

void Text::setOpacity(int opacity) {
    if (opacity < 0 || opacity > 255) {
        std::cerr << "Opacity must be between 0 and 255" << std::endl;
        return;
    }
    sf::Color color = _text.getFillColor();
    color.a         = opacity;
    _text.setFillColor(color);
    color   = _text.getOutlineColor();
    color.a = opacity;
    _text.setOutlineColor(color);
}

void Text::setColor(float r, float g, float b, float a) {
    if (r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255 || a < 0 || a > 255) {
        std::cerr << "Color must be between 0 and 255" << std::endl;
        return;
    }
    _text.setFillColor(sf::Color(r, g, b, a));
}

void Text::setBorderColor(float r, float g, float b, float a) {
    if (r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255 || a < 0 || a > 255) {
        std::cerr << "Color must be between 0 and 255" << std::endl;
        return;
    }
    _text.setOutlineColor(sf::Color(r, g, b, a));
}

void Text::setBorderThickness(float thickness) {
    if (thickness < 0) {
        std::cerr << "Thickness must be positive" << std::endl;
        return;
    }
    _text.setOutlineThickness(thickness);
}

void Text::setString(const std::string text) {
    _text.setString(text);
    center();
}

sf::Vector2f Text::getPosition() const { return _text.getPosition(); }

void Text::setCharacterSize(int size) {
    if (size < 0) {
        std::cerr << "Size must be positive" << std::endl;
        return;
    }
    _text.setCharacterSize(size);
}
}  // namespace RealEngine