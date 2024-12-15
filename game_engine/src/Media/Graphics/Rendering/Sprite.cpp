#include "../include/Media/Graphics/Rendering/Sprite.hpp"

namespace RealEngine {

Sprite::Sprite(const std::shared_ptr<sf::Texture> texture) {
    if (!texture) {
        std::cerr << "Error: Null texture passed to Sprite constructor!" << std::endl;
        return;
    }
    _texture = texture;
    _sprite.setTexture(*texture);
    centerOrigin();
    setPosition(0, 0);
    setColor(sf::Color::White);
    _flipped = false;
}

Sprite::~Sprite() {}

void Sprite::loadImage(sf::Image image) {
    _texture->loadFromImage(image);
    _sprite.setTexture(*_texture);
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

void Sprite::setSmooth(bool smooth) { _texture->setSmooth(smooth); }

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

void Sprite::centerOrigin() {
    sf::FloatRect bounds = _sprite.getLocalBounds();
    _sprite.setOrigin(bounds.width / 2, bounds.height / 2);
}

void Sprite::setOrigin(float x, float y) { _sprite.setOrigin(x, y); }

void Sprite::scaleFromSize(const float width, const float height) {
    sf::Vector2u textureSize = _texture->getSize();

    float scaleX = width / static_cast<float>(textureSize.x);
    float scaleY = height / static_cast<float>(textureSize.y);
    _sprite.setScale(scaleX, scaleY);
}

bool Sprite::isTextureOfBounds() {
    sf::IntRect  rect = _sprite.getTextureRect();
    if (!_texture)
        std::cerr << "Error: Null texture in Sprite!" << std::endl;
    sf::Vector2u size = _texture->getSize();

    // std::cout << "Texture size: " << size.x << "x" << size.y << std::endl;
    // std::cout << "Texture rect: " << rect.left << "," << rect.top << "," << rect.width << ","
    //           << rect.height << std::endl;

    // Check if the texture coordinates go out of bounds
    if (rect.left < 0 || rect.top < 0 || rect.left + rect.width >= size.x ||
        rect.top + rect.height > size.y) {
        return true;
    }

    return false;
}

}  // namespace RealEngine