/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** EyeBomber
*/

#include "Mobs/EyeMinion.hpp"

namespace rtype {

static void simpleBehavior(RealEngine::Registry& registry, RealEngine::Entity entity,
                           float deltaTime) {
    auto* eyeRotation     = registry.get_component<RealEngine::Rotation>(entity);
    auto* eyeVelocity     = registry.get_component<RealEngine::Velocity>(entity);
    auto* eyeAcceleration = registry.get_component<RealEngine::Acceleration>(entity);

    if (!eyeRotation || !eyeVelocity || !eyeAcceleration)
        std::cout << "Error: EyeMinion components not found!" << std::endl;
}

EyeMinion::EyeMinion(RealEngine::Registry& registry, sf::Vector2f position, sf::Vector2f direction,
                     float speed)
    : _eyeEntity(registry.spawn_entity()),
      _eyeSprite(*(RealEngine::AssetManager::getInstance().getSprite("eye_minion"))) {
    _eyeSheet.emplace("normal", _eyeSprite);

    registry.add_component(_eyeEntity, RealEngine::Position{position.x, position.y});
    registry.add_components(
        _eyeEntity,
        RealEngine::SpriteSheet{
            _eyeSheet, "normal", 0, {18, 11}, false, true, 120, {14, 5}, sf::Clock()},
        RealEngine::Drawable{});
    registry.add_component(_eyeEntity, RealEngine::Velocity{0.0f, 0.0f, {135.0f, 135.0f}, 0.8f});
    registry.add_component(_eyeEntity, RealEngine::Acceleration{60.0f, 5.0f, 0.5f});
    registry.add_component(_eyeEntity, RealEngine::Rotation{0.0f});
    registry.add_component(
        _eyeEntity,
        RealEngine::Collision{
            {0.0f, 0.0f, 15.f * GAME_SCALE, 10.f * GAME_SCALE},
            "mob",
            false,
            RealEngine::CollisionType::ENEMY,
            [this](RealEngine::CollisionType collisionType, RealEngine::Registry& registry,
                   RealEngine::Entity collider, RealEngine::Entity entity) {
                collisionBehaviour(collisionType, registry, collider, entity);
            }});
    registry.add_component(_eyeEntity, RealEngine::Health{50, 50});
    registry.add_component(_eyeEntity, RealEngine::AI{rushTowardsTarget, simpleBehavior, true});
    registry.add_component(_eyeEntity, RealEngine::Damage{40});
}

EyeMinion::~EyeMinion() {}

void EyeMinion::setTarget(std::shared_ptr<RealEngine::Entity> target,
                          RealEngine::Registry&               registry) {
    auto* acceleration = registry.get_component<RealEngine::Acceleration>(_eyeEntity);

    acceleration->ax = 240.0f;
    acceleration->ay = 240.0f;
    registry.add_component(_eyeEntity, RealEngine::Target{target});
}

void EyeMinion::collisionBehaviour(RealEngine::CollisionType collisionType,
                                   RealEngine::Registry& registry, RealEngine::Entity collider,
                                   RealEngine::Entity entity) {
    switch (collisionType) {
        case RealEngine::CollisionType::INACTIVE:
            break;
        case RealEngine::CollisionType::SOLID:
            break;
        case RealEngine::CollisionType::ENEMY:
            break;
        case RealEngine::CollisionType::PICKABLE:
            break;
        case RealEngine::CollisionType::PLAYER:
            selfDestruct(registry, entity);
            break;
        default:
            break;
    }
}

}  // namespace rtype