/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** WallTurret
*/

#include "Game/Mobs/WallTurret.hpp"

namespace rtype {

static void straight_line_behavior(RealEngine::Registry& registry, RealEngine::Entity entity,
                                   float deltaTime) {
    auto* position = registry.get_component<RealEngine::Position>(entity);
    auto* velocity = registry.get_component<RealEngine::Velocity>(entity);
}

static void aimAtTarget(RealEngine::Entity& entity, RealEngine::Position* targetPosition,
                        RealEngine::Registry& registry, float rotationSpeed, float deltaTime) {
    auto* position = registry.get_component<RealEngine::Position>(entity);
    auto* rotation = registry.get_component<RealEngine::Rotation>(entity);

    if (position && targetPosition) {
        float dx          = targetPosition->x - position->x;
        float dy          = targetPosition->y - position->y;
        float targetAngle = std::atan2(dy, dx) * 180.0f / M_PI;

        if (rotation) {
            float currentAngle    = rotation->angle;
            targetAngle           = std::fmod(targetAngle + 360.0f, 360.0f);
            currentAngle          = std::fmod(currentAngle + 360.0f, 360.0f);
            float angleDifference = targetAngle - currentAngle;

            if (angleDifference > 180.0f) {
                angleDifference -= 360.0f;
            } else if (angleDifference < -180.0f) {
                angleDifference += 360.0f;
            }
            if (angleDifference > 0.1f) {
                rotation->angle += rotationSpeed * deltaTime * 100.0f;
            } else if (angleDifference < -0.1f) {
                rotation->angle -= rotationSpeed * deltaTime * 100.0f;
            }
            rotation->angle = std::fmod(rotation->angle + 360.0f, 360.0f);
        }
    }
}

static void agressive_behavior(RealEngine::Registry& registry, RealEngine::Entity entity,
                               float deltaTime) {
    auto target         = registry.get_component<RealEngine::Target>(entity);
    auto targetPosition = registry.get_component<RealEngine::Position>(target->target);
    aimAtTarget(entity, targetPosition, registry, 0.75f, deltaTime);
}

WallTurret::WallTurret(RealEngine::Registry& registry, sf::Vector2f position, bool onGround)
    : _canonEntity(registry.spawn_entity()),
      _pedestalEntity(registry.spawn_entity()),
      _canonSprite(*(RealEngine::AssetManager::getInstance().getSprite("turret_canon"))),
      _pedestalSprite(*(RealEngine::AssetManager::getInstance().getSprite("turret_pedestal"))) {
    _canonSprite.setOrigin(5, 5);
    registry.add_component(_canonEntity, RealEngine::Position{position.x, position.y});
    registry.add_component(_canonEntity, RealEngine::SpriteComponent{_canonSprite});
    registry.add_component(_canonEntity, RealEngine::Drawable{});
    // registry.add_component(
    //     _canonEntity, RealEngine::Collision{
    //                  {0.f, 0.f, 16.f * GAME_SCALE, 8.f * GAME_SCALE},
    //                  "mob",
    //                  false,
    //                  RealEngine::CollisionType::ENEMY,
    //                  [this](RealEngine::CollisionType collisionType, RealEngine::Registry&
    //                  registry,
    //                         RealEngine::Entity collider, RealEngine::Entity entity) {
    //                      mob_collision_handler(collisionType, registry, collider, entity);
    //                  }});
    registry.add_component(_canonEntity,
                           RealEngine::AI{agressive_behavior, straight_line_behavior, true});
    registry.add_component(_canonEntity, RealEngine::Damage{10});
    registry.add_component(_canonEntity, RealEngine::Health{40, 40});
    registry.add_component(_canonEntity, RealEngine::Rotation{0.f});

    registry.add_component(_pedestalEntity,
                           RealEngine::Position{position.x, position.y + onGround ? -5.f : 5.f});
    registry.add_component(_pedestalEntity, RealEngine::SpriteComponent{_pedestalSprite});
    registry.add_component(_pedestalEntity, RealEngine::Drawable{});
    registry.add_component(_pedestalEntity, RealEngine::Rotation{0.f});
}

WallTurret::~WallTurret() {}

void WallTurret::setTarget(std::shared_ptr<RealEngine::Entity> target,
                           RealEngine::Registry&               registry) {
    registry.add_component(_canonEntity, RealEngine::Target{target});
}

}  // namespace rtype