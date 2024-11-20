#include "../include/Media/Graphics/Rendering/Sprite.hpp"

namespace RealEngine {

Sprite::Sprite(const std::string filepath) {
    loadFile(filepath);
    _sprite.setOrigin(_sprite.getLocalBounds().width / 2, _sprite.getLocalBounds().height / 2);
    setPosition(0, 0);
    setColor(sf::Color::White);
    _flipped = false;
}

Sprite::~Sprite() {}

void Sprite::loadImage(sf::Image image) {
    _texture.loadFromImage(image);
    _sprite.setTexture(_texture);
}

void Sprite::draw(sf::RenderWindow& window) { window.draw(_sprite); }

void Sprite::disappear() { _sprite.setColor(sf::Color::Transparent); }

void Sprite::setOpacity(float opacity) {
    if (opacity > 255) {
        opacity = 255;
    } else if (opacity < 0) {
        opacity = 0;
    }
    sf::Color color = _sprite.getColor();
    color.a         = opacity;
    _sprite.setColor(color);
}

void Sprite::setPosition(float x, float y) { _sprite.setPosition(x, y); }

void Sprite::move(float x, float y) { _sprite.move(x, y); }

void Sprite::setScale(float x, float y) { _sprite.setScale(x, y); }

void Sprite::setRotation(float angle) { _sprite.setRotation(angle); }

void Sprite::rotate(float angle) { _sprite.rotate(angle); }

void Sprite::setTextureRect(int start_x, int start_y, int width, int height) {
    _sprite.setTextureRect(sf::IntRect(start_x, start_y, width, height));
}

void Sprite::setSmooth(bool smooth) { _texture.setSmooth(smooth); }

void Sprite::setColor(sf::Color color) { _sprite.setColor(color); }

void Sprite::flip(bool left) {
    sf::IntRect rect = _sprite.getTextureRect();

    if (_flipped && left) {
        rect.width = -rect.width;
        _flipped   = false;
    } else if (!_flipped && !left) {
        rect.width = abs(rect.width);
        _flipped   = true;
    }
    _sprite.setTextureRect(rect);
}

void Sprite::loadFile(const std::string filePath) {
    if (!_texture.loadFromFile(filePath)) {
        _texture.loadFromFile("../assets/missing_texture.png");
        _texture.setRepeated(true);
        // get the sprite hibox to be the size of the texture
        _sprite.setTextureRect(sf::IntRect(0, 0, _texture.getSize().x, _texture.getSize().y));
    }
    _sprite.setTexture(_texture);
}

void Sprite::colorize(sf::Color colorToReplace, sf::Color newColor) {
    for (unsigned int x = 0; x < _image.getSize().x; x++) {
        for (unsigned int y = 0; y < _image.getSize().y; y++) {
            if (_image.getPixel(x, y) == colorToReplace) {
                _image.setPixel(x, y, newColor);
            }
        }
    }
    _texture.loadFromImage(_image);
    _sprite.setTexture(_texture);
}

void Sprite::scaleFromSize(const float width, const float height) {
    sf::Vector2u textureSize = _texture.getSize();

    float scaleX = width / static_cast<float>(textureSize.x);
    float scaleY = height / static_cast<float>(textureSize.y);
    _sprite.setScale(scaleX, scaleY);
}
} // namespace RealEngine