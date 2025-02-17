/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** Fireball
*/

#include "Game/Mobs/Projectiles/Fireball.hpp"

namespace rtype {

Fireball::Fireball(RealEngine::Registry& registry, sf::Vector2f position, float angle)
    : _entity(registry.spawnEntity()) {
    registry.addComponent(_entity, RealEngine::Position{position.x, position.y});
    registry.addComponent(_entity,
                          RealEngine::Interpolation{
                              {position.x, position.y}, {position.x, position.y}, 0.f, 1.f, false});
    registry.addComponent(_entity, RealEngine::Velocity{0, 0, {250.f, 250.f}, 0.5f});
    registry.addComponent(_entity, RealEngine::Acceleration{100.f, 100.f, 20.f});
    auto spriteSheet = *RealEngine::AssetManager::getInstance().getSpriteSheet("fireball");
    registry.addComponent(_entity, RealEngine::SpriteSheet{spriteSheet});
    registry.addComponent(_entity, RealEngine::Drawable{});
    registry.addComponent(_entity,
                          RealEngine::Collision{{0.f, 0.f, 16.f * GAME_SCALE, 8.f * GAME_SCALE},
                                                "mob",
                                                false,
                                                RealEngine::CollisionType::ENEMY_BULLET,
                                                destroyOnWallsAndPlayer});
    registry.addComponent(_entity, RealEngine::AI{noBehavior, goStraightConstantAngle, true});
    registry.addComponent(_entity, RealEngine::Damage{2, true, 3.0f, 1.0f, false});
    registry.addComponent(_entity, RealEngine::Health{1, 1});
    registry.addComponent(_entity, RealEngine::Rotation{angle});
    registry.addComponent(_entity, RealEngine::AutoDestructible{-1.0f, true, false});
    registry.addComponent(
        _entity, RealEngine::NetvarContainer{
                     {{"sprite_name", {"string", "sprite_name", std::string("fireball"), nullptr}},
                      {"new_entity", {"bool", "new_entity", true, nullptr}}}});
}

Fireball::~Fireball() {}

}  // namespace rtype