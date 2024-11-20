#include "Text.hpp"

namespace engine {

Text::Text(const std::string text, const std::string fontPath) {
    loadFile(fontPath);
    setString(text);
    center();
}

Text::~Text() {}

void Text::draw(sf::RenderWindow& window) { window.draw(_text); }

void Text::loadFile(const std::string filePath) {
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

void Text::setColor(float r, float g, float b, float a) {
    _text.setFillColor(sf::Color(r, g, b, a));
}

void Text::setBorderColor(float r, float g, float b, float a) {
    _text.setOutlineColor(sf::Color(r, g, b, a));
}

void Text::setBorderThickness(float thickness) { _text.setOutlineThickness(thickness); }

void Text::setString(const std::string text) {
    _text.setString(text);
    center();
}

void Text::setCharacterSize(int size) { _text.setCharacterSize(size); }
} // namespace engine