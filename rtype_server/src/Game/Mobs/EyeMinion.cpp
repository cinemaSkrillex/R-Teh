/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** EyeBomber
*/

#include "Game/Mobs/EyeMinion.hpp"

namespace rtype {

static void simpleBehavior(RealEngine::Registry& registry, RealEngine::Entity entity,
                           float deltaTime) {
    auto* eyeRotation     = registry.get_component<RealEngine::Rotation>(entity);
    auto* eyeVelocity     = registry.get_component<RealEngine::Velocity>(entity);
    auto* eyeAcceleration = registry.get_component<RealEngine::Acceleration>(entity);

    // eyeVelocity->vx += eyeAcceleration->ax * deltaTime;
    // eyeVelocity->vy += eyeAcceleration->ay * deltaTime;
    // eyeRotation->angle = std::fmod(eyeRotation->angle + 360.0f, 360.0f);
}

static void updateShootCooldown(RealEngine::Registry& registry, RealEngine::Entity entity,
                                RealEngine::Netvar& currentNetvar, float deltaTime) {
    auto* target = registry.get_component<RealEngine::Target>(entity);
    if (!target) return;

    auto* container = registry.get_component<RealEngine::NetvarContainer>(entity);
    auto* position  = registry.get_component<RealEngine::Position>(entity);
    auto* rotation  = registry.get_component<RealEngine::Rotation>(entity);
    float cooldown  = std::any_cast<float>(currentNetvar.value);

    if (cooldown <= 0) {
        if (rand() % 2 == 0) {
            SmallLaser laser(registry, {position->x, position->y}, rotation->angle);
            cooldown = 3.5f;
        } else {
            cooldown = 1.5f;
        }
    }
    cooldown -= deltaTime;
    currentNetvar.value = cooldown;
}

void mob_take_damage(RealEngine::Registry& registry, RealEngine::Entity collider,
                     RealEngine::Entity entity) {
    std::cerr << "mob take damage" << std::endl;
    auto* health       = registry.get_component<RealEngine::Health>(entity);
    auto* bulletHealth = registry.get_component<RealEngine::Health>(collider);

    if (health) {
        health->damage += 10;
    }
    if (bulletHealth) {
        bulletHealth->damage += 10;
    }
}

void mob_collision_handler(RealEngine::CollisionType collisionType, RealEngine::Registry& registry,
                           RealEngine::Entity collider, RealEngine::Entity entity) {
    switch (collisionType) {
        case RealEngine::CollisionType::INACTIVE:
            break;
        case RealEngine::CollisionType::SOLID:
            break;
        case RealEngine::CollisionType::PICKABLE:
            break;
        case RealEngine::CollisionType::OTHER:
            break;
        case RealEngine::CollisionType::PLAYER:
            std::cerr << "PLAYER COLLIDED" << std::endl;
            mob_take_damage(registry, collider, entity);
            break;
        case RealEngine::CollisionType::ALLY_BULLET:
            mob_take_damage(registry, collider, entity);
            break;
        case RealEngine::CollisionType::ENEMY_BULLET:
            break;
        default:
            break;
    }
}

EyeMinion::EyeMinion(RealEngine::Registry& registry, sf::Vector2f position, sf::Vector2f direction,
                     float speed)
    : _eyeEntity(registry.spawn_entity()) {
    registry.add_component(_eyeEntity, RealEngine::Position{position.x, position.y});
    registry.add_component(_eyeEntity, RealEngine::Drawable{});
    auto spriteSheet = *RealEngine::AssetManager::getInstance().getSpriteSheet("eye_minion");
    registry.add_component(_eyeEntity, RealEngine::SpriteSheet{spriteSheet});
    registry.add_component(_eyeEntity, RealEngine::Velocity{0.0f, 0.0f, {135.0f, 135.0f}, 0.8f});
    registry.add_component(_eyeEntity, RealEngine::Acceleration{60.0f, 30.0f, 0.5f});
    registry.add_component(_eyeEntity, RealEngine::Rotation{0.0f});
    registry.add_component(_eyeEntity,
                           RealEngine::Collision{{0.0f, 0.0f, 15.f * GAME_SCALE, 10.f * GAME_SCALE},
                                                 "mob",
                                                 false,
                                                 RealEngine::CollisionType::ENEMY,
                                                 takesDamage});

    registry.add_component(_eyeEntity, RealEngine::Health{50, 50});
    registry.add_component(_eyeEntity,
                           RealEngine::AI{rushAndAimTowardsTarget, simpleBehavior, true});
    registry.add_component(_eyeEntity, RealEngine::Damage{5});
    registry.add_component(
        _eyeEntity,
        RealEngine::NetvarContainer{
            {{"sprite_name", {"string", "sprite_name", std::string("eye_minion"), nullptr}},
             {"shootCooldown", {"float", "shootCooldown", 3.5f, updateShootCooldown}}}});
}

EyeMinion::~EyeMinion() {}

void EyeMinion::setTarget(std::shared_ptr<RealEngine::Entity> target,
                          RealEngine::Registry&               registry) {
    auto* acceleration = registry.get_component<RealEngine::Acceleration>(_eyeEntity);

    acceleration->ax = 240.0f;
    acceleration->ay = 240.0f;
    registry.add_component(_eyeEntity, RealEngine::Target{target});
}

}  // namespace rtype