/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** MiniBossShoot
*/

#include "Game/Mobs/Projectiles/MiniBossShoot.hpp"

namespace rtype {

MiniBossShoot::MiniBossShoot(RealEngine::Registry& registry, sf::Vector2f position, float angle,
                             float speed)
    : _entity(registry.spawn_entity()) {
    registry.add_component(
        _entity, RealEngine::Interpolation{
                     {position.x, position.y}, {position.x, position.y}, 0.f, 1.f, false});
    registry.add_component(_entity, RealEngine::Position{position.x, position.y});
    registry.add_component(_entity, RealEngine::Velocity{0, 0, {120.f, 120.f}, 0.5f});
    auto spriteSheet =
        *RealEngine::AssetManager::getInstance().getSpriteSheet("mini_boss_projectile");
    registry.add_component(_entity, RealEngine::SpriteSheet{spriteSheet});
    registry.add_component(_entity, RealEngine::AI{noBehavior, goStraightConstantAngle, true});
    registry.add_component(_entity, RealEngine::Damage{25});
    registry.add_component(_entity, RealEngine::Health{1, 1});
    registry.add_component(_entity, RealEngine::Rotation{angle});
    registry.add_component(_entity, RealEngine::AutoDestructible{-1.0f, true, false});
    registry.add_component(
        _entity, RealEngine::NetvarContainer{{
                     {"sprite_name",
                      {"string", "sprite_name", std::string("mini_boss_projectile"), nullptr}},
                     {"new_entity", {"bool", "new_entity", true, nullptr}},
                 }});
}

MiniBossShoot::~MiniBossShoot() {}

}  // namespace rtype