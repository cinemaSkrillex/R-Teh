/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** EyeLaser
*/

#include "Game/Mobs/Projectiles/EyeLaser.hpp"

namespace rtype {

EyeLaser::EyeLaser(RealEngine::Registry& registry, sf::Vector2f position, float angle, float speed)
    : _entity(registry.spawn_entity()),
      _projSprite(*(RealEngine::AssetManager::getInstance().getSprite("eye_laser"))) {
    _projSpriteSheet.emplace("normal", _projSprite);
    registry.add_component(_entity, RealEngine::Position{position.x, position.y});
    registry.add_component(_entity, RealEngine::Velocity{0, 0, {850.f, 850.f}, 0.f});
    registry.add_component(_entity, RealEngine::Acceleration{speed, speed, speed});
    registry.add_component(
        _entity,
        RealEngine::SpriteSheet{
            _projSpriteSheet, "normal", 0, {32, 32}, false, true, 55, {16, 16}, sf::Clock()});
    registry.add_component(_entity, RealEngine::Drawable{});
    registry.add_component(_entity,
                           RealEngine::Collision{{0.f, 0.f, 16.f * GAME_SCALE, 8.f * GAME_SCALE},
                                                 "enemy_bullet",
                                                 false,
                                                 RealEngine::CollisionType::ENEMY_BULLET,
                                                 destroyOnWallsAndPlayer});
    registry.add_component(_entity, RealEngine::AI{noBehavior, goStraightAngle, true});
    registry.add_component(_entity, RealEngine::Damage{10});
    registry.add_component(_entity, RealEngine::Health{1, 1});
    registry.add_component(_entity, RealEngine::Rotation{angle});
}

EyeLaser::~EyeLaser() {}

}  // namespace rtype