#pragma once

#include "../Render.hpp"

namespace engine {
class Sprite : public IRender {
  public:
    Sprite(sf::Image image);
    ~Sprite();
    void draw(sf::RenderWindow& window);
    void loadImage(sf::Image image);
    void reloadSprite();
    void disappear() override;
    void setPosition(float x, float y) override;
    void move(float x, float y);
    void setScale(float x, float y) override;
    void setRotation(float angle) override;
    void rotate(float angle);
    void setTextureRect(int start_x, int start_y, int width, int height);
    void setSmooth(bool smooth);
    void setColor(sf::Color color);
    void flip(bool invert);

  private:
    sf::Sprite  _sprite;
    sf::Texture _texture;
    bool        _flipped;
};

// class Image {
//   public:
//     Image(const std::string filePath);
//     ~Image();
//     sf::Image getImage() { return _image; }
//     void      loadFile(const std::string filePath);
//     void      colorize(sf::Color colorToReplace, sf::Color newColor);
//     void      flipImage(bool horizontal, bool vertical);

//   private:
//     sf::Image _image;
// };
} // namespace engine