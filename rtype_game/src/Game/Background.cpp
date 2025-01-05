/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** Background
*/

#include "Game/Background.hpp"

namespace rtype {
Background::Background(RealEngine::Registry& registry, float speed, int scale)
    : _entity(registry.spawn_entity()),
      _backgroundSprite(*(RealEngine::AssetManager::getInstance().getSprite("stars_background"))) {
    registry.add_component(_entity, RealEngine::Position{0.f, 0.f});
    _backgroundSprite.setScale(scale, scale);
    _backgroundSprite.setOrigin(0, 0.5f);
    registry.add_component(_entity, RealEngine::SpriteComponent{_backgroundSprite});
    registry.add_component(_entity, RealEngine::Parallax{speed, sf::Vector2f(800.0f, 600.0f)});
    registry.add_component(_entity, RealEngine::Drawable{});
}

Background::~Background() {}

}  // namespace rtype