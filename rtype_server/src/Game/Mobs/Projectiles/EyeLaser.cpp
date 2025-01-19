/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** EyeLaser
*/

#include "Game/Mobs/Projectiles/EyeLaser.hpp"

namespace rtype {

EyeLaser::EyeLaser(RealEngine::Registry& registry, sf::Vector2f position, float angle)
    : _entity(registry.spawn_entity()) {
    registry.add_component(_entity, RealEngine::Position{position.x, position.y});
    registry.add_component(_entity, RealEngine::Velocity{0, 0, {150.f, 150.f}, 0.f});
    registry.add_component(_entity, RealEngine::Acceleration{200.f, 200.f, 200.f});
    registry.add_component(
        _entity, RealEngine::Interpolation{
                     {position.x, position.y}, {position.x, position.y}, 0.f, 1.f, false});
    registry.add_component(
        _entity, RealEngine::SpriteComponent{
                     {*(RealEngine::AssetManager::getInstance().getSprite("eye_laser"))}});
    registry.add_component(_entity, RealEngine::Drawable{});
    registry.add_component(_entity,
                           RealEngine::Collision{{0.f, 0.f, 16.f * GAME_SCALE, 8.f * GAME_SCALE},
                                                 "enemy_bullet",
                                                 false,
                                                 RealEngine::CollisionType::ENEMY_BULLET,
                                                 noCollisionBehavior});
    registry.add_component(_entity, RealEngine::AI{noBehavior, goStraightConstantAngle, true});
    registry.add_component(_entity, RealEngine::Damage{10});
    registry.add_component(_entity, RealEngine::Health{1, 1});
    registry.add_component(_entity, RealEngine::Rotation{angle});
    registry.add_component(_entity, RealEngine::AutoDestructible{-1.0f, true, false});
    registry.add_component(
        _entity, RealEngine::NetvarContainer{
                     {{"sprite_name", {"string", "sprite_name", std::string("eye_laser"), nullptr}},
                      {"new_entity", {"bool", "new_entity", true, nullptr}}}});
}

EyeLaser::~EyeLaser() {}

}  // namespace rtype