/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** EyeMinion
*/

#include "Game/Mobs/EyeMinion.hpp"

namespace rtype {

static void simpleBehavior(RealEngine::Registry& registry, RealEngine::Entity entity,
                           float deltaTime) {
    auto* eyeRotation     = registry.get_component<RealEngine::Rotation>(entity);
    auto* eyeVelocity     = registry.get_component<RealEngine::Velocity>(entity);
    auto* eyeAcceleration = registry.get_component<RealEngine::Acceleration>(entity);
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

EyeMinion::EyeMinion(RealEngine::Registry& registry, sf::Vector2f position)
    : _entity(registry.spawn_entity()) {
    registry.add_component(_entity, RealEngine::Position{position.x, position.y});
    registry.add_component(
        _entity, RealEngine::Interpolation{
                     {position.x, position.y}, {position.x, position.y}, 0.f, 1.f, false});
    registry.add_component(_entity, RealEngine::Drawable{});
    auto spriteSheet = *RealEngine::AssetManager::getInstance().getSpriteSheet("eye_minion");
    registry.add_component(_entity, RealEngine::SpriteSheet{spriteSheet});
    registry.add_component(_entity, RealEngine::Velocity{0.0f, 0.0f, {135.0f, 135.0f}, 0.8f});
    registry.add_component(_entity, RealEngine::Acceleration{60.0f, 30.0f, 0.5f});
    registry.add_component(_entity, RealEngine::Rotation{0.0f});
    registry.add_component(_entity,
                           RealEngine::Collision{{0.0f, 0.0f, 15.f * GAME_SCALE, 10.f * GAME_SCALE},
                                                 "mob",
                                                 false,
                                                 RealEngine::CollisionType::ENEMY,
                                                 takesDamage});

    registry.add_component(_entity, RealEngine::Health{15, 50});
    registry.add_component(_entity, RealEngine::AI{rushAndAimTowardsTarget, simpleBehavior, true});
    registry.add_component(_entity, RealEngine::TargetRadius{200.0f});
    registry.add_component(_entity, RealEngine::Damage{5});
    registry.add_component(_entity, RealEngine::AutoDestructible{-1.0f, true, false});
    registry.add_component(
        _entity,
        RealEngine::NetvarContainer{
            {{"sprite_name", {"string", "sprite_name", std::string("eye_minion"), nullptr}},
             {"shootCooldown", {"float", "shootCooldown", 3.5f, updateShootCooldown}},
             {"new_entity", {"bool", "new_entity", true, nullptr}},
             {"powerup_drop", {"float", "powerup_drop", 70.f, nullptr}},
             {"entity_type", {"int", "entity_type", 2, nullptr}}}});
    registry.add_component(_entity, RealEngine::Score{50});
}

EyeMinion::~EyeMinion() {}

void EyeMinion::setTarget(std::shared_ptr<RealEngine::Entity> target,
                          RealEngine::Registry&               registry) {
    auto* acceleration = registry.get_component<RealEngine::Acceleration>(_entity);

    acceleration->ax = 240.0f;
    acceleration->ay = 240.0f;
    registry.add_component(_entity, RealEngine::Target{target});
}

}  // namespace rtype