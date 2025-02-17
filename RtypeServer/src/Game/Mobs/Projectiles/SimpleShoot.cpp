/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** SimpleShoot
*/

#include "Game/Mobs/Projectiles/SimpleShoot.hpp"

namespace rtype {

SimpleShoot::SimpleShoot(RealEngine::Registry& registry, sf::Vector2f position, float angle)
    : _entity(registry.spawnEntity()) {
    registry.addComponent(_entity, RealEngine::Position{position.x, position.y});
    registry.addComponent(_entity, RealEngine::Velocity{0, 0, {210.f, 210.f}, 0.5f});
    registry.addComponent(_entity, RealEngine::Acceleration{210.f, 210.f, 105.f});
    registry.addComponent(
        _entity, RealEngine::SpriteComponent{
                     *(RealEngine::AssetManager::getInstance().getSprite("simple_shoot"))});
    registry.addComponent(_entity,
                          RealEngine::Interpolation{
                              {position.x, position.y}, {position.x, position.y}, 0.f, 1.f, false});
    registry.addComponent(_entity,
                          RealEngine::Collision{{0.f, 0.f, 16.f * GAME_SCALE, 8.f * GAME_SCALE},
                                                "mob",
                                                false,
                                                RealEngine::CollisionType::ENEMY_BULLET,
                                                takesDamage});
    registry.addComponent(_entity, RealEngine::AI{noBehavior, goStraightConstantAngle, true});
    registry.addComponent(_entity, RealEngine::Damage{10});
    registry.addComponent(_entity, RealEngine::Health{5, 5});
    registry.addComponent(_entity, RealEngine::Rotation{angle});
    registry.addComponent(_entity, RealEngine::AutoDestructible{-1.0f, true, false});
    registry.addComponent(_entity, RealEngine::Score{5});
    registry.addComponent(
        _entity,
        RealEngine::NetvarContainer{
            {{"sprite_name", {"string", "sprite_name", std::string("simple_shoot"), nullptr}},
             {"new_entity", {"bool", "new_entity", true, nullptr}}}});
}

SimpleShoot::~SimpleShoot() {}

}  // namespace rtype