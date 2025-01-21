/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** RobotBossMinion
*/

#include "Game/Mobs/RobotBossMinion.hpp"

namespace rtype {

static void goForward(RealEngine::Registry& registry, RealEngine::Entity entity, float deltaTime) {
    auto* position     = registry.getComponent<RealEngine::Position>(entity);
    auto* velocity     = registry.getComponent<RealEngine::Velocity>(entity);
    auto* acceleration = registry.getComponent<RealEngine::Acceleration>(entity);

    if (position && velocity && acceleration) {
        velocity->vx += acceleration->ax;
        velocity->vy = acceleration->ay;
    }
}

static void alignOnTargetOnY(RealEngine::Registry& registry, RealEngine::Entity entity,
                             float deltaTime) {
    goForward(registry, entity, deltaTime);
    auto  entity_target  = registry.getComponent<RealEngine::Target>(entity);
    auto* position       = registry.getComponent<RealEngine::Position>(entity);
    auto* velocity       = registry.getComponent<RealEngine::Velocity>(entity);
    auto* targetPosition = registry.getComponent<RealEngine::Position>(entity_target->target);
    auto* acceleration   = registry.getComponent<RealEngine::Acceleration>(entity);

    if (position && targetPosition) {
        float dy = targetPosition->y - position->y;

        if (std::abs(dy) > 1.0f) {
            velocity->vy = (dy > 0) ? std::min(acceleration->ay, dy / deltaTime)
                                    : std::max(-acceleration->ay, dy / deltaTime);
        } else {
            velocity->vy = 0;
        }
    }
}

static void updateShootCooldown(RealEngine::Registry& registry, RealEngine::Entity entity,
                                RealEngine::Netvar& currentNetvar, float deltaTime) {
    auto* container = registry.getComponent<RealEngine::NetvarContainer>(entity);
    auto* position  = registry.getComponent<RealEngine::Position>(entity);
    auto* rotation  = registry.getComponent<RealEngine::Rotation>(entity);
    float cooldown  = std::any_cast<float>(currentNetvar.value);

    if (cooldown <= 0) {
        if (rand() % 4 <= 2) {
            SpaceMissile shoot(registry, {position->x, position->y}, 180.f);
            cooldown = 5.f;
        } else {
            cooldown = 5.f;
        }
    }
    cooldown -= deltaTime;
    currentNetvar.value = cooldown;
}

RobotBossMinion::RobotBossMinion(RealEngine::Registry& registry, sf::Vector2f position)
    : _entity(registry.spawnEntity()) {
    registry.addComponent(_entity, RealEngine::Position{position.x, position.y});
    registry.addComponent(_entity,
                          RealEngine::Interpolation{
                              {position.x, position.y}, {position.x, position.y}, 0.f, 1.f, false});
    registry.addComponent(_entity, RealEngine::Velocity{0, 0, {130.f, 200.f}, 0.5f});
    registry.addComponent(_entity, RealEngine::Acceleration{
                                       -10.0f, position.y > VIEW_HEIGHT / 2 ? -50.f : 50.f, 0.5f});
    auto spriteSheet = *RealEngine::AssetManager::getInstance().getSpriteSheet("robot_boss_minion");
    registry.addComponent(_entity, RealEngine::SpriteSheet{spriteSheet});
    registry.addComponent(_entity,
                          RealEngine::Collision{{0.f, 0.f, 16.f * GAME_SCALE, 8.f * GAME_SCALE},
                                                "mob",
                                                false,
                                                RealEngine::CollisionType::ENEMY,
                                                takesDamage});
    registry.addComponent(_entity, RealEngine::AI{noBehavior, noBehavior, true});
    registry.addComponent(_entity, RealEngine::Damage{15});
    registry.addComponent(_entity, RealEngine::Health{30, 30});
    registry.addComponent(_entity, RealEngine::TargetRadius{200.0f});
    registry.addComponent(_entity, RealEngine::AI{alignOnTargetOnY, goForward, true});
    registry.addComponent(_entity, RealEngine::Rotation{0.f});
    registry.addComponent(_entity, RealEngine::AutoDestructible{-1.0f, true, false});
    registry.addComponent(
        _entity,
        RealEngine::NetvarContainer{{
            {"sprite_name", {"string", "sprite_name", std::string("robot_boss_minion"), nullptr}},
            {"new_entity", {"bool", "new_entity", true, nullptr}},
            {"destroy_out_of_screen", {"bool", "destroy_out_of_screen", false, nullptr}},
            {"shoot_cooldown", {"float", "shoot_cooldown", 5.0f, updateShootCooldown}},
            {"powerup_drop", {"float", "powerup_drop", 50.f, nullptr}},
        }});
}

RobotBossMinion::~RobotBossMinion() {}

}  // namespace rtype