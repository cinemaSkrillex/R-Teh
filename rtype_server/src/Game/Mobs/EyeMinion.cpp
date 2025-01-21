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
    registry.addComponent(entity, RealEngine::Acceleration{-60.0f, 0.0f, 0.5f});
    goStraightConstant(registry, entity, deltaTime);
}

static void adjustSpeedAndRush(RealEngine::Registry& registry, RealEngine::Entity entity,
                               float deltaTime) {
    registry.addComponent(entity, RealEngine::Acceleration{60.0f, 30.0f, 0.5f});
    rushAndAimTowardsTarget(registry, entity, deltaTime);
}

static void updateShootCooldown(RealEngine::Registry& registry, RealEngine::Entity entity,
                                RealEngine::Netvar& currentNetvar, float deltaTime) {
    auto* target = registry.getComponent<RealEngine::Target>(entity);
    if (!target) return;

    auto* container = registry.getComponent<RealEngine::NetvarContainer>(entity);
    auto* position  = registry.getComponent<RealEngine::Position>(entity);
    auto* rotation  = registry.getComponent<RealEngine::Rotation>(entity);
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
    : _entity(registry.spawnEntity()) {
    registry.addComponent(_entity, RealEngine::Position{position.x, position.y});
    registry.addComponent(_entity,
                          RealEngine::Interpolation{
                              {position.x, position.y}, {position.x, position.y}, 0.f, 1.f, false});
    registry.addComponent(_entity, RealEngine::Drawable{});
    auto spriteSheet = *RealEngine::AssetManager::getInstance().getSpriteSheet("eye_minion");
    registry.addComponent(_entity, RealEngine::SpriteSheet{spriteSheet});
    registry.addComponent(_entity, RealEngine::Velocity{0.0f, 0.0f, {135.0f, 135.0f}, 0.8f});
    registry.addComponent(_entity, RealEngine::Acceleration{60.0f, 30.0f, 0.5f});
    registry.addComponent(_entity, RealEngine::Rotation{180.0f});
    registry.addComponent(_entity,
                          RealEngine::Collision{{0.0f, 0.0f, 15.f * GAME_SCALE, 10.f * GAME_SCALE},
                                                "mob",
                                                false,
                                                RealEngine::CollisionType::ENEMY,
                                                takesDamage});
    registry.addComponent(_entity, RealEngine::Health{15, 15});
    registry.addComponent(_entity, RealEngine::AI{adjustSpeedAndRush, simpleBehavior, true});
    registry.addComponent(_entity, RealEngine::TargetRadius{200.0f});
    registry.addComponent(_entity, RealEngine::Damage{5});
    registry.addComponent(
        _entity, RealEngine::NetvarContainer{{
                     {"sprite_name", {"string", "sprite_name", std::string("eye_minion"), nullptr}},
                     {"new_entity", {"bool", "new_entity", true, nullptr}},
                     {"entity_type", {"int", "entity_type", 2, nullptr}},
                     {"destroy_out_of_screen", {"bool", "destroy_out_of_screen", false, nullptr}},
                     {"shootCooldown", {"float", "shootCooldown", 3.5f, updateShootCooldown}},
                     {"powerup_drop", {"float", "powerup_drop", 20.f, nullptr}},
                 }});
    registry.addComponent(_entity, RealEngine::Score{50});
}

EyeMinion::~EyeMinion() {}

void EyeMinion::setTarget(std::shared_ptr<RealEngine::Entity> target,
                          RealEngine::Registry&               registry) {
    auto* acceleration = registry.getComponent<RealEngine::Acceleration>(_entity);

    acceleration->ax = 240.0f;
    acceleration->ay = 240.0f;
    registry.addComponent(_entity, RealEngine::Target{target});
}

}  // namespace rtype