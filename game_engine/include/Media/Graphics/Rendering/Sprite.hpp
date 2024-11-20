#pragma once

#include "../Render.hpp"
#include <iostream>

namespace RealEngine {
class Sprite : public IRender {
  public:
    Sprite(const std::string filePath);
    ~Sprite();
    void       draw(sf::RenderWindow& window);
    void       loadImage(sf::Image image);
    void       reloadSprite();
    void       disappear() override;
    void       setOpacity(float opacity);
    void       setPosition(float x, float y) override;
    void       move(float x, float y);
    void       setScale(float x, float y) override;
    void       setRotation(float angle) override;
    void       rotate(float angle);
    void       setTextureRect(int start_x, int start_y, int width, int height);
    void       setSmooth(bool smooth);
    void       setColor(sf::Color color);
    void       flip(bool invert);
    sf::Sprite getSprite() const { return _sprite; }
    sf::Image  getImage() const { return _image; }
    void       loadFile(const std::string filePath);
    void       colorize(sf::Color colorToReplace, sf::Color newColor);
    void       scaleFromSize(const float width, const float height);

  private:
    sf::Sprite  _sprite;
    sf::Texture _texture;
    sf::Image   _image;
    bool        _flipped;
};

} // namespace RealEngine