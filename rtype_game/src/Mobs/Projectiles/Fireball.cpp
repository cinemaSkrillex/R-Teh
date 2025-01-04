/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** Fireball
*/

#include "Mobs/Projectiles/Fireball.hpp"

namespace rtype {

static void agressive_behavior(RealEngine::Registry& registry, RealEngine::Entity entity,
                               float deltaTime) {
    // no agressive behavior
}

Fireball::Fireball(RealEngine::Registry& registry, sf::Vector2f position, float angle, float speed)
    : _entity(registry.spawn_entity()),
      _projSprite(*(RealEngine::AssetManager::getInstance().getSprite("fireball"))) {
    _projSpriteSheet.emplace("normal", _projSprite);
    registry.add_component(_entity, RealEngine::Position{position.x, position.y});
    registry.add_component(_entity, RealEngine::Velocity{0, 0, {850.f, 850.f}, 0.5f});
    registry.add_component(_entity, RealEngine::Acceleration{speed, speed, speed});
    registry.add_component(
        _entity,
        RealEngine::SpriteSheet{
            _projSpriteSheet, "normal", 0, {12, 12}, false, true, 100, {6, 6}, sf::Clock()});
    registry.add_component(_entity, RealEngine::Drawable{});
    registry.add_component(_entity,
                           RealEngine::Collision{{0.f, 0.f, 16.f * GAME_SCALE, 8.f * GAME_SCALE},
                                                 "mob",
                                                 false,
                                                 RealEngine::CollisionType::ENEMY_BULLET,
                                                 destroyOnWallsAndPlayer});
    registry.add_component(_entity, RealEngine::AI{agressive_behavior, goStraightAngle, true});
    registry.add_component(_entity, RealEngine::Damage{10, true, 3.0f, 1.0f, false});
    registry.add_component(_entity, RealEngine::Health{40, 40});
    registry.add_component(_entity, RealEngine::Rotation{angle});
}

Fireball::~Fireball() {}

}  // namespace rtype