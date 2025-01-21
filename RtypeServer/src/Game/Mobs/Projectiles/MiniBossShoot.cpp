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
    : _entity(registry.spawnEntity()) {
    registry.addComponent(_entity,
                          RealEngine::Interpolation{
                              {position.x, position.y}, {position.x, position.y}, 0.f, 1.f, false});
    registry.addComponent(_entity, RealEngine::Position{position.x, position.y});
    registry.addComponent(_entity, RealEngine::Velocity{0, 0, {120.f, 120.f}, 0.5f});
    auto spriteSheet =
        *RealEngine::AssetManager::getInstance().getSpriteSheet("mini_boss_projectile");
    registry.addComponent(_entity, RealEngine::SpriteSheet{spriteSheet});
    registry.addComponent(_entity, RealEngine::AI{noBehavior, goStraightConstantAngle, true});
    registry.addComponent(_entity, RealEngine::Damage{25});
    registry.addComponent(_entity, RealEngine::Health{1, 1});
    registry.addComponent(_entity, RealEngine::Rotation{angle});
    registry.addComponent(_entity, RealEngine::AutoDestructible{-1.0f, true, false});
    registry.addComponent(
        _entity, RealEngine::NetvarContainer{{
                     {"sprite_name",
                      {"string", "sprite_name", std::string("mini_boss_projectile"), nullptr}},
                     {"new_entity", {"bool", "new_entity", true, nullptr}},
                 }});
}

MiniBossShoot::~MiniBossShoot() {}

}  // namespace rtype