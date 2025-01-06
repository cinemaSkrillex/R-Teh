/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** SpaceLaser
*/

#include "Game/Mobs/Projectiles/SpaceLaser.hpp"

namespace rtype {

SpaceLaser::SpaceLaser(RealEngine::Registry& registry, sf::Vector2f position, float angle)
    : _entity(registry.spawn_entity()),
      _projSprite(*(RealEngine::AssetManager::getInstance().getSprite("space_laser"))) {
    _projSpriteSheet.emplace("normal", _projSprite);
    registry.add_component(_entity, RealEngine::Position{position.x, position.y});
    registry.add_component(_entity, RealEngine::Velocity{0, 0, {340.f, 340.f}, 0.f});
    registry.add_component(_entity, RealEngine::Acceleration{340.f, 340.f, 105.f});
    registry.add_component(
        _entity,
        RealEngine::SpriteSheet{
            _projSpriteSheet, "normal", 0, {48, 4}, false, true, 55, {-1, -1}, sf::Clock()});
    registry.add_component(_entity, RealEngine::Drawable{});

    registry.add_component(_entity, RealEngine::AI{noBehavior, goStraightAngle, true});
    registry.add_component(_entity, RealEngine::Damage{15});
    registry.add_component(_entity, RealEngine::Health{1, 1});
    registry.add_component(_entity, RealEngine::Rotation{angle});
}

SpaceLaser::~SpaceLaser() {}

}  // namespace rtype