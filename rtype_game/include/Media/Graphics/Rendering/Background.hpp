#pragma once

#include "../Render.hpp"
#include "Sprite.hpp"

namespace RealEngine {
class ABackground : public IRender {
   public:
    ABackground()                                = default;
    ~ABackground()                               = default;
    virtual void update(float deltaTime)         = 0;
    virtual void draw(sf::RenderTexture& window) = 0;
    virtual void setPosition(float x, float y)   = 0;
    virtual void setSize(float x, float y)       = 0;
    virtual void setScale(float x, float y)      = 0;
    virtual void setRotation(float angle)        = 0;
};

class SolidColorBackground : public ABackground {
   public:
    SolidColorBackground(float r, float g, float b, float a);
    void update(float deltaTime) = 0;
    void draw(sf::RenderTexture& window) override;
    void setPosition(float x, float y) override;
    void setSize(float x, float y) override;
    void setScale(float x, float y) override;
    void setRotation(float angle) override;
    void setColor(float r, float g, float b, float a);

   private:
    sf::RectangleShape _shape;
};

class ScrollingBackground : public ABackground {
   public:
    ScrollingBackground(sf::Image image);
    void update(float deltaTime) override;
    void draw(sf::RenderTexture& window) override;
    void setPosition(float x, float y) override;
    void setSize(float x, float y) override;
    void setScale(float x, float y) override;
    void setRotation(float angle) override;
    void reinitPosition(bool horizonal, bool vertical);
    void setSpriteRect(int posX, int posY, int sizeX, int sizeY);
    void setSpeed(float speed);
    void setDirection(float angle);
    void setColor(float r, float g, float b, float a);

   private:
    sf::Texture  _texture;
    sf::Sprite   _sprite;
    sf::IntRect  _rect;
    float        _speed;
    sf::Vector2f _position;
    sf::Vector2f _defaultPos;
    float        _angle;
    sf::Vector2f _textureSize;
};

class ImageBackground : public ABackground {
   public:
    ImageBackground(sf::Image image);
    void update(float deltaTime) = 0;
    void draw(sf::RenderTexture& window) override;
    void setPosition(float x, float y) override;
    void setSize(float x, float y) override;
    void setScale(float x, float y) override;
    void setSpriteRect(int posX, int posY, int sizeX, int sizeY);
    void setRotation(float angle) override;
    void setColor(float r, float g, float b, float a);

   private:
    sf::Texture _texture;
    sf::Sprite  _sprite;
    sf::IntRect _rect;
};

// Not correctly implemented
class ParallaxBackground : public ABackground {
   public:
    ParallaxBackground(sf::Image image, int layers);
    void update(float deltaTime) override;
    void draw(sf::RenderTexture& window) override;
    void setPosition(float x, float y) override;
    void setSize(float x, float y) override;
    void setScale(float x, float y) override;
    void setRotation(float angle) override;
    void setSpeed(int layer, float speed);
    void setDirection(float angle);

   private:
    sf::Texture         _texture;
    std::vector<Sprite> _sprites;
    std::vector<float>  _speeds;
    sf::Vector2f        _position;
    float               _angle;
};
}  // namespace RealEngine