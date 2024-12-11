#pragma once

#include <iostream>

#include "../Render.hpp"

namespace RealEngine {
class Sprite : public IRender {
   public:
    Sprite() = default;
    Sprite(const std::string filePath);
    Sprite(const std::string filePath, sf::IntRect textureRect);
    ~Sprite();
    void          draw(sf::RenderWindow& window);
    void          loadFile(const std::string filePath);
    void          loadFile(const std::string filePath, const sf::IntRect textureRect);
    void          loadImage(sf::Image image);
    void          reloadSprite();
    void          disappear() override;
    void          setOpacity(float opacity);
    void          setPosition(float x, float y) override;
    void          move(float x, float y);
    void          setScale(float x, float y) override;
    void          setRotation(float angle) override;
    void          rotate(float angle);
    void          setTextureRect(int start_x, int start_y, int width, int height);
    void          setSmooth(bool smooth);
    void          setColor(sf::Color color);
    void          flip(bool invert);
    void          centerOrigin();
    void          setOrigin(float x, float y);
    sf::Sprite    getSprite() const { return _sprite; }
    sf::Image     getImage() const { return _image; }
    sf::Texture   getTexture() const { return _texture; }
    void          colorize(sf::Color colorToReplace, sf::Color newColor);
    void          scaleFromSize(const float width, const float height);
    bool          isTextureOfBounds();
    sf::FloatRect getBounds() { return _sprite.getGlobalBounds(); }

   private:
    sf::Sprite  _sprite;
    sf::Texture _texture;
    sf::Image   _image;
    bool        _flipped;
};

}  // namespace RealEngine