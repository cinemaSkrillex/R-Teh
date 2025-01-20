#pragma once

#include <SFML/Graphics.hpp>

namespace rtype {
class LauncherBackground {
   public:
    LauncherBackground(const std::string& texturePath, float speed);
    ~LauncherBackground();

    void update(float deltaTime);
    void draw(sf::RenderTarget& target);

   private:
    sf::Texture _texture;
    sf::Sprite  _sprite1;
    sf::Sprite  _sprite2;
    float       _speed;
    float       _offset;
};
}  // namespace rtype
