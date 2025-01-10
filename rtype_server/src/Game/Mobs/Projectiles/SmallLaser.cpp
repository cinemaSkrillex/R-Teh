/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** SmallLaser
*/

#include "Game/Mobs/Projectiles/SmallLaser.hpp"

namespace rtype {

SmallLaser::SmallLaser(RealEngine::Registry& registry, sf::Vector2f position, float angle)
    : _entity(registry.spawn_entity()),
      _projSprite(*(RealEngine::AssetManager::getInstance().getSprite("small_laser"))) {
    _projSpriteSheet.emplace("normal", _projSprite);
    registry.add_component(_entity, RealEngine::Position{position.x, position.y});
    registry.add_component(_entity, RealEngine::Velocity{0, 0, {500.f, 500.f}, 0.f});
    registry.add_component(_entity, RealEngine::Acceleration{500.f, 500.f, 105.f});
    registry.add_component(
        _entity,
        RealEngine::SpriteSheet{
            _projSpriteSheet, "normal", 0, {48, 4}, false, true, 55, {-1, -1}, sf::Clock()});
    registry.add_component(_entity, RealEngine::Drawable{});
    registry.add_component(_entity,
                           RealEngine::Collision{{0.f, 0.f, 16.f * GAME_SCALE, 8.f * GAME_SCALE},
                                                 "mob",
                                                 false,
                                                 RealEngine::CollisionType::ENEMY_BULLET,
                                                 destroyOnWalls});
    registry.add_component(_entity, RealEngine::AI{noBehavior, goStraightConstantAngle, true});
    registry.add_component(_entity, RealEngine::Damage{10});
    registry.add_component(_entity, RealEngine::Health{1, 1});
    registry.add_component(_entity, RealEngine::Rotation{angle});
    registry.add_component(_entity, RealEngine::AutoDestructible{-1.0f, true, false});
    registry.add_component(
        _entity,
        RealEngine::NetvarContainer{
            {{"sprite_name", {"string", "sprite_name", std::string("small_laser"), nullptr}}

            }});
}

SmallLaser::~SmallLaser() {}

}  // namespace rtype