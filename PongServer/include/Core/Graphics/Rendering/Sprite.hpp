#pragma once

#include <iostream>
#include <memory>

#include "../Render.hpp"

namespace RealEngine {
class Sprite : public IRender {
   public:
    Sprite() = default;
    Sprite(const std::shared_ptr<sf::Texture> texture);
    Sprite(const std::shared_ptr<sf::Texture>, sf::IntRect textureRect);
    ~Sprite();
    void                         draw(sf::RenderTexture& window);
    void                         loadFile(const std::string filePath);
    void                         loadImage(sf::Image image);
    void                         reloadSprite();
    void                         disappear() override;
    void                         setOpacity(float opacity);
    void                         setPosition(float x, float y) override;
    void                         move(float x, float y);
    void                         setScale(float x, float y) override;
    void                         setRotation(float angle) override;
    void                         rotate(float angle);
    void                         setTextureRect(int startX, int startY, int width, int height);
    void                         setSmooth(bool smooth);
    void                         setColor(sf::Color color);
    void                         flip(bool invert);
    void                         centerOrigin();
    void                         setOrigin(float x, float y);
    sf::Sprite                   getSprite() const { return _sprite; }
    sf::Image                    getImage() const { return _image; }
    std::shared_ptr<sf::Texture> getTexture() const { return _texture; }
    void                         scaleFromSize(const float width, const float height);
    bool                         isTextureOfBounds();
    sf::FloatRect                getBounds() { return _sprite.getGlobalBounds(); }
    float                        getOpacity() { return _sprite.getColor().a; }

   private:
    sf::Sprite                   _sprite;
    std::shared_ptr<sf::Texture> _texture;
    sf::Image                    _image;
    bool                         _flipped;
};

}  // namespace RealEngine