/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** DirectionalCanon
*/

#include "Game/Mobs/DirectionalCanon.hpp"

namespace rtype {

static void updateDiagShootCooldown(RealEngine::Registry& registry, RealEngine::Entity entity,
                                    RealEngine::Netvar& currentNetvar, float deltaTime) {
    auto* container = registry.getComponent<RealEngine::NetvarContainer>(entity);
    auto* position  = registry.getComponent<RealEngine::Position>(entity);
    float cooldown  = std::any_cast<float>(currentNetvar.value);

    if (cooldown <= 0) {
        if (position) {
            Fireball fireball(registry, {position->x, position->y}, 45);
            Fireball fireball2(registry, {position->x, position->y}, 135);
            Fireball fireball3(registry, {position->x, position->y}, 225);
            Fireball fireball4(registry, {position->x, position->y}, 315);
        }
        cooldown = 8.f;
    }
    cooldown -= deltaTime;
    currentNetvar.value = cooldown;
}

static void updateDirectShootCooldown(RealEngine::Registry& registry, RealEngine::Entity entity,
                                      RealEngine::Netvar& currentNetvar, float deltaTime) {
    auto* container = registry.getComponent<RealEngine::NetvarContainer>(entity);
    auto* position  = registry.getComponent<RealEngine::Position>(entity);
    float cooldown  = std::any_cast<float>(currentNetvar.value);

    if (cooldown <= 0) {
        if (position) {
            Fireball fireball(registry, {position->x, position->y}, 0);
            Fireball fireball2(registry, {position->x, position->y}, 90);
            Fireball fireball3(registry, {position->x, position->y}, 180);
            Fireball fireball4(registry, {position->x, position->y}, 270);
        }
        cooldown = 8.f;
    }
    cooldown -= deltaTime;
    currentNetvar.value = cooldown;
}

DirectionalCanon::DirectionalCanon(RealEngine::Registry& registry, sf::Vector2f position,
                                   float level_speed)
    : _entity(registry.spawnEntity()) {
    registry.addComponent(_entity, RealEngine::Position{position.x, position.y});
    registry.addComponent(_entity,
                          RealEngine::Interpolation{
                              {position.x, position.y}, {position.x, position.y}, 0.f, 1.f, false});
    registry.addComponent(_entity, RealEngine::Acceleration{level_speed, 0, 0});
    auto spriteSheet = *RealEngine::AssetManager::getInstance().getSpriteSheet("directional_canon");
    registry.addComponent(_entity, RealEngine::SpriteSheet{spriteSheet});
    registry.addComponent(_entity,
                          RealEngine::Collision{{0.f, 0.f, 16.f * GAME_SCALE, 8.f * GAME_SCALE},
                                                "mob",
                                                false,
                                                RealEngine::CollisionType::ENEMY,
                                                takesDamage});
    registry.addComponent(_entity, RealEngine::AI{noBehavior, moveWithoutVelocity, true});
    registry.addComponent(_entity, RealEngine::Damage{10});
    registry.addComponent(_entity, RealEngine::Health{40, 40});
    registry.addComponent(_entity, RealEngine::Rotation{0.f});
    registry.addComponent(_entity, RealEngine::AutoDestructible{-1.0f, true, false});

    registry.addComponent(
        _entity,
        RealEngine::NetvarContainer{{
            {"sprite_name", {"string", "sprite_name", std::string("directional_canon"), nullptr}},
            {"new_entity", {"bool", "new_entity", true, nullptr}},
            {"diagShootCooldown", {"float", "diagShootCooldown", 8.f, updateDiagShootCooldown}},
            {"directShootCooldown",
             {"float", "directShootCooldown", 4.f, updateDirectShootCooldown}},
            {"powerup_drop", {"float", "powerup_drop", 10.f, nullptr}},
            {"powerup_type", {"int", "powerup_type", 1, nullptr}},
        }});
}

DirectionalCanon::~DirectionalCanon() {}

}  // namespace rtype