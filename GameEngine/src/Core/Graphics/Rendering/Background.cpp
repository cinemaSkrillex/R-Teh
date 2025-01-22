/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** Background.cpp
*/

#include "../include/Core/Graphics/Rendering/Background.hpp"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace RealEngine {
SolidColorBackground::SolidColorBackground(float r, float g, float b, float a) {
    if (r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255 || a < 0 || a > 255) {
        std::cerr << "Color values must be between 0 and 255" << std::endl;
        return;
    }
    setPosition(0, 0);
    setSize(800, 600);
    _shape.setFillColor(sf::Color(r, g, b, a));
}

void SolidColorBackground::update(float deltaTime) {}

void SolidColorBackground::draw(sf::RenderTexture& window) { window.draw(_shape); }

void SolidColorBackground::setPosition(float x, float y) { _shape.setPosition(x, y); }

void SolidColorBackground::setSize(float x, float y) {
    if (x < 0 || y < 0) {
        std::cerr << "Size values must be greater than 0" << std::endl;
        return;
    }
    _shape.setSize(sf::Vector2f(x, y));
}

void SolidColorBackground::setScale(float x, float y) { _shape.setScale(x, y); }

void SolidColorBackground::setRotation(float angle) { _shape.setRotation(angle); }

void SolidColorBackground::setColor(float r, float g, float b, float a) {
    if (r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255 || a < 0 || a > 255) {
        std::cerr << "Color values must be between 0 and 255" << std::endl;
        return;
    }
    _shape.setFillColor(sf::Color(r, g, b, a));
}

ScrollingBackground::ScrollingBackground(sf::Image image) {
    _defaultPos = {-800 / 2, -600 / 2};
    _texture.loadFromImage(image);
    _texture.setSmooth(true);
    _texture.setRepeated(true);
    setSpriteRect(0, 0, 800 * 2, 600 * 2);
    _sprite.setTexture(_texture);
    _textureSize = {static_cast<float>(_texture.getSize().x),
                    static_cast<float>(_texture.getSize().y)};
    setSpeed(1);
    setDirection(45);
}

void ScrollingBackground::update(float deltaTime) {
    if (deltaTime < 0) {
        std::cerr << "Delta can't be negative" << std::endl;
        return;
    }
    _sprite.move(_speed * cos(_angle), _speed * sin(_angle));
    sf::Vector2f position = _sprite.getPosition();
    position.x += 800 / 2;
    position.y += 600 / 2;

    if (position.x <= -_textureSize.x || position.x >= _textureSize.x) reinitPosition(true, false);
    if (position.y <= -_textureSize.y || position.y >= _textureSize.y) reinitPosition(false, true);
}

void ScrollingBackground::draw(sf::RenderTexture& window) { window.draw(_sprite); }

void ScrollingBackground::setPosition(float x, float y) { _sprite.setPosition(x, y); }

void ScrollingBackground::setSize(float x, float y) {
    if (x == 0 || y == 0) return;
    float scaleFactorX = x / _sprite.getGlobalBounds().width;
    float scaleFactorY = y / _sprite.getGlobalBounds().height;
    setScale(scaleFactorX, scaleFactorY);
}

void ScrollingBackground::setScale(float x, float y) {
    setSpriteRect(0, 0, 800 * 2 * 1 / x, 600 * 2 * 1 / y);
    _sprite.setTexture(_texture);
    _textureSize.x = _texture.getSize().x * x;
    _textureSize.y = _texture.getSize().y * y;
    _sprite.setScale(x, y);
    reinitPosition(true, true);
}

void ScrollingBackground::setRotation(float angle) { _sprite.setRotation(angle); }

void ScrollingBackground::reinitPosition(bool horizontal, bool vertical) {
    setPosition(horizontal ? _defaultPos.x : _sprite.getPosition().x,
                vertical ? _defaultPos.y : _sprite.getPosition().y);
}

void ScrollingBackground::setSpriteRect(int posX, int posY, int sizeX, int sizeY) {
    _rect.left = posX;
    _rect.top  = posY;
    if (sizeX < 0 || sizeY < 0) {
        std::cerr << "Size values must be greater than 0" << std::endl;
        return;
    }
    _rect.width  = sizeX;
    _rect.height = sizeY;
    _sprite.setTextureRect(_rect);
}

void ScrollingBackground::setSpeed(float speed) {
    _speed = speed;
    reinitPosition(true, true);
}

void ScrollingBackground::setDirection(float angle) {
    _angle = angle * (M_PI / 180);
    reinitPosition(true, true);
}

void ScrollingBackground::setColor(float r, float g, float b, float a) {
    if (r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255 || a < 0 || a > 255) {
        std::cerr << "Color values must be between 0 and 255" << std::endl;
        return;
    }
    _sprite.setColor(sf::Color(r, g, b, a));
}

ImageBackground::ImageBackground(sf::Image image) {
    _texture.loadFromImage(image);
    _sprite.setTexture(_texture);
    _rect = _sprite.getTextureRect();
}

void ImageBackground::update(float deltaTime) {}

void ImageBackground::draw(sf::RenderTexture& window) { window.draw(_sprite); }

void ImageBackground::setPosition(float x, float y) { _sprite.setPosition(x, y); }

void ImageBackground::setSize(float x, float y) {
    if (x < 0 || y < 0) {
        std::cerr << "Size values must be greater than 0" << std::endl;
        return;
    }
    _rect.width  = x;
    _rect.height = y;
    _sprite.setTextureRect(_rect);
}

void ImageBackground::setScale(float x, float y) { _sprite.setScale(x, y); }

void ImageBackground::setSpriteRect(int posX, int posY, int sizeX, int sizeY) {
    _rect.left = posX;
    _rect.top  = posY;
    if (sizeX < 0 || sizeY < 0) {
        std::cerr << "Size values must be greater than 0" << std::endl;
        return;
    }
    _rect.width  = sizeX;
    _rect.height = sizeY;
    _sprite.setTextureRect(_rect);
}

void ImageBackground::setRotation(float angle) { _sprite.setRotation(angle); }

void ImageBackground::setColor(float r, float g, float b, float a) {
    if (r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255 || a < 0 || a > 255) {
        std::cerr << "Color values must be between 0 and 255" << std::endl;
        return;
    }
    _sprite.setColor(sf::Color(r, g, b, a));
}

}  // namespace RealEngine