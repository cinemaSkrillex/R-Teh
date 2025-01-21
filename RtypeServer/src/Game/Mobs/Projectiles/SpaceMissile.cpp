/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** SpaceMissile
*/

#include "Game/Mobs/Projectiles/SpaceMissile.hpp"

namespace rtype {

static void agressive_behavior(RealEngine::Registry& registry, RealEngine::Entity entity,
                               float deltaTime) {
    auto* target         = registry.getComponent<RealEngine::Target>(entity);
    auto* targetPosition = registry.getComponent<RealEngine::Position>(target->target);

    registry.addComponent(entity, RealEngine::Acceleration{30.0f, 30.0f, 0.5f});
    aimAtTarget(entity, targetPosition, registry, 0.08f, deltaTime);
    goStraightAngle(registry, entity, deltaTime);
}

SpaceMissile::SpaceMissile(RealEngine::Registry& registry, sf::Vector2f position, float angle)
    : _entity(registry.spawnEntity()) {
    registry.addComponent(_entity, RealEngine::Position{position.x, position.y});
    registry.addComponent(_entity,
                          RealEngine::Interpolation{
                              {position.x, position.y}, {position.x, position.y}, 0.f, 1.f, false});
    registry.addComponent(_entity, RealEngine::Velocity{0, 0, {90.f, 90.f}, 0.5f});
    registry.addComponent(_entity, RealEngine::Acceleration{-40.f, 0.f, 0.5f});
    registry.addComponent(
        _entity, RealEngine::SpriteComponent{
                     *(RealEngine::AssetManager::getInstance().getSprite("space_missile"))});
    registry.addComponent(_entity,
                          RealEngine::Collision{{0.f, 0.f, 16.f * GAME_SCALE, 8.f * GAME_SCALE},
                                                "mob",
                                                false,
                                                RealEngine::CollisionType::ENEMY_BULLET,
                                                takesDamage});
    registry.addComponent(_entity, RealEngine::AI{agressive_behavior, goStraightConstant, true});
    registry.addComponent(_entity, RealEngine::TargetRadius{200.0f});
    registry.addComponent(_entity, RealEngine::Damage{20});
    registry.addComponent(_entity, RealEngine::Health{10, 10});
    registry.addComponent(_entity, RealEngine::Rotation{angle});
    registry.addComponent(_entity, RealEngine::AutoDestructible{-1.0f, true, false});
    registry.addComponent(
        _entity,
        RealEngine::NetvarContainer{{
            {"sprite_name", {"string", "sprite_name", std::string("space_missile"), nullptr}},
            {"new_entity", {"bool", "new_entity", true, nullptr}},
        }});
}

SpaceMissile::~SpaceMissile() {}

}  // namespace rtype